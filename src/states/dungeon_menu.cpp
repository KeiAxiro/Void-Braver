#include "states.h"
#include "database.h"
#include "state_helpers.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    /// @brief
    /// @param ctx
    /// @param dungeon
    /// @param depth
    /// @return
    EnemyInstance spawnEnemyForDepth(const GameContext &ctx, const json &dungeon, int depth)
    {
        std::vector<const json *> pool;
        const bool wantsBoss = depth == dungeonMaxDepth(dungeon);

        if (ctx.gameData.contains("enemies") && ctx.gameData["enemies"].is_array())
        {
            for (const auto &enemy : ctx.gameData["enemies"])
            {
                if (!enemy.contains("spawn_depth") || !enemy["spawn_depth"].is_object())
                    continue;

                const int start = enemy["spawn_depth"].value("start", 1);
                const int end = enemy["spawn_depth"].value("end", 1);
                const bool matchesDepth = depth >= start && depth <= end;
                const bool isBoss = enemy.value("is_boss", false);
                if (matchesDepth && isBoss == wantsBoss)
                    pool.push_back(&enemy);
            }

            if (pool.empty())
            {
                for (const auto &enemy : ctx.gameData["enemies"])
                {
                    if (!enemy.contains("spawn_depth") || !enemy["spawn_depth"].is_object())
                        continue;
                    const int start = enemy["spawn_depth"].value("start", 1);
                    const int end = enemy["spawn_depth"].value("end", 1);
                    if (depth >= start && depth <= end)
                        pool.push_back(&enemy);
                }
            }
        }

        if (pool.empty())
        {
            EnemyInstance fallback;
            fallback.id = "training_slime";
            fallback.name = wantsBoss ? dungeon.value("boss_name", std::string("Apex Slime")) : "Training Slime";
            fallback.type = "Slime";
            fallback.level = depth;
            fallback.max_hp = fallback.hp = 40 + depth * 20;
            fallback.max_mp = fallback.mp = 20 + depth * 5;
            fallback.atk = 10 + depth * 5;
            fallback.def = 5 + depth * 3;
            fallback.crit_rate = 0.05;
            fallback.is_boss = wantsBoss;
            fallback.exp_drop = 40 + depth * 30;
            return fallback;
        }

        const json &templateEnemy = *pool[static_cast<std::size_t>(randInt(0, static_cast<int>(pool.size()) - 1))];
        EnemyInstance enemy;
        enemy.id = templateEnemy.value("id", std::string("enemy"));
        enemy.name = templateEnemy.value("name", std::string("Enemy"));
        enemy.type = templateEnemy.value("type", std::string("Unknown"));
        enemy.is_boss = wantsBoss || templateEnemy.value("is_boss", false);

        const int enemyLevelMin = templateEnemy["level_range"].value("min", 1);
        const int enemyLevelMax = templateEnemy["level_range"].value("max", enemyLevelMin);

        int levelMin = enemyLevelMin;
        int levelMax = enemyLevelMax;

        // Level musuh ikut dikunci oleh rekomendasi depth supaya depth 1
        // tidak diam-diam memunculkan varian level 10 dari enemy yang sama.
        const json *depthData = findDungeonDepth(dungeon, depth);
        if (depthData != nullptr && depthData->contains("level_range"))
        {
            levelMin = max(levelMin, (*depthData)["level_range"].value("min", levelMin));
            levelMax = min(levelMax, (*depthData)["level_range"].value("max", levelMax));
            if (levelMin > levelMax)
                levelMax = levelMin;
        }

        enemy.level = randInt(levelMin, levelMax);

        enemy.max_hp = interpolateStat(enemy.level, levelMin, levelMax,
                                       templateEnemy["stats_range"]["hp"].value("min", 10),
                                       templateEnemy["stats_range"]["hp"].value("max", 20));
        enemy.max_mp = interpolateStat(enemy.level, levelMin, levelMax,
                                       templateEnemy["stats_range"]["mp"].value("min", 0),
                                       templateEnemy["stats_range"]["mp"].value("max", 0));
        enemy.atk = interpolateStat(enemy.level, levelMin, levelMax,
                                    templateEnemy["stats_range"]["atk"].value("min", 1),
                                    templateEnemy["stats_range"]["atk"].value("max", 2));
        enemy.def = interpolateStat(enemy.level, levelMin, levelMax,
                                    templateEnemy["stats_range"]["def"].value("min", 0),
                                    templateEnemy["stats_range"]["def"].value("max", 1));

        if (wantsBoss)
        {
            enemy.name = dungeon.value("boss_name", enemy.name);
            enemy.max_hp = static_cast<int>(enemy.max_hp * 1.35);
            enemy.atk = static_cast<int>(enemy.atk * 1.20);
            enemy.def = static_cast<int>(enemy.def * 1.20);
            enemy.crit_rate = std::max(templateEnemy.value("crit_rate", 0.05), 0.10);
        }
        else
        {
            enemy.crit_rate = templateEnemy.value("crit_rate", 0.05);
        }

        enemy.hp = enemy.max_hp;
        enemy.mp = enemy.max_mp;
        enemy.exp_drop = enemy.level * 24 + depth * 18 + (enemy.is_boss ? 220 : 0);

        if (templateEnemy.contains("drops") && templateEnemy["drops"].is_array())
        {
            for (const auto &drop : templateEnemy["drops"])
                enemy.drops.push_back({drop.value("item_id", std::string()), drop.value("drop_rate", 100)});
        }

        return enemy;
    }

    bool isDepthLevelAllowed(const json &depthRow, int level)
    {
        if (!depthRow.contains("level_range") || !depthRow["level_range"].is_object())
            return true;
        const int minLevel = depthRow["level_range"].value("min", 1);
        const int maxLevel = depthRow["level_range"].value("max", 999);
        return level >= minLevel && level <= maxLevel;
    }

    void printDungeonLore(const json &depthRow)
    {
        if (!depthRow.contains("tale") || !depthRow["tale"].is_object())
            return;
        const auto &tale = depthRow["tale"];
        std::cout << colorText("Chapter", Color::Magenta, true) << ": " << tale.value("chapter_name", std::string("-")) << '\n';
        std::cout << colorText("Title", Color::Cyan, true) << ": " << tale.value("depth_title", std::string("-")) << '\n';
        std::cout << colorText("Lore", Color::Yellow, true) << ": " << tale.value("lore_content", std::string("-")) << '\n';
    }

    bool selectDungeonDepth(GameContext &ctx, bool enterBattleState)
    {
        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "ENTER DUNGEON");
            int index = 1;
            for (const auto &dungeon : ctx.gameData["dungeons"])
            {
                const bool locked = isFinalDungeonLocked(ctx, dungeon);
                const int unlockedDepth = getDungeonUnlockedDepth(ctx.player, dungeon);
                const int maxDepth = dungeonMaxDepth(dungeon);
                std::cout << colorText(to_string(index++) + ". " + dungeon.value("name", std::string()), Color::Yellow, true) << '\n';
                std::cout << "   Theme : " << dungeon.value("theme", std::string("-")) << '\n';
                std::cout << "   Boss  : " << dungeon.value("boss_name", std::string("-"))
                          << " | Orb: " << dungeon.value("reward_orb", std::string("-")) << '\n';
                std::cout << "   Depth : " << unlockedDepth << "/" << maxDepth;
                if (isDungeonCompleted(ctx.player, dungeon))
                    std::cout << " [Completed]";
                if (locked)
                    std::cout << " [Locked]";
                std::cout << "\n\n";
            }
            std::cout << "Pilih dungeon dengan nomor, atau c untuk kembali: ";

            const std::string input = toLower(readLine());
            if (input == "c")
                return false;

            int selected = 0;
            if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(ctx.gameData["dungeons"].size()))
            {
                std::cout << "Pilihan dungeon tidak valid.\n";
                continue;
            }

            const json &dungeon = ctx.gameData["dungeons"][static_cast<std::size_t>(selected - 1)];
            if (isFinalDungeonLocked(ctx, dungeon))
            {
                std::cout << "Tahta Kehampaan masih terkunci. Selesaikan dungeon lain dulu.\n";
                waitForEnter();
                continue;
            }

            const int unlockedDepth = getDungeonUnlockedDepth(ctx.player, dungeon);
            const int maxDepth = dungeonMaxDepth(dungeon);

            clearScreen();
            printStateHeader(ctx, dungeon.value("name", std::string()));
            std::cout << "Pilih depth 1 sampai " << unlockedDepth << ".\n\n";
            for (int depth = 1; depth <= unlockedDepth && depth <= maxDepth; ++depth)
            {
                const json *depthRow = findDungeonDepth(dungeon, depth);
                if (!depthRow)
                    continue;
                const int minLevel = (*depthRow)["level_range"].value("min", 1);
                const int maxLevelRow = (*depthRow)["level_range"].value("max", 999);
                std::cout << colorText(to_string(depth) + ". Depth " + to_string(depth), Color::Cyan, true)
                          << " | Recommended Lv " << minLevel << "-" << maxLevelRow;
                if (depth == maxDepth)
                    std::cout << " [Boss]";
                std::cout << '\n';
                printDungeonLore(*depthRow);
                std::cout << '\n';
            }

            std::cout << "Depth pilihan, atau c untuk cancel: ";
            const std::string depthInput = toLower(readLine());
            if (depthInput == "c")
                continue;

            int depthChoice = 0;
            if (!tryParseInt(depthInput, depthChoice) || depthChoice < 1 || depthChoice > unlockedDepth || depthChoice > maxDepth)
            {
                std::cout << "Depth tidak valid.\n";
                continue;
            }

            const json *depthRow = findDungeonDepth(dungeon, depthChoice);
            if (!depthRow)
            {
                std::cout << "Data depth tidak ditemukan.\n";
                waitForEnter();
                continue;
            }
            if (!isDepthLevelAllowed(*depthRow, ctx.player.level))
            {
                std::cout << "Level character belum sesuai dengan range depth ini.\n";
                waitForEnter();
                continue;
            }

            ctx.player.progress.current_dungeon = dungeon.value("id", std::string());
            ctx.player.progress.current_depth = depthChoice;
            ctx.player.progress.max_depth_unlocked = unlockedDepth;
            if (enterBattleState)
                ctx.stateStack.push_back(GameState::Battle);
            return true;
        }
    }

    void chooseDungeon(GameContext &ctx)
    {
        selectDungeonDepth(ctx, true);
    }

    bool canCraftItem(const GameContext &ctx, const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].is_object() || !item["crafting"].contains("ingredients"))
            return false;

        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const std::string itemId = ingredient.value("item_id", std::string());
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
            const std::string itemId = ingredient.value("item_id", std::string());
            owned += std::min(inventoryCount(ctx.player, itemId), ingredient.value("quantity", 0));
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

    std::vector<std::string> collectCraftingCategories(const GameContext &ctx)
    {
        std::vector<std::string> categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const std::string categoryName = category.get<std::string>();
            bool exists = false;
            for (const auto &item : ctx.gameData["items"])
            {
                if (item.value("category", std::string()) == categoryName &&
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

    std::vector<std::string> categoriesForShopMode(const GameContext &ctx, const std::string &mode)
    {
        std::vector<std::string> categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const std::string categoryName = category.get<std::string>();
            bool exists = false;
            for (const auto &item : ctx.gameData["items"])
            {
                if (item.value("category", std::string()) != categoryName)
                    continue;
                if (mode == "buy" && item.value("buy_price", 0) > 0)
                {
                    exists = true;
                    break;
                }
                if (mode == "sell" && item.value("sell_price", 0) > 0 && inventoryCount(ctx.player, item.value("id", std::string())) > 0)
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
        std::cout << MENU_SEPARATOR << '\n';
        std::cout << item.value("name", std::string()) << '\n';
        std::cout << "Category: " << item.value("category", std::string()) << '\n';
        std::cout << "Tier: " << item.value("tier_level", 0) << '\n';
        std::cout << "Required Class: " << item.value("required_class_id", std::string("-")) << '\n';
        std::cout << "Stats -> ATK +" << item["stats"].value("atk_bonus", 0)
                  << ", DEF +" << item["stats"].value("def_bonus", 0)
                  << ", HP +" << item["stats"].value("hp_bonus", 0)
                  << ", MP +" << item["stats"].value("mp_bonus", 0) << '\n';
        std::cout << "Ingredients:\n";
        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const std::string ingredientId = ingredient.value("item_id", std::string());
            const int need = ingredient.value("quantity", 0);
            const int own = inventoryCount(ctx.player, ingredientId);
            std::cout << " - " << itemDisplayName(ctx, ingredientId) << " : " << own << "/" << need << '\n';
        }
        std::cout << MENU_SEPARATOR << '\n';
    }

} // namespace state_helpers
