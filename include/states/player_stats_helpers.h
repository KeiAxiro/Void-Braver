#pragma once

#include "battle_types.h"

namespace state_helpers
{
    int calculatePlayerAttack(const GameContext &ctx, const BattleState *battle = nullptr);
    int calculatePlayerDefense(const GameContext &ctx, const BattleState *battle = nullptr);
    int calculatePlayerSpeed(const GameContext &ctx, const BattleState *battle = nullptr);
    double calculatePlayerCritRate(const GameContext &ctx, const BattleState *battle = nullptr);
    void refreshPlayerResources(GameContext &ctx);
    void levelUpIfNeeded(GameContext &ctx);
} // namespace state_helpers
