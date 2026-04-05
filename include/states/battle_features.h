#pragma once

#include <vector>

#include "battle_types.h"

namespace state_helpers
{
    int calculateEnemySpeed(const EnemyInstance &enemy);
    bool enemyGetsFirstTurn(const GameContext &ctx, const EnemyInstance &enemy, bool ambush);
    std::vector<int> buildEncounterStepsForMove();
    EncounterResult runEncounterBattle(GameContext &ctx, const json &dungeon, const json &depthData, EnemyInstance enemy, bool enemyStartsFirst);
    void grantEnemyDrops(GameContext &ctx, const EnemyInstance &enemy);
    void advanceDungeonProgress(GameContext &ctx, const json &dungeon, int clearedDepth);
    int calculateEnemyDamage(const EnemyInstance &enemy, const BattleState &battle);
    int calculateEnemyDefense(const EnemyInstance &enemy, const BattleState &battle);
    void tickBattleEffects(GameContext &ctx, EnemyInstance &enemy, BattleState &battle);
    bool useBattleConsumable(GameContext &ctx);
    void resolveSkillUse(GameContext &ctx, EnemyInstance &enemy, BattleState &battle, const json &skill, bool &turnConsumed);
    void showBattleSkills(const GameContext &ctx);
    void applyBattleRewards(GameContext &ctx, const json &dungeon, const json &depthRow, const EnemyInstance &enemy);
    void applyDepthCompletionRewards(GameContext &ctx, const json &dungeon, const json &depthRow);
} // namespace state_helpers
