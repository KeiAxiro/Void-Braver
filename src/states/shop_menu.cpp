#include "states/shop_features.h"

#include "database.h"
#include "states/dungeon_features.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
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
    void shopBuyMenu(GameContext &ctx)
    {
        const auto categories = categoriesForShopMode(ctx, "buy");
        if (categories.empty())
        {
            std::cout << "Shop belum punya item yang bisa dibeli.\n";
            waitForEnter();
            return;
        }

        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "SHOP BUY CATEGORY");
            for (std::size_t i = 0; i < categories.size(); ++i)
                std::cout << colorText(std::to_string(i + 1) + ". " + categories[i], Color::Yellow, true) << '\n';
            std::cout << "Pilih kategori atau c untuk kembali: ";

            const std::string input = toLower(readLine());
            if (input == "c")
                return;

            int categoryIndex = 0;
            if (!tryParseInt(input, categoryIndex) || categoryIndex < 1 || categoryIndex > static_cast<int>(categories.size()))
            {
                std::cout << "Kategori tidak valid.\n";
                waitForEnter();
                continue;
            }

            const std::string selectedCategory = categories[static_cast<std::size_t>(categoryIndex - 1)];
            int page = 1;
            while (true)
            {
                clearScreen();
                std::vector<const json *> items;
                for (const auto &item : ctx.gameData["items"])
                {
                    if (item.value("category", std::string()) != selectedCategory)
                        continue;
                    if (item.value("buy_price", 0) <= 0)
                        continue;
                    items.push_back(&item);
                }

                std::sort(items.begin(), items.end(), [&](const json *left, const json *right)
                          {
                    const bool leftClass = left->value("required_class_id", std::string()) == ctx.player.class_id;
                    const bool rightClass = right->value("required_class_id", std::string()) == ctx.player.class_id;
                    if (leftClass != rightClass)
                        return leftClass > rightClass;
                    if (left->value("tier_level", 0) != right->value("tier_level", 0))
                        return left->value("tier_level", 0) < right->value("tier_level", 0);
                    return left->value("name", std::string()) < right->value("name", std::string()); });

                const int totalItems = static_cast<int>(items.size());
                const int totalPages = std::max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
                page = clampInt(page, 1, totalPages);
                const int start = (page - 1) * PAGE_SIZE;
                const int end = std::min(totalItems, start + PAGE_SIZE);

                printStateHeader(ctx, "SHOP BUY");
                std::cout << colorText("Category", Color::Cyan, true) << ": " << selectedCategory
                          << " | Page: " << page << "/" << totalPages << '\n';
                std::cout << "\n";
                for (int i = start; i < end; ++i)
                {
                    const json &item = *items[static_cast<std::size_t>(i)];
                    std::cout << colorText(std::to_string(i - start + 1) + ". " + item.value("name", std::string()), Color::White, true)
                              << " | Price " << item.value("buy_price", 0)
                              << " | Tier " << item.value("tier_level", 0);
                    // jika class mismatch, tampilkan [Class mismatch] jika null (string) atau tidak ada field required_class_id, anggap cocok dengan semua class
                    // if (item.contains("required_class_id") && !item.value("required_class_id", std::string()).empty())
                    // {
                    //     if (item.value("required_class_id", std::string()) != ctx.player.class_id )
                    //         std::cout << colorText(" [Class mismatch]", Color::Red, true);
                    // }

                    // if (item.value("required_class_id", std::string()) != ctx.player.class_id)
                    //     std::cout << " [Class mismatch]";

                    // print merah jika missmatch class, tampilkan kuning jika null
                    if (item.contains("required_class_id"))
                    {
                        if (item.value("required_class_id", std::string()) == "null")
                            std::cout << colorText(" [All classes]", Color::Yellow, true);
                        else if (item.value("required_class_id", std::string()) != ctx.player.class_id)
                            std::cout << colorText(" [Class mismatch]", Color::Red, true);
                    }
                    std::cout << '\n';
                }
                std::cout << "\nInput nomor item, q prev, e next, c kembali: ";

                const std::string shopInput = toLower(readLine());
                if (shopInput == "c")
                    break;
                if (shopInput == "q")
                {
                    page = std::max(1, page - 1);
                    continue;
                }
                if (shopInput == "e")
                {
                    page = std::min(totalPages, page + 1);
                    continue;
                }

                int number = 0;
                if (!tryParseInt(shopInput, number))
                {
                    std::cout << "Input tidak valid.\n";
                    waitForEnter();
                    continue;
                }

                if (number >= 1 && number <= (end - start))
                {
                    const json &item = *items[static_cast<std::size_t>(start + number - 1)];
                    clearScreen();
                    printStateHeader(ctx, "ITEM DETAIL");
                    showItemDetails(ctx, item, inventoryCount(ctx.player, item.value("id", std::string())), 0);
                    std::cout << colorText("1. Buy Item", Color::Green) << '\n';
                    std::cout << colorText("2. Back", Color::Magenta) << '\n';
                    std::cout << "Choose: ";

                    const std::string action = readLine();
                    if (action != "1")
                        continue;

                    std::cout << "Jumlah beli: ";
                    const std::string quantityInput = readLine();
                    int quantity = 0;
                    if (!tryParseInt(quantityInput, quantity) || quantity <= 0)
                    {
                        std::cout << "Jumlah tidak valid.\n";
                        waitForEnter();
                        continue;
                    }

                    const int cost = quantity * item.value("buy_price", 0);
                    if (ctx.player.gold < cost)
                    {
                        std::cout << "Gold tidak cukup.\n";
                        waitForEnter();
                        continue;
                    }

                    ctx.player.gold -= cost;
                    addItem(ctx.player, item.value("id", std::string()), quantity, false, "");
                    std::cout << "Berhasil membeli " << quantity << " x " << item.value("name", std::string()) << ".\n";
                    saveGame(ctx);
                    waitForEnter();
                    continue;
                }

                if (number >= 1 && number <= totalPages)
                {
                    page = number;
                    continue;
                }

                std::cout << "Nomor tidak valid.\n";
                waitForEnter();
            }
        }
    }

    void shopSellMenu(GameContext &ctx)
    {
        const auto categories = categoriesForShopMode(ctx, "sell");
        if (categories.empty())
        {
            std::cout << "Tidak ada item yang bisa dijual.\n";
            waitForEnter();
            return;
        }

        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "SHOP SELL CATEGORY");
            for (std::size_t i = 0; i < categories.size(); ++i)
                std::cout << colorText(std::to_string(i + 1) + ". " + categories[i], Color::Yellow, true) << '\n';
            std::cout << "Pilih kategori atau c untuk kembali: ";

            const std::string input = toLower(readLine());
            if (input == "c")
                return;

            int categoryIndex = 0;
            if (!tryParseInt(input, categoryIndex) || categoryIndex < 1 || categoryIndex > static_cast<int>(categories.size()))
            {
                std::cout << "Kategori tidak valid.\n";
                waitForEnter();
                continue;
            }

            const std::string selectedCategory = categories[static_cast<std::size_t>(categoryIndex - 1)];
            std::vector<std::size_t> inventoryIndexes;
            for (std::size_t i = 0; i < ctx.player.inventory.size(); ++i)
            {
                const auto *item = getItemById(ctx, ctx.player.inventory[i].item_id);
                if (!item)
                    continue;
                if (item->value("category", std::string()) != selectedCategory)
                    continue;
                if (item->value("sell_price", 0) <= 0)
                    continue;
                inventoryIndexes.push_back(i);
            }

            if (inventoryIndexes.empty())
            {
                std::cout << "Tidak ada item kategori ini yang bisa dijual.\n";
                waitForEnter();
                continue;
            }

            clearScreen();
            printStateHeader(ctx, "SHOP SELL");
            std::cout << colorText("Category", Color::Cyan, true) << ": " << selectedCategory << '\n';
            std::cout << "\n";
            for (std::size_t i = 0; i < inventoryIndexes.size(); ++i)
            {
                const auto &entry = ctx.player.inventory[inventoryIndexes[i]];
                const auto *item = getItemById(ctx, entry.item_id);
                std::cout << colorText(std::to_string(i + 1) + ". " + item->value("name", entry.item_id), Color::White, true)
                          << " x" << entry.quantity
                          << " | Sell " << item->value("sell_price", 0);
                if (entry.equipped)
                    std::cout << " [Equipped]";
                std::cout << '\n';
            }
            std::cout << "Pilih nomor item atau c untuk kembali: ";

            const std::string sellInput = toLower(readLine());
            if (sellInput == "c")
                continue;

            int selected = 0;
            if (!tryParseInt(sellInput, selected) || selected < 1 || selected > static_cast<int>(inventoryIndexes.size()))
            {
                std::cout << "Pilihan tidak valid.\n";
                waitForEnter();
                continue;
            }

            const std::size_t inventoryIndex = inventoryIndexes[static_cast<std::size_t>(selected - 1)];
            const auto *item = getItemById(ctx, ctx.player.inventory[inventoryIndex].item_id);
            if (!item)
                continue;

            clearScreen();
            printStateHeader(ctx, "ITEM DETAIL");

            int equippedCount = 0;
            for (const auto &entry : ctx.player.inventory)
            {
                if (entry.item_id == ctx.player.inventory[inventoryIndex].item_id && entry.equipped)
                    equippedCount += entry.quantity;
            }

            showItemDetails(ctx, *item, inventoryCount(ctx.player, ctx.player.inventory[inventoryIndex].item_id), equippedCount);
            std::cout << colorText("1. Sell Item", Color::Yellow) << '\n';
            std::cout << colorText("2. Back", Color::Magenta) << '\n';
            std::cout << "Choose: ";

            const std::string action = readLine();
            if (action != "1")
                continue;

            std::cout << "Jumlah jual: ";
            const std::string quantityInput = readLine();
            int quantity = 0;
            if (!tryParseInt(quantityInput, quantity) || quantity <= 0)
            {
                std::cout << "Jumlah tidak valid.\n";
                waitForEnter();
                continue;
            }
            if (quantity > ctx.player.inventory[inventoryIndex].quantity)
            {
                std::cout << "Jumlah melebihi item yang kamu punya.\n";
                waitForEnter();
                continue;
            }

            ctx.player.gold += quantity * item->value("sell_price", 0);
            removeItem(ctx.player, ctx.player.inventory[inventoryIndex].item_id, quantity);
            std::cout << "Item berhasil dijual.\n";
            saveGame(ctx);
            waitForEnter();
        }
    }

    void shopMenu(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "SHOP");
            std::cout << colorText("1. Buy", Color::Green, true) << '\n';
            std::cout << colorText("2. Sell", Color::Yellow, true) << '\n';
            std::cout << colorText("3. Back", Color::Magenta, true) << '\n';
            std::cout << "Choose: ";

            const std::string choice = readLine();
            if (choice == "1")
                shopBuyMenu(ctx);
            else if (choice == "2")
                shopSellMenu(ctx);
            else if (choice == "3")
                return;
            else
            {
                std::cout << "Pilihan tidak valid.\n";
                waitForEnter();
            }
        }
    }

} // namespace state_helpers
