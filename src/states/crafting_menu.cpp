#include "states/crafting_features.h"

#include "database.h"
#include "states/dungeon_features.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void craftingMenu(GameContext &ctx)
    {
        const auto categories = collectCraftingCategories(ctx);
        if (categories.empty())
        {
            cout << "Belum ada kategori crafting yang tersedia.\n";
            waitForEnter();
            return;
        }

        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "CRAFTING CATEGORY");
            for (std::size_t i = 0; i < categories.size(); ++i)
                cout << colorText(std::to_string(i + 1) + ". " + categories[i], Color::Yellow, true) << '\n';
            cout << "Pilih kategori atau c untuk kembali: ";

            const std::string categoryInput = toLower(readLine());
            if (categoryInput == "c")
                return;

            int categoryIndex = 0;
            if (!tryParseInt(categoryInput, categoryIndex) || categoryIndex < 1 || categoryIndex > static_cast<int>(categories.size()))
            {
                cout << "Kategori tidak valid.\n";
                waitForEnter();
                continue;
            }

            const std::string selectedCategory = categories[static_cast<std::size_t>(categoryIndex - 1)];
            std::string mode = "craft";
            int page = 1;

            while (true)
            {
                clearScreen();
                std::vector<const json *> recipes;
                for (const auto &item : ctx.gameData["items"])
                {
                    if (item.value("category", std::string()) != selectedCategory)
                        continue;
                    if (!item.contains("crafting") || !item["crafting"].contains("ingredients"))
                        continue;
                    recipes.push_back(&item);
                }

                std::sort(recipes.begin(), recipes.end(), [&](const json *left, const json *right)
                          {
                    const bool leftCanCraft = canCraftItem(ctx, *left);
                    const bool rightCanCraft = canCraftItem(ctx, *right);
                    const bool leftClass = left->value("required_class_id", std::string()) == ctx.player.class_id;
                    const bool rightClass = right->value("required_class_id", std::string()) == ctx.player.class_id;
                    const int leftOwned = ownedIngredientCount(ctx, *left);
                    const int rightOwned = ownedIngredientCount(ctx, *right);
                    const int leftNeed = totalIngredientNeed(*left);
                    const int rightNeed = totalIngredientNeed(*right);

                    if (mode == "craft")
                    {
                        if (leftCanCraft != rightCanCraft)
                            return leftCanCraft > rightCanCraft;
                        if (leftClass != rightClass)
                            return leftClass > rightClass;
                    }
                    else
                    {
                        if (leftClass != rightClass)
                            return leftClass > rightClass;
                        if (leftOwned != rightOwned)
                            return leftOwned > rightOwned;
                    }

                    if (leftNeed != rightNeed)
                        return leftNeed < rightNeed;
                    return left->value("name", std::string()) < right->value("name", std::string()); });

                bool anyCraftableNow = false;
                for (const auto *recipe : recipes)
                {
                    if (canCraftItem(ctx, *recipe))
                    {
                        anyCraftableNow = true;
                        break;
                    }
                }
                if (mode == "craft" && !anyCraftableNow)
                    mode = "recipe";

                const int totalItems = static_cast<int>(recipes.size());
                const int totalPages = std::max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
                page = clampInt(page, 1, totalPages);
                const int start = (page - 1) * PAGE_SIZE;
                const int end = std::min(totalItems, start + PAGE_SIZE);

                printStateHeader(ctx, "CRAFTING");
                cout << colorText("Category", Color::Cyan, true) << ": " << selectedCategory
                     << " | Mode: " << (mode == "craft" ? "Craft" : "Recipe")
                     << " | Page: " << page << "/" << totalPages << '\n';
                cout << "\n";
                if (totalItems == 0)
                    cout << "(Tidak ada recipe pada kategori ini)\n";

                for (int i = start; i < end; ++i)
                {
                    const json &item = *recipes[static_cast<std::size_t>(i)];
                    cout << (i - start + 1) << ". " << item.value("name", std::string())
                         << " | Tier " << item.value("tier_level", 0);
                    if (canCraftItem(ctx, item))
                        cout << " [Craftable]";
                    if (item.value("required_class_id", std::string()) != ctx.player.class_id)
                        cout << " [Class mismatch]";
                    cout << '\n';
                }

                cout << "Input nomor item, q prev, e next, r ganti mode, c kembali";
                if (totalPages > 1)
                    cout << ", atau nomor page";
                cout << ": ";

                const std::string input = toLower(readLine());
                if (input == "c")
                    break;
                if (input == "q")
                {
                    page = std::max(1, page - 1);
                    continue;
                }
                if (input == "e")
                {
                    page = std::min(totalPages, page + 1);
                    continue;
                }
                if (input == "r")
                {
                    mode = (mode == "craft") ? "recipe" : "craft";
                    continue;
                }

                int number = 0;
                if (!tryParseInt(input, number))
                {
                    cout << "Input tidak valid.\n";
                    waitForEnter();
                    continue;
                }

                if (number >= 1 && number <= (end - start))
                {
                    const json &selectedItem = *recipes[static_cast<std::size_t>(start + number - 1)];
                    if (mode == "recipe")
                    {
                        showRecipeDetail(ctx, selectedItem);
                        waitForEnter();
                        continue;
                    }

                    showRecipeDetail(ctx, selectedItem);
                    if (!canCraftItem(ctx, selectedItem))
                    {
                        cout << "Material belum cukup untuk crafting item ini.\n";
                        waitForEnter();
                        continue;
                    }

                    cout << colorText("Konfirmasi crafting item ini? (y/n): ", Color::Yellow, true);
                    const std::string confirm = toLower(readLine());
                    if (confirm != "y" && confirm != "yes")
                    {
                        cout << "Crafting dibatalkan.\n";
                        waitForEnter();
                        continue;
                    }

                    for (const auto &ingredient : selectedItem["crafting"]["ingredients"])
                        removeItem(ctx.player, ingredient.value("item_id", std::string()), ingredient.value("quantity", 0));

                    addItem(ctx.player, selectedItem.value("id", std::string()), 1, false, "");
                    cout << "Berhasil crafting " << selectedItem.value("name", std::string()) << ".\n";
                    refreshPlayerResources(ctx);
                    saveGame(ctx);
                    waitForEnter();
                    continue;
                }

                if (number >= 1 && number <= totalPages)
                {
                    page = number;
                    continue;
                }

                cout << "Nomor tidak valid.\n";
                waitForEnter();
            }
        }
    }

} // namespace state_helpers
