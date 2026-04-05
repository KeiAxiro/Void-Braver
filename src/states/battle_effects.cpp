#include "states/battle_features.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/progress_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"
#include "config/enemy_balance.h"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void grantEnemyDrops(GameContext &ctx, const EnemyInstance &enemy)
    {
        if (enemy.drops.empty())
            return;

        cout << "Drop:\n";
        for (const auto &[itemId, dropRate] : enemy.drops)
        {
            if (randInt(1, 100) <= dropRate)
            {
                addItem(ctx.player, itemId, 1, false, "");
                cout << " - " << itemDisplayName(ctx, itemId) << '\n';
            }
        }
    }

    void advanceDungeonProgress(GameContext &ctx, const json &dungeon, int clearedDepth)
    {
        json &progress = ensureDungeonProgress(ctx.player, dungeon);
        const int maxDepth = dungeonMaxDepth(dungeon);
        progress["highest_cleared_depth"] = max(progress.value("highest_cleared_depth", 0), clearedDepth);

        if (clearedDepth >= maxDepth)
        {
            progress["unlocked_depth"] = maxDepth;
            progress["completed"] = true;
            cout << "Dungeon selesai! Reward orb: " << dungeon.value("reward_orb", std::string("-")) << '\n';
        }
        else
        {
            progress["unlocked_depth"] = max(progress.value("unlocked_depth", 1), clearedDepth + 1);
        }

        ctx.player.progress.current_dungeon = dungeon.value("id", std::string());
        ctx.player.progress.current_depth = min(progress.value("unlocked_depth", 1), maxDepth);
        ctx.player.progress.max_depth_unlocked = progress.value("unlocked_depth", 1);
    }

    int calculateEnemyDamage(const EnemyInstance &enemy, const BattleState &battle)
    {
        int attack = enemy.atk;
        if (!enemy.is_boss)
            attack = static_cast<int>(attack * enemy_balance::kRegularEnemyAttackMultiplier);
        if (battle.enemyAtkDebuffTurns > 0)
            attack = static_cast<int>(attack * enemy_balance::kEnemyAttackDebuffMultiplier);
        if (enemy.is_boss)
            attack = static_cast<int>(attack * enemy_balance::kBossEnemyAttackMultiplier);
        return max(1, attack);
    }

    int calculateEnemyDefense(const EnemyInstance &enemy, const BattleState &battle)
    {
        int defense = enemy.def;
        if (battle.enemyDefDebuffTurns > 0)
            defense = static_cast<int>(defense * enemy_balance::kEnemyDefenseDebuffMultiplier);
        return max(0, defense);
    }

    void tickBattleEffects(GameContext &ctx, EnemyInstance &enemy, BattleState &battle)
    {
        if (battle.enemyDotTurns > 0)
        {
            enemy.hp = max(0, enemy.hp - battle.enemyDotDamage);
            cout << enemy.name << " terkena " << battle.enemyDotLabel
                 << " dan kehilangan " << battle.enemyDotDamage << " HP.\n";
            --battle.enemyDotTurns;
        }

        if (battle.playerAtkBuffTurns > 0)
            --battle.playerAtkBuffTurns;
        if (battle.playerDefBuffTurns > 0)
            --battle.playerDefBuffTurns;
        if (battle.playerAgiBuffTurns > 0)
            --battle.playerAgiBuffTurns;
        if (battle.playerInvincibleTurns > 0)
            --battle.playerInvincibleTurns;
        if (battle.playerEvadeTurns > 0)
            --battle.playerEvadeTurns;
        if (battle.enemyAtkDebuffTurns > 0)
            --battle.enemyAtkDebuffTurns;
        if (battle.enemyDefDebuffTurns > 0)
            --battle.enemyDefDebuffTurns;
        if (battle.enemySilenceTurns > 0)
            --battle.enemySilenceTurns;
        if (battle.enemyStunTurns > 0)
            --battle.enemyStunTurns;

        battle.nextAttackGuaranteedCrit = false;
        reduceCooldowns(ctx.player);
    }

    void applyBattleRewards(GameContext &ctx, const json &dungeon, const json &depthRow, const EnemyInstance &enemy)
    {
        (void)dungeon;
        (void)depthRow;

        const int expReward = enemy.exp_drop;
        const int goldReward = max(enemy_balance::kBattleGoldMinimum,
                                   enemy.level * enemy_balance::kBattleGoldPerEnemyLevel +
                                       enemy.atk / enemy_balance::kBattleGoldAtkDivisor);

        ctx.player.current_exp += expReward;
        ctx.player.gold += goldReward;

        cout << "Reward: +" << expReward << " EXP, +" << goldReward << " Gold\n";

        grantEnemyDrops(ctx, enemy);
        levelUpIfNeeded(ctx);
        saveGame(ctx);
    }

    void applyDepthCompletionRewards(GameContext &ctx, const json &dungeon, const json &depthRow)
    {
        const int baseDepthExp = depthRow.value("exp_gain", 0);
        const int clearExpReward = max(enemy_balance::kDepthClearExpMinimum,
                                       (baseDepthExp * enemy_balance::kDepthClearExpNumerator) /
                                           enemy_balance::kDepthClearExpDenominator);
        const int clearGoldReward = max(enemy_balance::kDepthClearGoldMinimum,
                                        baseDepthExp / enemy_balance::kDepthClearGoldDivisor);

        ctx.player.current_exp += clearExpReward;
        ctx.player.gold += clearGoldReward;

        cout << "\n";
        printLine('=');
        cout << colorText("DEPTH CLEAR", Color::Green, true) << '\n';
        cout << "Bonus clear: +" << clearExpReward << " EXP, +" << clearGoldReward << " Gold\n";

        advanceDungeonProgress(ctx, dungeon, depthRow.value("depth", 1));
        levelUpIfNeeded(ctx);
        saveGame(ctx);
    }
} // namespace state_helpers
