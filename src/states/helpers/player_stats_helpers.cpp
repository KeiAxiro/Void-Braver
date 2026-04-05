#include "states/player_stats_helpers.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
#include "config/player_balance.h"

#include <algorithm>
#include <iostream>

using namespace std;

namespace state_helpers
{
    int calculatePlayerAttack(const GameContext &ctx, const BattleState *battle)
    {
        const string primary = classPrimaryStat(ctx, ctx.player.class_id);
        const int primaryValue = getPrimaryStatValue(ctx.player, primary);

        int attack = ctx.player.level * player_balance::kAttackBasePerLevel;
        attack += primaryValue / player_balance::kAttackPrimaryDivisor;
        attack += ctx.player.stats.str / player_balance::kAttackStrDivisor;
        attack += ctx.player.stats.agi / player_balance::kAttackAgiDivisor;
        attack += static_cast<int>(itemBonusFromEquipped(ctx, ctx.player, "atk") * player_balance::kAttackEquipmentMultiplier);

        if (battle && battle->playerAtkBuffTurns > 0)
            attack = static_cast<int>(attack * player_balance::kAttackBuffMultiplier);
        return max(1, attack);
    }

    int calculatePlayerDefense(const GameContext &ctx, const BattleState *battle)
    {
        int defense = ctx.player.level * player_balance::kDefenseBasePerLevel;
        defense += ctx.player.stats.vit / player_balance::kDefenseVitDivisor;
        defense += ctx.player.stats.str / player_balance::kDefenseStrDivisor;
        defense += static_cast<int>(itemBonusFromEquipped(ctx, ctx.player, "def") * player_balance::kDefenseEquipmentMultiplier);

        if (battle && battle->playerDefBuffTurns > 0)
            defense = static_cast<int>(defense * player_balance::kDefenseBuffMultiplier);
        return max(0, defense);
    }

    int calculatePlayerSpeed(const GameContext &ctx, const BattleState *battle)
    {
        int speed = ctx.player.level * player_balance::kSpeedBasePerLevel +
                    ctx.player.stats.agi * player_balance::kSpeedPerAgi;
        if (battle && battle->playerAgiBuffTurns > 0)
            speed = static_cast<int>(speed * player_balance::kSpeedBuffMultiplier);
        return max(1, speed);
    }

    double calculatePlayerCritRate(const GameContext &ctx, const BattleState *battle)
    {
        double rate = player_balance::kCritBaseRate + (ctx.player.stats.agi * player_balance::kCritPerAgi);
        if (battle && battle->nextAttackGuaranteedCrit)
            rate = 1.0;
        return min(1.0, rate);
    }

    void refreshPlayerResources(GameContext &ctx)
    {
        normalizePlayerResources(ctx);
    }

    void levelUpIfNeeded(GameContext &ctx)
    {
        bool leveled = false;
        while (ctx.player.current_exp >= expRequiredForNextLevel(ctx, ctx.player.level + 1))
        {
            ++ctx.player.level;
            ctx.player.stat_points += STAT_POINTS_PER_LEVEL;
            leveled = true;
            cout << "Level up! Sekarang level kamu " << ctx.player.level
                 << " dan mendapat " << STAT_POINTS_PER_LEVEL << " stat points.\n";
        }

        if (leveled)
        {
            refreshPlayerResources(ctx);
            ctx.player.hp = ctx.player.max_hp;
            ctx.player.mp = ctx.player.max_mp;
        }
    }
} // namespace state_helpers
