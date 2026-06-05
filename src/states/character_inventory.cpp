#include "states/character_inventory.h"
#include "database.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <iostream>
#include <string>

// ---- Character menu
using namespace std;
using namespace consoleui;

namespace state_helpers
{
    Stats baseStatsForCurrentClass(const GameContext &ctx)
    {
        Stats stats;
        stats.str = Config::Defaults::PLAYER_STAT_STR;
        stats.intl = Config::Defaults::PLAYER_STAT_INT;
        stats.agi = Config::Defaults::PLAYER_STAT_AGI;
        stats.vit = Config::Defaults::PLAYER_STAT_VIT;

        const string primary = classPrimaryStat(ctx, ctx.player.class_id);
        if (primary == "INT")
            stats.intl += player_balance::kPrimaryStatBonusOnNewGame;
        else if (primary == "AGI")
            stats.agi += player_balance::kPrimaryStatBonusOnNewGame;
        else if (primary == "VIT")
            stats.vit += player_balance::kPrimaryStatBonusOnNewGame;
        else
            stats.str += player_balance::kPrimaryStatBonusOnNewGame;

        return stats;
    }

    void resetCharacterStats(GameContext &ctx)
    {
        const Stats baseStats = baseStatsForCurrentClass(ctx);
        const int refundedPoints =
            max(Config::Math::ZERO, ctx.player.stats.str - baseStats.str) +
            max(Config::Math::ZERO, ctx.player.stats.intl - baseStats.intl) +
            max(Config::Math::ZERO, ctx.player.stats.agi - baseStats.agi) +
            max(Config::Math::ZERO, ctx.player.stats.vit - baseStats.vit);

        clearScreen();
        printStateHeader(ctx, "RESET STATS");
        cout << "Stat akan dikembalikan ke base class dan " << refundedPoints << " point dialokasikan ulang.\n";
        cout << "Konfirmasi reset stats? (y/n): ";

        const string confirm = toLower(readLine());
        if (confirm != "y" && confirm != "yes")
        {
            cout << "Reset stats dibatalkan.\n";
            waitForEnter();
            return;
        }

        ctx.player.stats = baseStats;
        ctx.player.stat_points += refundedPoints;
        refreshPlayerResources(ctx);
        ctx.player.hp = ctx.player.max_hp;
        ctx.player.mp = ctx.player.max_mp;
        saveGame(ctx);

        cout << "Stats berhasil di-reset. HP dan MP dipulihkan penuh.\n";
        waitForEnter();
    }

    void characterMenu(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            showCharacterOverview(ctx);
            std::cout << colorText("1. Allocate Stats", Color::Green) << '\n';
            std::cout << colorText("2. Inventory & Equipment", Color::Cyan) << '\n';
            std::cout << colorText("3. Skill List", Color::Yellow) << '\n';
            std::cout << colorText("4. Reset Stats", Color::Red) << '\n';
            std::cout << colorText("5. Back", Color::Magenta) << '\n';
            std::cout << "Choose: ";

            const std::string choice = readLine();
            if (choice == "1")
                allocateStats(ctx);
            else if (choice == "2")
                inventoryMenu(ctx);
            else if (choice == "3")
                skillMenu(ctx);
            else if (choice == "4")
                resetCharacterStats(ctx);
            else if (choice == "5")
                return;
            else
            {
                std::cout << "Pilihan tidak valid.\n";
                waitForEnter();
            }
        }
    }
} // namespace state_helpers

