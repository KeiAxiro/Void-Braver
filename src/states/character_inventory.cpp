#include "states.h"
#include "database.h"
#include "utils/console_ui.h"
#include <algorithm>
#include <iostream>
#include <string>


// ---- merged from src/states/character_menu.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void characterMenu(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            showCharacterOverview(ctx);
            std::cout << colorText("1. Allocate Stats", Color::Green) << '\n';
            std::cout << colorText("2. Inventory & Equipment", Color::Cyan) << '\n';
            std::cout << colorText("3. Skill List", Color::Yellow) << '\n';
            std::cout << colorText("4. Back", Color::Magenta) << '\n';
            std::cout << "Choose: ";

            const std::string choice = readLine();
            if (choice == "1")
                allocateStats(ctx);
            else if (choice == "2")
                inventoryMenu(ctx);
            else if (choice == "3")
                skillMenu(ctx);
            else if (choice == "4")
                return;
            else
            {
                std::cout << "Pilihan tidak valid.\n";
                waitForEnter();
            }
        }
    }
} // namespace state_helpers

// ---- merged from src/states/character_progression.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void allocateStats(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            refreshPlayerResources(ctx);
            printStateHeader(ctx, "ALLOCATE STAT POINTS");
            cout << colorText("Available Points", Color::Yellow, true) << ": " << ctx.player.stat_points << "\n\n";
            cout << colorText("1. STR", Color::Red) << " (" << ctx.player.stats.str << ")\n";
            cout << colorText("2. INT", Color::Cyan) << " (" << ctx.player.stats.intl << ")\n";
            cout << colorText("3. AGI", Color::Green) << " (" << ctx.player.stats.agi << ")\n";
            cout << colorText("4. VIT", Color::Yellow) << " (" << ctx.player.stats.vit << ")\n";
            cout << colorText("5. Back", Color::Magenta) << '\n';
            cout << "Choose stat: ";

            const string choice = readLine();
            if (choice == "5")
                return;

            if (ctx.player.stat_points <= 0)
            {
                cout << "Tidak ada stat point tersisa.\n";
                waitForEnter();
                return;
            }

            int statIndex = 0;
            if (!tryParseInt(choice, statIndex) || statIndex < 1 || statIndex > 4)
            {
                cout << "Pilihan tidak valid.\n";
                waitForEnter();
                continue;
            }

            cout << "Masukkan jumlah point yang ingin dialokasikan: ";
            const string amountInput = readLine();
            int amount = 0;
            if (!tryParseInt(amountInput, amount) || amount <= 0)
            {
                cout << "Jumlah tidak valid.\n";
                waitForEnter();
                continue;
            }

            amount = min(amount, ctx.player.stat_points);
            if (statIndex == 1)
                ctx.player.stats.str += amount;
            else if (statIndex == 2)
                ctx.player.stats.intl += amount;
            else if (statIndex == 3)
                ctx.player.stats.agi += amount;
            else if (statIndex == 4)
                ctx.player.stats.vit += amount;

            ctx.player.stat_points -= amount;
            refreshPlayerResources(ctx);
            cout << "Stat berhasil dialokasikan. HP/MP maksimum ikut diperbarui.\n";
            waitForEnter();
        }
    }

    int skillUnlockLevelForIndex(int listIndex)
    {
        if (listIndex <= 1)
            return 1;
        return 1 + (listIndex - 1) * 2;
    }

    bool isSkillUnlockedByLevel(const Player &player, int listIndex)
    {
        return player.level >= skillUnlockLevelForIndex(listIndex);
    }

    SkillTreeNode *buildSkillTree(const json &skills, int leftIndex, int rightIndex)
    {
        if (!skills.is_array() || leftIndex > rightIndex)
            return nullptr;

        const int mid = (leftIndex + rightIndex) / 2;
        SkillTreeNode *node = new SkillTreeNode;
        node->skill = &skills[static_cast<std::size_t>(mid - 1)];
        node->list_index = mid;
        node->unlock_level = skillUnlockLevelForIndex(mid);
        node->left = buildSkillTree(skills, leftIndex, mid - 1);
        node->right = buildSkillTree(skills, mid + 1, rightIndex);
        return node;
    }

    const json *findSkillInTree(SkillTreeNode *root, int listIndex)
    {
        if (root == nullptr)
            return nullptr;
        if (root->list_index == listIndex)
            return root->skill;
        if (listIndex < root->list_index)
            return findSkillInTree(root->left, listIndex);
        return findSkillInTree(root->right, listIndex);
    }

    void clearSkillTree(SkillTreeNode *root)
    {
        if (root == nullptr)
            return;
        clearSkillTree(root->left);
        clearSkillTree(root->right);
        delete root;
    }

    void printSkillTreeInOrder(const Player &player, SkillTreeNode *root)
    {
        if (root == nullptr || root->skill == nullptr)
            return;

        printSkillTreeInOrder(player, root->left);

        const bool unlocked = isSkillUnlockedByLevel(player, root->list_index);
        cout << colorText(to_string(root->list_index) + ". " + root->skill->value("name", string()), unlocked ? Color::Yellow : Color::White, true) << '\n';
        cout << "   Unlock Lv: " << root->unlock_level
             << " | Status: " << (unlocked ? "Unlocked" : "Locked")
             << " | Mana: " << root->skill->value("mana_cost", 0)
             << " | Cooldown: " << root->skill->value("cooldown_turns", 0)
             << " | CD Saat Ini: " << getCooldown(player, root->skill->value("id", string())) << '\n';
        if (root->skill->contains("damage_formula"))
            cout << "   Formula: " << root->skill->value("damage_formula", string("-")) << '\n';
        cout << "   " << root->skill->value("mechanic_description", string("-")) << "\n\n";

        printSkillTreeInOrder(player, root->right);
    }

    void skillMenu(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        clearScreen();
        printStateHeader(ctx, "SKILL LIST");
        if (!cls || !cls->contains("skills") || !(*cls)["skills"].is_array())
        {
            cout << "Skill tidak tersedia.\n";
            waitForEnter();
            return;
        }

        SkillTreeNode *root = buildSkillTree((*cls)["skills"], 1, static_cast<int>((*cls)["skills"].size()));
        cout << "Binary Skill Tree Traversal: InOrder\n\n";
        printSkillTreeInOrder(ctx.player, root);
        clearSkillTree(root);

        printLine('=');
        waitForEnter();
    }
} // namespace state_helpers

