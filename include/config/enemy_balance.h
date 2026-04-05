#pragma once

// Angka inti musuh dan reward yang sering disentuh saat balancing.
namespace enemy_balance
{
    inline constexpr int kFallbackHpBase = 40;
    inline constexpr int kFallbackHpPerDepth = 20;
    inline constexpr int kFallbackMpBase = 20;
    inline constexpr int kFallbackMpPerDepth = 5;
    inline constexpr int kFallbackAtkBase = 10;
    inline constexpr int kFallbackAtkPerDepth = 5;
    inline constexpr int kFallbackDefBase = 5;
    inline constexpr int kFallbackDefPerDepth = 3;
    inline constexpr int kFallbackExpBase = 40;
    inline constexpr int kFallbackExpPerDepth = 30;

    inline constexpr double kRegularEnemyAttackMultiplier = 0.92;
    inline constexpr double kBossEnemyAttackMultiplier = 1.10;
    inline constexpr double kEnemyAttackDebuffMultiplier = 0.85;
    inline constexpr double kEnemyDefenseDebuffMultiplier = 0.70;
    inline constexpr int kEnemySpeedPerLevel = 2;
    inline constexpr int kEnemySpeedAttackDivisor = 6;
    inline constexpr int kEnemyDamageJitterMin = -3;
    inline constexpr int kEnemyDamageJitterMax = 2;
    inline constexpr double kEnemyCriticalDamageMultiplier = 1.50;
    inline constexpr int kEnemyOpeningAmbushSpeedBonus = 8;

    inline constexpr double kBossHpMultiplier = 1.35;
    inline constexpr double kBossAtkMultiplier = 1.20;
    inline constexpr double kBossDefMultiplier = 1.20;
    inline constexpr double kBossMinimumCritRate = 0.10;

    inline constexpr int kExpDropPerEnemyLevel = 24;
    inline constexpr int kExpDropPerDepth = 18;
    inline constexpr int kBossExpBonus = 220;

    inline constexpr int kBattleGoldMinimum = 18;
    inline constexpr int kBattleGoldPerEnemyLevel = 8;
    inline constexpr int kBattleGoldAtkDivisor = 2;

    inline constexpr int kDepthClearGoldMinimum = 14;
    inline constexpr int kDepthClearExpMinimum = 20;
    inline constexpr int kDepthClearExpNumerator = 2;
    inline constexpr int kDepthClearExpDenominator = 5;
    inline constexpr int kDepthClearGoldDivisor = 5;
} // namespace enemy_balance