// ---- Character progression and skills
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
        if (listIndex < skill_balance::kFirstSkillIndex)
            return Config::Defaults::PLAYER_LEVEL;

        const int unlockIndex = listIndex - skill_balance::kFirstSkillIndex;
        if (unlockIndex >= skill_balance::kUnlockLevelCount)
            return Config::Defaults::PLAYER_LEVEL;

        return skill_balance::kUnlockLevels[unlockIndex];
    }

    bool isSkillUnlockedByLevel(const Player &player, int listIndex)
    {
        return player.level >= skillUnlockLevelForIndex(listIndex);
    }

    SkillTreeNode *buildSkillTree(const json &skills, int leftIndex, int rightIndex)
    {
        if (!skills.is_array() || leftIndex > rightIndex)
            return nullptr;

        const int mid = (leftIndex + rightIndex) / skill_balance::kSkillTreeSplitDivisor;
        SkillTreeNode *node = new SkillTreeNode;
        node->skill = &skills[static_cast<std::size_t>(mid - Config::Math::INDEX_OFFSET)];
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

// ---- Inventory menu
using namespace std;
using namespace consoleui;

namespace state_helpers
{
    string itemClassRequirement(const json &item)
    {
        if (!item.contains("required_class_id") || item["required_class_id"].is_null())
            return "";

        string requiredClass;
        if (item["required_class_id"].is_string())
            requiredClass = item["required_class_id"].get<string>();
        else
            requiredClass = item.value("required_class_id", string());

        requiredClass = trim(requiredClass);
        if (toLower(requiredClass) == "null")
            return "";
        return requiredClass;
    }

    bool isItemClassCompatible(const GameContext &ctx, const json &item)
    {
        const string requiredClass = itemClassRequirement(item);
        return requiredClass.empty() || requiredClass == ctx.player.class_id;
    }

    void printClassAvailabilityTag(const GameContext &ctx, const json &item)
    {
        const string requiredClass = itemClassRequirement(item);
        if (requiredClass.empty())
            cout << colorText(" [All classes]", Color::Yellow, true);
        else if (requiredClass != ctx.player.class_id)
            cout << colorText(" [Class mismatch]", Color::Red, true);
    }

    StringList collectInventoryCategories(const GameContext &ctx)
    {
        StringList categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const string categoryName = category.get<string>();
            bool exists = false;

            for (const auto &entry : ctx.player.inventory)
            {
                const auto *item = getItemById(ctx, entry.item_id);
                if (item && item->value("category", string()) == categoryName)
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

    IndexList collectInventoryIndexes(const GameContext &ctx, const string &selectedCategory)
    {
        IndexList inventoryIndexes;
        for (size_t i = 0; i < ctx.player.inventory.size(); ++i)
        {
            const auto *item = getItemById(ctx, ctx.player.inventory[i].item_id);
            if (!item)
                continue;
            if (item->value("category", string()) != selectedCategory)
                continue;
            inventoryIndexes.push_back(i);
        }

        manualSort(inventoryIndexes, [&](size_t leftIndex, size_t rightIndex)
                   {
            const auto &left = ctx.player.inventory[leftIndex];
            const auto &right = ctx.player.inventory[rightIndex];
            if (left.equipped != right.equipped)
                return left.equipped > right.equipped;
            return leftIndex < rightIndex; });

        return inventoryIndexes;
    }

    bool removeInventoryEntryQuantity(Player &player, size_t index, int quantity)
    {
        if (quantity <= 0 || index >= player.inventory.size())
            return false;
        if (player.inventory[index].quantity < quantity)
            return false;

        size_t currentIndex = 0;
        for (auto it = player.inventory.begin(); it != player.inventory.end();)
        {
            if (currentIndex != index)
            {
                ++it;
                ++currentIndex;
                continue;
            }

            it->quantity -= quantity;
            if (it->quantity <= 0)
                player.inventory.erase(it);
            break;
        }

        mergeInventory(player);
        return true;
    }

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

        if (!isItemClassCompatible(ctx, *item))
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

        if (ctx.player.inventory[index].quantity > Config::Item::EQUIPMENT_SPLIT_THRESHOLD)
        {
            ctx.player.inventory[index].quantity -= Config::Item::EQUIPMENT_TRANSFER_QUANTITY;
            addItem(ctx.player, selected.item_id, Config::Item::EQUIPMENT_TRANSFER_QUANTITY, true, slot);
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

        if (ctx.player.inventory[index].quantity > Config::Item::EQUIPMENT_SPLIT_THRESHOLD)
        {
            ctx.player.inventory[index].quantity -= Config::Item::EQUIPMENT_TRANSFER_QUANTITY;
            addItem(ctx.player, ctx.player.inventory[index].item_id, Config::Item::EQUIPMENT_TRANSFER_QUANTITY, false, "");
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

    void useConsumable(GameContext &ctx, size_t index, BattleState *battle)
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

        const string itemId = item->value("id", string());
        const string itemName = item->value("name", string());
        if (itemId == "infinity_sphere")
        {
            if (battle)
            {
                battle->playerAtkBuffTurns = max(battle->playerAtkBuffTurns, 999);
                battle->playerDefBuffTurns = max(battle->playerDefBuffTurns, 999);
                battle->playerAgiBuffTurns = max(battle->playerAgiBuffTurns, 999);
                cout << itemName << " digunakan. Semua statistik battle meningkat 100% untuk sisa pertarungan.\n";
            }
            else
            {
                cout << itemName << " digunakan. Efek ini hanya aktif saat battle.\n";
            }
            removeItem(ctx.player, ctx.player.inventory[index].item_id, Config::Item::CONSUMABLE_USE_QUANTITY);
            return;
        }

        if (itemId == "soul_lantern")
        {
            const int targetHp = ctx.player.max_hp * 75 / 100;
            const int targetMp = ctx.player.max_mp * 75 / 100;
            const int healHp = max(0, targetHp - ctx.player.hp);
            const int healMp = max(0, targetMp - ctx.player.mp);

            ctx.player.hp = max(ctx.player.hp, targetHp);
            ctx.player.mp = max(ctx.player.mp, targetMp);
            removeItem(ctx.player, ctx.player.inventory[index].item_id, Config::Item::CONSUMABLE_USE_QUANTITY);
            cout << itemName << " digunakan. ";
            if (healHp > 0)
                cout << "HP +" << healHp;
            if (healMp > 0)
            {
                if (healHp > 0)
                    cout << ", ";
                cout << "MP +" << healMp;
            }
            if (healHp == 0 && healMp == 0)
                cout << "HP dan MP sudah berada di atas 75% maksimum.";
            cout << '\n';
            return;
        }

        const string nameLower = toLower(item->value("name", string()));
        int healHp = 0;
        int healMp = 0;
        if (nameLower.find("mana") != string::npos)
            healMp = max(Config::Item::CONSUMABLE_MP_MIN_HEAL, ctx.player.max_mp / Config::Item::CONSUMABLE_HEAL_DIVISOR);
        else
            healHp = max(Config::Item::CONSUMABLE_HP_MIN_HEAL, ctx.player.max_hp / Config::Item::CONSUMABLE_HEAL_DIVISOR);

        ctx.player.hp = min(ctx.player.max_hp, ctx.player.hp + healHp);
        ctx.player.mp = min(ctx.player.max_mp, ctx.player.mp + healMp);
        removeItem(ctx.player, ctx.player.inventory[index].item_id, Config::Item::CONSUMABLE_USE_QUANTITY);
        cout << itemName << " digunakan. ";
        if (healHp > 0)
            cout << "HP +" << healHp;
        if (healMp > 0)
            cout << "MP +" << healMp;
        cout << '\n';
    }

    void inventoryMenu(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            mergeInventory(ctx.player);
            refreshPlayerResources(ctx);

            const auto categories = collectInventoryCategories(ctx);

            printStateHeader(ctx, "INVENTORY CATEGORY");
            cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold << "\n\n";
            if (categories.empty())
                cout << "(Inventory kosong)\n";
            for (size_t i = 0; i < categories.size(); ++i)
                cout << colorText(to_string(i + 1) + ". " + categories[i], Color::Yellow, true) << '\n';

            cout << "Pilih kategori, s shop, r crafting, c kembali: ";
            const string categoryInput = toLower(readLine());
            if (categoryInput == "c")
                return;
            if (categoryInput == "s")
            {
                shopMenu(ctx);
                continue;
            }
            if (categoryInput == "r")
            {
                craftingMenu(ctx);
                continue;
            }

            int categoryIndex = 0;
            if (!tryParseInt(categoryInput, categoryIndex) || categoryIndex < 1 || categoryIndex > static_cast<int>(categories.size()))
            {
                cout << "Kategori tidak valid.\n";
                waitForEnter();
                continue;
            }

            const string selectedCategory = categories[static_cast<size_t>(categoryIndex - 1)];
            int page = 1;

            while (true)
            {
                clearScreen();
                mergeInventory(ctx.player);
                refreshPlayerResources(ctx);

                IndexList inventoryIndexes = collectInventoryIndexes(ctx, selectedCategory);
                const int totalItems = static_cast<int>(inventoryIndexes.size());
                const int totalPages = max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
                page = clampInt(page, 1, totalPages);
                const int start = (page - 1) * PAGE_SIZE;
                const int end = min(totalItems, start + PAGE_SIZE);

                printStateHeader(ctx, "INVENTORY");
                cout << colorText("Category", Color::Cyan, true) << ": " << selectedCategory
                     << " | Page: " << page << "/" << totalPages;
                cout << '\n';
                cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold << "\n\n";

                if (totalItems == 0)
                    cout << "(Tidak ada item pada kategori ini)\n";

                for (int i = start; i < end; ++i)
                {
                    const size_t inventoryIndex = inventoryIndexes[static_cast<size_t>(i)];
                    const auto &entry = ctx.player.inventory[inventoryIndex];
                    const auto *item = getItemById(ctx, entry.item_id);
                    if (!item)
                        continue;

                    cout << colorText(to_string(i - start + 1) + ". " + item->value("name", entry.item_id), Color::White, true)
                         << " x" << entry.quantity
                         << " | Tier " << item->value("tier_level", 0);
                    if (entry.equipped)
                        cout << colorText(" [Equipped:" + entry.slot + "]", Color::Green, true);
                    printClassAvailabilityTag(ctx, *item);
                    cout << '\n';
                }

                cout << "\nInput nomor item, q prev, e next, s shop, r crafting, c kategori";
                if (totalPages > 1)
                    cout << ", atau nomor page";
                cout << ": ";

                const string input = toLower(readLine());
                if (input == "c")
                    break;
                if (input == "s")
                {
                    shopMenu(ctx);
                    page = 1;
                    continue;
                }
                if (input == "r")
                {
                    craftingMenu(ctx);
                    page = 1;
                    continue;
                }
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
                    const size_t selectedIndex = inventoryIndexes[static_cast<size_t>(start + number - 1)];
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

                    cout << colorText("1. Equip/Unequip", Color::Green) << '\n';
                    cout << colorText("2. Use Consumable", Color::Yellow) << '\n';
                    cout << colorText("3. Back", Color::Magenta) << '\n';
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
    }
} // namespace state_helpers

// ---- Item catalog helpers
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

// ---- Crafting menu
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
                    const bool leftClass = isItemClassCompatible(ctx, *left);
                    const bool rightClass = isItemClassCompatible(ctx, *right);
                    const int leftOwned = ownedIngredientCount(ctx, *left);
                    const int rightOwned = ownedIngredientCount(ctx, *right);
                    const int leftNeed = totalIngredientNeed(*left);
                    const int rightNeed = totalIngredientNeed(*right);

                    if (leftClass != rightClass)
                        return leftClass > rightClass;

                    if (mode == "craft")
                    {
                        if (leftCanCraft != rightCanCraft)
                            return leftCanCraft > rightCanCraft;
                    }
                    else
                    {
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
                     << " | Page: " << page << "/" << totalPages;
                cout << '\n';
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
                    printClassAvailabilityTag(ctx, item);
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

                    addItem(ctx.player, selectedItem.value("id", std::string()), Config::Item::CRAFT_RESULT_QUANTITY, false, "");
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

// ---- Shop menu
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
                    const bool leftClass = isItemClassCompatible(ctx, *left);
                    const bool rightClass = isItemClassCompatible(ctx, *right);
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
                          << " | Page: " << page << "/" << totalPages;
                std::cout << '\n';
                std::cout << "\n";
                if (totalItems == 0)
                    std::cout << "(Tidak ada item pada kategori ini)\n";
                for (int i = start; i < end; ++i)
                {
                    const json &item = *items[static_cast<std::size_t>(i)];
                    std::cout << colorText(std::to_string(i - start + 1) + ". " + item.value("name", std::string()), Color::White, true)
                              << " | Price " << item.value("buy_price", 0)
                              << " | Tier " << item.value("tier_level", 0);
                    printClassAvailabilityTag(ctx, item);
                    std::cout << '\n';
                }
                std::cout << "\nInput nomor item, q prev, e next, c kembali";
                if (totalPages > 1)
                    std::cout << ", atau nomor page";
                std::cout << ": ";

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
        while (true)
        {
            const auto categories = categoriesForShopMode(ctx, "sell");
            if (categories.empty())
            {
                std::cout << "Tidak ada item yang bisa dijual.\n";
                waitForEnter();
                return;
            }

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
            int page = 1;

            while (true)
            {
                clearScreen();
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

                manualSort(inventoryIndexes, [&](std::size_t leftIndex, std::size_t rightIndex)
                           {
                    const auto *leftItem = getItemById(ctx, ctx.player.inventory[leftIndex].item_id);
                    const auto *rightItem = getItemById(ctx, ctx.player.inventory[rightIndex].item_id);
                    const bool leftClass = leftItem && isItemClassCompatible(ctx, *leftItem);
                    const bool rightClass = rightItem && isItemClassCompatible(ctx, *rightItem);
                    if (leftClass != rightClass)
                        return leftClass > rightClass;
                    if (ctx.player.inventory[leftIndex].equipped != ctx.player.inventory[rightIndex].equipped)
                        return ctx.player.inventory[leftIndex].equipped > ctx.player.inventory[rightIndex].equipped;
                    return leftIndex < rightIndex; });

                const int totalItems = static_cast<int>(inventoryIndexes.size());
                const int totalPages = std::max(1, (totalItems + PAGE_SIZE - 1) / PAGE_SIZE);
                page = clampInt(page, 1, totalPages);
                const int start = (page - 1) * PAGE_SIZE;
                const int end = std::min(totalItems, start + PAGE_SIZE);

                printStateHeader(ctx, "SHOP SELL");
                std::cout << colorText("Category", Color::Cyan, true) << ": " << selectedCategory
                          << " | Page: " << page << "/" << totalPages;
                std::cout << "\n\n";

                if (totalItems == 0)
                    std::cout << "(Tidak ada item pada kategori ini)\n";

                for (int i = start; i < end; ++i)
                {
                    const std::size_t inventoryIndex = inventoryIndexes[static_cast<std::size_t>(i)];
                    const auto &entry = ctx.player.inventory[inventoryIndex];
                    const auto *item = getItemById(ctx, entry.item_id);
                    if (!item)
                        continue;

                    std::cout << colorText(std::to_string(i - start + 1) + ". " + item->value("name", entry.item_id), Color::White, true)
                              << " x" << entry.quantity
                              << " | Sell " << item->value("sell_price", 0)
                              << " | Tier " << item->value("tier_level", 0);
                    if (entry.equipped)
                        std::cout << colorText(" [Equipped:" + entry.slot + "]", Color::Green, true);
                    printClassAvailabilityTag(ctx, *item);
                    std::cout << '\n';
                }

                std::cout << "\nInput nomor item, q prev, e next, c kembali";
                if (totalPages > 1)
                    std::cout << ", atau nomor page";
                std::cout << ": ";

                const std::string sellInput = toLower(readLine());
                if (sellInput == "c")
                    break;
                if (sellInput == "q")
                {
                    page = std::max(1, page - 1);
                    continue;
                }
                if (sellInput == "e")
                {
                    page = std::min(totalPages, page + 1);
                    continue;
                }

                int selected = 0;
                if (!tryParseInt(sellInput, selected))
                {
                    std::cout << "Input tidak valid.\n";
                    waitForEnter();
                    continue;
                }

                if (selected >= 1 && selected <= (end - start))
                {
                    const std::size_t inventoryIndex = inventoryIndexes[static_cast<std::size_t>(start + selected - 1)];
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

                    if (!removeInventoryEntryQuantity(ctx.player, inventoryIndex, quantity))
                    {
                        std::cout << "Item gagal dijual.\n";
                        waitForEnter();
                        continue;
                    }

                    ctx.player.gold += quantity * item->value("sell_price", 0);
                    refreshPlayerResources(ctx);
                    std::cout << "Item berhasil dijual.\n";
                    saveGame(ctx);
                    waitForEnter();
                    continue;
                }

                if (selected >= 1 && selected <= totalPages)
                {
                    page = selected;
                    continue;
                }

                std::cout << "Nomor tidak valid.\n";
                waitForEnter();
            }
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
