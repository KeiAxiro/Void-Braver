#include "states/dungeon_features.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/math_helpers.h"
#include "states/progress_helpers.h"
#include "config/enemy_balance.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

namespace state_helpers
{
    bool isDepthLevelAllowed(const json &depthRow, int level)
    {
        if (!depthRow.contains("level_range") || !depthRow["level_range"].is_object())
            return true;

        const int minLevel = depthRow["level_range"].value("min", 1);
        const int maxLevel = depthRow["level_range"].value("max", 999);
        return level >= minLevel && level <= maxLevel;
    }

    EnemyInstance spawnEnemyForDepth(const GameContext &ctx, const json &dungeon, int depth)
    {
        vector<const json *> pool;
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
            fallback.name = wantsBoss ? dungeon.value("boss_name", string("Apex Slime")) : "Training Slime";
            fallback.type = "Slime";
            fallback.level = depth;
            fallback.max_hp = fallback.hp = enemy_balance::kFallbackHpBase + depth * enemy_balance::kFallbackHpPerDepth;
            fallback.max_mp = fallback.mp = enemy_balance::kFallbackMpBase + depth * enemy_balance::kFallbackMpPerDepth;
            fallback.atk = enemy_balance::kFallbackAtkBase + depth * enemy_balance::kFallbackAtkPerDepth;
            fallback.def = enemy_balance::kFallbackDefBase + depth * enemy_balance::kFallbackDefPerDepth;
            fallback.crit_rate = 0.05;
            fallback.is_boss = wantsBoss;
            fallback.exp_drop = enemy_balance::kFallbackExpBase + depth * enemy_balance::kFallbackExpPerDepth;
            return fallback;
        }

        const json &templateEnemy = *pool[static_cast<size_t>(randInt(0, static_cast<int>(pool.size()) - 1))];
        EnemyInstance enemy;
        enemy.id = templateEnemy.value("id", string("enemy"));
        enemy.name = templateEnemy.value("name", string("Enemy"));
        enemy.type = templateEnemy.value("type", string("Unknown"));
        enemy.is_boss = wantsBoss || templateEnemy.value("is_boss", false);

        const int enemyLevelMin = templateEnemy["level_range"].value("min", 1);
        const int enemyLevelMax = templateEnemy["level_range"].value("max", enemyLevelMin);

        int levelMin = enemyLevelMin;
        int levelMax = enemyLevelMax;

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
            enemy.max_hp = static_cast<int>(enemy.max_hp * enemy_balance::kBossHpMultiplier);
            enemy.atk = static_cast<int>(enemy.atk * enemy_balance::kBossAtkMultiplier);
            enemy.def = static_cast<int>(enemy.def * enemy_balance::kBossDefMultiplier);
            enemy.crit_rate = max(templateEnemy.value("crit_rate", 0.05), enemy_balance::kBossMinimumCritRate);
        }
        else
        {
            enemy.crit_rate = templateEnemy.value("crit_rate", 0.05);
        }

        enemy.hp = enemy.max_hp;
        enemy.mp = enemy.max_mp;
        enemy.exp_drop = enemy.level * enemy_balance::kExpDropPerEnemyLevel +
                         depth * enemy_balance::kExpDropPerDepth +
                         (enemy.is_boss ? enemy_balance::kBossExpBonus : 0);

        if (templateEnemy.contains("drops") && templateEnemy["drops"].is_array())
        {
            for (const auto &drop : templateEnemy["drops"])
                enemy.drops.push_back({drop.value("item_id", string()), drop.value("drop_rate", 100)});
        }

        return enemy;
    }
} // namespace state_helpers
