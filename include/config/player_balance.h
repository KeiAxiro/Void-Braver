#pragma once

// Semua angka inti player diletakkan di sini
// supaya balancing lebih gampang dan lebih manusiawi dibaca.
namespace player_balance
{
    inline constexpr int kBaseHpStart = 65;
    inline constexpr int kBaseHpPerLevel = 7;
    inline constexpr int kBaseHpPerVit = 4;
    inline constexpr int kBaseHpPerStr = 1;

    inline constexpr int kBaseMpStart = 40;
    inline constexpr int kBaseMpPerLevel = 4;
    inline constexpr int kBaseMpPerInt = 3;
    inline constexpr int kBaseMpPerTwoAgi = 1;

    inline constexpr int kAttackBasePerLevel = 1;
    inline constexpr int kAttackPrimaryDivisor = 3;
    inline constexpr int kAttackStrDivisor = 5;
    inline constexpr int kAttackAgiDivisor = 8;
    inline constexpr double kAttackEquipmentMultiplier = 0.75;
    inline constexpr double kAttackBuffMultiplier = 1.30;
    inline constexpr int kAttackJitterMin = -2;
    inline constexpr int kAttackJitterMax = 4;

    inline constexpr int kDefenseBasePerLevel = 1;
    inline constexpr int kDefenseVitDivisor = 4;
    inline constexpr int kDefenseStrDivisor = 12;
    inline constexpr double kDefenseEquipmentMultiplier = 0.55;
    inline constexpr double kDefenseBuffMultiplier = 1.30;

    inline constexpr int kSpeedBasePerLevel = 1;
    inline constexpr int kSpeedPerAgi = 2;
    inline constexpr double kSpeedBuffMultiplier = 1.25;

    inline constexpr double kCritBaseRate = 0.05;
    inline constexpr double kCritPerAgi = 0.002;
    inline constexpr int kPrimaryStatBonusOnNewGame = 5;
    inline constexpr int kDefeatRecoveryDivisor = 2;

    inline constexpr double kLevelUpEarlyMultiplier = 0.95;
    inline constexpr double kLevelUpMidMultiplier = 0.80;
    inline constexpr double kLevelUpLateMultiplier = 0.68;
    inline constexpr double kLevelUpEndgameMultiplier = 0.58;
    inline constexpr int kMinimumExpRequirement = 80;
} // namespace player_balance
