#pragma once

namespace Config
{
    namespace DungeonMap
    {
        inline constexpr int ROOT_DEPTH = 0;                         // Depth node awal peta eksplorasi.
        inline constexpr int MAX_BRANCH_DEPTH = 14;                  // Depth terakhir sebelum node final.
        inline constexpr int FINAL_NODE_DEPTH = 15;                  // Depth node final peta eksplorasi.
        inline constexpr int LATE_BRANCH_START_DEPTH = 9;            // Mulai depth yang cenderung menggabungkan jalur.
        inline constexpr int MID_SPLIT_DEPTH = 8;                    // Depth khusus untuk peluang split jalur.
        inline constexpr int EARLY_SKIP_MAX_DEPTH = 4;               // Depth maksimum yang boleh membuat skip branch.
        inline constexpr int NORMAL_STEP = 1;                        // Tambahan depth normal antar node.
        inline constexpr int SKIP_STEP = 2;                          // Tambahan depth untuk jalur skip.
        inline constexpr int SKIP_ROLL_MIN = 1;                      // Nilai minimum roll skip branch.
        inline constexpr int SKIP_ROLL_MAX = 4;                      // Nilai maksimum roll skip branch.
        inline constexpr int SKIP_ROLL_SUCCESS = 1;                  // Nilai roll yang membuat skip branch aktif.
        inline constexpr int LATE_SHARE_NODE_CHANCE = 40;            // Peluang persen dua jalur late memakai node yang sama.
        inline constexpr int MID_SPLIT_CHANCE = 50;                  // Peluang persen depth mid membuat dua jalur.
        inline constexpr int VISIBLE_ENCOUNTER_THRESHOLD = 30;       // Batas roll kumulatif untuk encounter terlihat.
        inline constexpr int HIDDEN_ENCOUNTER_THRESHOLD = 35;        // Batas roll kumulatif untuk encounter tersembunyi.
        inline constexpr int CAMPFIRE_THRESHOLD = 40;                // Batas roll kumulatif untuk campfire.
        inline constexpr int TRAP_THRESHOLD = 50;                    // Batas roll kumulatif untuk trap.
        inline constexpr int LEAF_PAIR_SIZE = 2;                     // Ukuran pasangan leaf untuk jaminan campfire.
        inline constexpr int ODD_LEAF_REMAINDER = 1;                 // Sisa leaf ganjil saat dihitung modulo pair.
        inline constexpr int CAMPFIRE_HP_BASE_HEAL = 30;             // Heal HP dasar saat menemukan campfire.
        inline constexpr int CAMPFIRE_MP_BASE_HEAL = 20;             // Heal MP dasar saat menemukan campfire.
        inline constexpr double CAMPFIRE_HP_DEPTH_RATIO = 0.01;      // Bonus heal HP campfire per depth terhadap max HP.
        inline constexpr double CAMPFIRE_MP_DEPTH_RATIO = 0.01;      // Bonus heal MP campfire per depth terhadap max MP.
        inline constexpr double TRAP_BASE_DAMAGE_RATIO = 0.05;       // Damage dasar trap terhadap max HP.
        inline constexpr int TRAP_RANDOM_BONUS_MIN = 1;              // Bonus damage trap minimum dalam persen.
        inline constexpr int TRAP_RANDOM_BONUS_MAX = 10;             // Bonus damage trap maksimum dalam persen.
        inline constexpr double TRAP_PERCENT_DIVISOR = 100.0;        // Pembagi persen trap agar menjadi rasio pecahan.
    }
}
