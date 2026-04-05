#pragma once

// Aturan umum game yang sering dipakai lintas fitur.
// Kalau ingin mengubah flow besar game, mulai cek file ini dulu.
namespace game_rules
{
    inline constexpr int kStartingGold = 650;
    inline constexpr int kStartingHp = 100;
    inline constexpr int kStartingMp = 50;
    inline constexpr int kStartingStatPoints = 5;
    inline constexpr int kStatPointsPerLevel = 5;

    inline constexpr int kDefaultPageSize = 5;
    inline constexpr int kMoveStepsPerDepth = 5;
    inline constexpr int kMinEncountersPerMove = 1;
    inline constexpr int kMaxEncountersPerMove = 3;

    inline constexpr const char *kMenuSeparator = "-------------------------------------------------------";
    inline constexpr const char *kTitleSeparator = "========================================";
} // namespace game_rules