// ---- merged from src/states/inventory_menu.cpp



using namespace std;
using namespace consoleui;

namespace state_helpers
{
    bool equipInventoryEntry(GameContext &ctx, size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return false;

        InventoryEntry selected = ctx.player.inventory[index];
        const auto *item = getItemById(ctx, selected.item_id);
        if (!item)
            return false;

        const string category = item->value("category", string());
        if (!isEquipmentCategory(category))
        {
            cout << "Item ini bukan equipment.\n";
            return false;
        }

        const string requiredClass = item->value("required_class_id", string());
        if (!requiredClass.empty() && requiredClass != ctx.player.class_id)
        {
            cout << "Class kamu tidak cocok untuk item ini.\n";
            return false;
        }

        const string slot = slotFromCategory(category);
        for (auto &entry : ctx.player.inventory)
        {
            if (entry.equipped && entry.slot == slot)
            {
                entry.equipped = false;
                entry.slot.clear();
            }
        }

        if (ctx.player.inventory[index].quantity > 1)
        {
            ctx.player.inventory[index].quantity -= 1;
            addItem(ctx.player, selected.item_id, 1, true, slot);
        }
        else
        {
            ctx.player.inventory[index].equipped = true;
            ctx.player.inventory[index].slot = slot;
        }

        mergeInventory(ctx.player);
        refreshPlayerResources(ctx);
        cout << item->value("name", selected.item_id) << " berhasil di-equip pada slot " << slot << ".\n";
        return true;
    }

    bool unequipInventoryEntry(GameContext &ctx, size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return false;
        if (!ctx.player.inventory[index].equipped)
            return false;

        if (ctx.player.inventory[index].quantity > 1)
        {
            ctx.player.inventory[index].quantity -= 1;
            addItem(ctx.player, ctx.player.inventory[index].item_id, 1, false, "");
        }
        else
        {
            ctx.player.inventory[index].equipped = false;
            ctx.player.inventory[index].slot.clear();
        }

        mergeInventory(ctx.player);
        refreshPlayerResources(ctx);
        cout << "Equipment berhasil dilepas.\n";
        return true;
    }

