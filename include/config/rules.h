#pragma once

namespace Config
{
    namespace Rules
    {
        inline constexpr int STARTING_GOLD = 1200;             // Gold awal saat membuat karakter baru.
        inline constexpr int STARTING_HP = 250;                // HP awal fallback saat save tidak punya data HP.
        inline constexpr int STARTING_MP = 50;                 // MP awal fallback saat save tidak punya data MP.
        inline constexpr int STARTING_STAT_POINTS = 15;        // Stat point awal saat membuat karakter baru.
        inline constexpr int STAT_POINTS_PER_LEVEL = 10;       // Stat point yang didapat setiap naik level.
        inline constexpr int DEFAULT_PAGE_SIZE = 5;            // Jumlah item yang tampil per halaman list.
        inline constexpr int MOVE_STEPS_PER_DEPTH = 5;         // Jumlah langkah eksplorasi lama per depth.
        inline constexpr int MIN_ENCOUNTERS_PER_MOVE = 1;      // Encounter minimum dalam satu gerakan eksplorasi lama.
        inline constexpr int MAX_ENCOUNTERS_PER_MOVE = 3;      // Encounter maksimum dalam satu gerakan eksplorasi lama.
        inline constexpr const char *MENU_SEPARATOR = "-------------------------------------------------------"; // Garis pemisah menu.
        inline constexpr const char *TITLE_SEPARATOR = "========================================";                 // Garis pemisah judul.
    }
}
