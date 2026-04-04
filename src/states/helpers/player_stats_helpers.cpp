#include "state_helpers.h"
#include "database.h"

#include <algorithm>
#include <iostream>

using namespace std;

namespace state_helpers
{
    int calculatePlayerAttack(const GameContext &ctx, const BattleState *battle)
    {
        const string primary = classPrimaryStat(ctx, ctx.player.class_id);
        const int primaryValue = getPrimaryStatValue(ctx.player, primary);

        int attack = ctx.player.level;
        attack += primaryValue / 3;
        attack += ctx.player.stats.str / 5;
        attack += ctx.player.stats.agi / 8;
        attack += static_cast<int>(itemBonusFromEquipped(ctx, ctx.player, "atk") * 0.75);

        if (battle && battle->playerAtkBuffTurns > 0)
            attack = static_cast<int>(attack * 1.30);
        return max(1, attack);
    }

    int calculatePlayerDefense(const GameContext &ctx, const BattleState *battle)
    {
        int defense = ctx.player.level;
        defense += ctx.player.stats.vit / 4;
        defense += ctx.player.stats.str / 12;
        defense += static_cast<int>(itemBonusFromEquipped(ctx, ctx.player, "def") * 0.55);

        if (battle && battle->playerDefBuffTurns > 0)
            defense = static_cast<int>(defense * 1.30);
        return max(0, defense);
    }

    int calculatePlayerSpeed(const GameContext &ctx, const BattleState *battle)
    {
        int speed = ctx.player.level + ctx.player.stats.agi * 2;
        if (battle && battle->playerAgiBuffTurns > 0)
            speed = static_cast<int>(speed * 1.25);
        return max(1, speed);
    }

    double calculatePlayerCritRate(const GameContext &ctx, const BattleState *battle)
    {
        double rate = 0.05 + (ctx.player.stats.agi * 0.002);
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