    void useConsumable(GameContext &ctx, size_t index)
    {
        if (index >= ctx.player.inventory.size())
            return;

        const auto *item = getItemById(ctx, ctx.player.inventory[index].item_id);
        if (!item)
            return;
        if (item->value("category", string()) != "consumable")
        {
            cout << "Item ini bukan consumable.\n";
            return;
        }

        const string nameLower = toLower(item->value("name", string()));
        int healHp = 0;
        int healMp = 0;
        if (nameLower.find("mana") != string::npos)
            healMp = max(20, ctx.player.max_mp / 4);
        else
            healHp = max(30, ctx.player.max_hp / 4);

        ctx.player.hp = min(ctx.player.max_hp, ctx.player.hp + healHp);
        ctx.player.mp = min(ctx.player.max_mp, ctx.player.mp + healMp);
        removeItem(ctx.player, ctx.player.inventory[index].item_id, 1);
        cout << item->value("name", string()) << " digunakan. ";
        if (healHp > 0)
            cout << "HP +" << healHp;
        if (healMp > 0)
            cout << "MP +" << healMp;
        cout << '\n';
    }

    void inventoryMenu(GameContext &ctx)
    {
        int page = 1;
        while (true)
        {
            clearScreen();
            mergeInventory(ctx.player);
            refreshPlayerResources(ctx);

            const int totalItems = static_cast<int>(ctx.player.inventory.size());
            const int totalPages = max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
            page = clampInt(page, 1, totalPages);

            const int start = (page - 1) * PAGE_SIZE;
            const int end = min(totalItems, start + PAGE_SIZE);

            printStateHeader(ctx, "INVENTORY");
            cout << colorText("Page", Color::Cyan, true) << ": " << page << "/" << totalPages << '\n';
            cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold << "\n\n";
            if (totalItems == 0)
                cout << "(Kosong)\n";

            for (int i = start; i < end; ++i)
            {
                const auto &entry = ctx.player.inventory[static_cast<size_t>(i)];
                const auto *item = getItemById(ctx, entry.item_id);
                const string name = item ? item->value("name", entry.item_id) : entry.item_id;
                const string category = item ? item->value("category", string()) : "-";

                cout << (i - start + 1) << ". " << name << " x" << entry.quantity
                     << " [" << category << "]";
                if (entry.equipped)
                    cout << " [Equipped:" << entry.slot << "]";
                cout << '\n';
            }

            cout << "Input nomor item, q prev, e next, c kembali: ";
            const string input = toLower(readLine());
            if (input == "c")
                return;
            if (input == "q")
            {
                page = max(1, page - 1);
                continue;
            }
            if (input == "e")
            {
                page = min(totalPages, page + 1);
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
                const size_t selectedIndex = static_cast<size_t>(start + number - 1);
                const auto *item = getItemById(ctx, ctx.player.inventory[selectedIndex].item_id);
                if (!item)
                {
                    cout << "Data item tidak ditemukan.\n";
                    waitForEnter();
                    continue;
                }

                clearScreen();
                printStateHeader(ctx, item->value("name", string()));

                int equippedCount = 0;
                for (const auto &entry : ctx.player.inventory)
                {
                    if (entry.item_id == ctx.player.inventory[selectedIndex].item_id && entry.equipped)
                        equippedCount += entry.quantity;
                }

                showItemDetails(ctx, *item, inventoryCount(ctx.player, ctx.player.inventory[selectedIndex].item_id), equippedCount);

                cout << "1. Equip/Unequip\n";
                cout << "2. Use Consumable\n";
                cout << "3. Back\n";
                cout << "Choose: ";
                const string action = readLine();

                if (action == "1")
                {
                    if (ctx.player.inventory[selectedIndex].equipped)
                        unequipInventoryEntry(ctx, selectedIndex);
                    else
                        equipInventoryEntry(ctx, selectedIndex);
                    waitForEnter();
                }
                else if (action == "2")
                {
                    useConsumable(ctx, selectedIndex);
                    waitForEnter();
                }

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
} // namespace state_helpers

// ---- merged from src/states/item_catalog_features.cpp



using namespace std;

namespace state_helpers
{
    bool canCraftItem(const GameContext &ctx, const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].is_object() || !item["crafting"].contains("ingredients"))
            return false;

        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const string itemId = ingredient.value("item_id", string());
            const int quantity = ingredient.value("quantity", 0);
            if (inventoryCount(ctx.player, itemId) < quantity)
                return false;
        }

        return true;
    }

