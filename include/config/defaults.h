#pragma once

#include "rules.h"

namespace Config
{
    namespace Defaults
    {
        inline constexpr const char *PLAYER_NAME = "Hero";       // Nama fallback jika nama karakter kosong.
        inline constexpr const char *PLAYER_ID = "hero";         // ID fallback jika nama tidak punya karakter valid.
        inline constexpr const char *CLASS_ID = "knight";        // Class fallback jika class kosong atau tidak valid.
        inline constexpr const char *PRIMARY_STAT = "STR";       // Primary stat fallback saat class tidak ditemukan.
        inline constexpr int BOOL_TRUE_AS_INT = 1;               // Nilai integer saat boolean true dikonversi ke angka.
        inline constexpr int BOOL_FALSE_AS_INT = 0;              // Nilai integer saat boolean false dikonversi ke angka.
        inline constexpr int PLAYER_LEVEL = 1;                   // Level default karakter.
        inline constexpr int PLAYER_EXP = 0;                     // EXP default karakter.
        inline constexpr int PLAYER_GOLD = Rules::STARTING_GOLD; // Gold default karakter mengikuti starting gold.
        inline constexpr int PLAYER_STAT_STR = 10;               // STR default karakter.
        inline constexpr int PLAYER_STAT_INT = 10;               // INT default karakter.
        inline constexpr int PLAYER_STAT_AGI = 10;               // AGI default karakter.
        inline constexpr int PLAYER_STAT_VIT = 10;               // VIT default karakter.
        inline constexpr int PLAYER_HP = Rules::STARTING_HP;     // HP default karakter.
        inline constexpr int PLAYER_MP = Rules::STARTING_MP;     // MP default karakter.
        inline constexpr int PLAYER_STAT_POINTS = 0;             // Stat point default untuk data kosong.
        inline constexpr int INVENTORY_QUANTITY = 1;             // Jumlah default satu baris inventory.
        inline constexpr int COOLDOWN_TURNS = 0;                 // Cooldown default skill.
        inline constexpr int ACTIVE_CHARACTER_NONE = -1;         // Penanda tidak ada karakter aktif.
        inline constexpr int ACTIVE_CHARACTER_FIRST_INDEX = 0;   // Index karakter pertama setelah load save.
        inline constexpr int DUPLICATE_ID_SUFFIX_START = 1;      // Suffix awal saat membuat ID karakter duplikat.
        inline constexpr int SAVE_JSON_INDENT = 2;               // Jumlah spasi indent JSON save.
    }
}