    int ownedIngredientCount(const GameContext &ctx, const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].contains("ingredients"))
            return 0;

        int owned = 0;
        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const string itemId = ingredient.value("item_id", string());
            owned += min(inventoryCount(ctx.player, itemId), ingredient.value("quantity", 0));
        }
        return owned;
    }

    int totalIngredientNeed(const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].contains("ingredients"))
            return 0;

        int total = 0;
        for (const auto &ingredient : item["crafting"]["ingredients"])
            total += ingredient.value("quantity", 0);
        return total;
    }

    StringList collectCraftingCategories(const GameContext &ctx)
    {
        StringList categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const string categoryName = category.get<string>();
            bool exists = false;

            for (const auto &item : ctx.gameData["items"])
            {
                if (item.value("category", string()) == categoryName &&
                    item.contains("crafting") &&
                    item["crafting"].contains("ingredients"))
                {
                    exists = true;
                    break;
                }
            }

            if (exists)
                categories.push_back(categoryName);
        }

        return categories;
    }

    StringList categoriesForShopMode(const GameContext &ctx, const string &mode)
    {
        StringList categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const string categoryName = category.get<string>();
            bool exists = false;

            for (const auto &item : ctx.gameData["items"])
            {
                if (item.value("category", string()) != categoryName)
                    continue;

                if (mode == "buy" && item.value("buy_price", 0) > 0)
                {
                    exists = true;
                    break;
                }

                if (mode == "sell" && item.value("sell_price", 0) > 0 &&
                    inventoryCount(ctx.player, item.value("id", string())) > 0)
                {
                    exists = true;
                    break;
                }
            }

            if (exists)
                categories.push_back(categoryName);
        }

        return categories;
    }

    void showRecipeDetail(const GameContext &ctx, const json &item)
    {
        cout << MENU_SEPARATOR << '\n';
        cout << item.value("name", string()) << '\n';
        cout << "Category: " << item.value("category", string()) << '\n';
        cout << "Tier: " << item.value("tier_level", 0) << '\n';
        cout << "Required Class: " << item.value("required_class_id", string("-")) << '\n';
        cout << "Stats -> ATK +" << item["stats"].value("atk_bonus", 0)
             << ", DEF +" << item["stats"].value("def_bonus", 0)
             << ", HP +" << item["stats"].value("hp_bonus", 0)
             << ", MP +" << item["stats"].value("mp_bonus", 0) << '\n';
        cout << "Ingredients:\n";

        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const string ingredientId = ingredient.value("item_id", string());
            const int need = ingredient.value("quantity", 0);
            const int own = inventoryCount(ctx.player, ingredientId);
            cout << " - " << itemDisplayName(ctx, ingredientId) << " : " << own << "/" << need << '\n';
        }

        cout << MENU_SEPARATOR << '\n';
    }
} // namespace state_helpers

// ---- merged from src/states/crafting_menu.cpp



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
                JsonPointerList recipes;
                for (const auto &item : ctx.gameData["items"])
                {
                    if (item.value("category", std::string()) != selectedCategory)
                        continue;
                    if (!item.contains("crafting") || !item["crafting"].contains("ingredients"))
                        continue;
                    recipes.push_back(&item);
                }

                manualSort(recipes, [&](const json *left, const json *right)
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

// ---- merged from src/states/shop_menu.cpp



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
                JsonPointerList items;
                for (const auto &item : ctx.gameData["items"])
                {
                    if (item.value("category", std::string()) != selectedCategory)
                        continue;
                    if (item.value("buy_price", 0) <= 0)
                        continue;
                    items.push_back(&item);
                }

                manualSort(items, [&](const json *left, const json *right)
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
            IndexList inventoryIndexes;
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


