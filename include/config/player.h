#pragma once

namespace Config
{
    namespace Player
    {
        inline constexpr int BASE_HP_START = 200;                    // HP dasar sebelum bonus level/stat.
        inline constexpr int BASE_HP_PER_LEVEL = 70;                 // Tambahan HP setiap level.
        inline constexpr int BASE_HP_PER_VIT = 12;                   // Tambahan HP setiap 1 VIT.
        inline constexpr int BASE_HP_PER_STR = 4;                    // Tambahan HP setiap 1 STR.
        inline constexpr int BASE_MP_START = 45;                     // MP dasar sebelum bonus level/stat.
        inline constexpr int BASE_MP_PER_LEVEL = 3;                  // Tambahan MP setiap level.
        inline constexpr int BASE_MP_PER_INT = 4;                    // Tambahan MP setiap 1 INT.
        inline constexpr int BASE_MP_AGI_DIVISOR = 2;                // Pembagi AGI sebelum memberi bonus MP.
        inline constexpr int BASE_MP_PER_TWO_AGI = 1;                // Tambahan MP setiap AGI yang lolos pembagi.
        inline constexpr int ATTACK_BASE_PER_LEVEL = 6;              // Kontribusi level ke ATK.
        inline constexpr int ATTACK_PRIMARY_DIVISOR = 2;             // Pembagi primary stat untuk ATK.
        inline constexpr int ATTACK_STR_DIVISOR = 2;                 // Pembagi STR untuk ATK.
        inline constexpr int ATTACK_AGI_DIVISOR = 4;                 // Pembagi AGI untuk ATK.
        inline constexpr double ATTACK_EQUIPMENT_MULTIPLIER = 1.35;  // Pengali bonus ATK dari equipment.
        inline constexpr double ATTACK_BUFF_MULTIPLIER = 1.65;       // Pengali ATK saat buff aktif.
        inline constexpr int ATTACK_JITTER_MIN = -1;                 // Variasi damage minimum serangan normal.
        inline constexpr int ATTACK_JITTER_MAX = 5;                  // Variasi damage maksimum serangan normal.
        inline constexpr int DEFENSE_BASE_PER_LEVEL = 1;             // Kontribusi level ke DEF.
        inline constexpr int DEFENSE_VIT_DIVISOR = 6;                // Pembagi VIT untuk DEF.
        inline constexpr int DEFENSE_STR_DIVISOR = 12;               // Pembagi STR untuk DEF.
        inline constexpr double DEFENSE_EQUIPMENT_MULTIPLIER = 0.65; // Pengali bonus DEF dari equipment.
        inline constexpr double DEFENSE_BUFF_MULTIPLIER = 1.25;      // Pengali DEF saat buff aktif.
        inline constexpr int SPEED_BASE_PER_LEVEL = 2;                // Kontribusi level ke speed.
        inline constexpr int SPEED_PER_AGI = 2;                      // Tambahan speed setiap 1 AGI.
        inline constexpr double SPEED_BUFF_MULTIPLIER = 1.25;        // Pengali speed saat buff AGI aktif.
        inline constexpr double CRIT_BASE_RATE = 0.12;               // Peluang critical dasar player.
        inline constexpr double CRIT_PER_AGI = 0.006;                // Tambahan peluang critical setiap 1 AGI.
        inline constexpr double CRIT_CAP = 1.0;                      // Batas maksimum peluang critical.
        inline constexpr int PRIMARY_STAT_BONUS_ON_NEW_GAME = 8;     // Bonus primary stat saat karakter dibuat.
        inline constexpr int DEFEAT_RECOVERY_DIVISOR = 2;            // Pembagi max HP/MP untuk recovery setelah kalah.
        inline constexpr int MIN_ATTACK = 1;                         // ATK minimum setelah kalkulasi.
        inline constexpr int MIN_DEFENSE = 0;                        // DEF minimum setelah kalkulasi.
        inline constexpr int MAX_EFFECTIVE_DEFENSE_FOR_DAMAGE = 500;  // Batas DEF efektif agar damage tidak selalu jatuh ke minimum.
        inline constexpr int DEFENSE_REDUCED_DAMAGE_FLOOR_DENOMINATOR = 2; // Pembagi sisa DEF di atas cap agar scaling tetap terkendali.

        // Kap untuk damage yang diterima pemain dari serangan musuh.
        // Masalah yang dilaporkan: saat DEF target (player) terlalu tinggi, rumus damage enemy - player menjadi turun ke nilai floor (1).
        // Dengan memotong DEF efektif, damage tidak “menghancur” jadi 1.
        inline constexpr int MAX_EFFECTIVE_PLAYER_DEFENSE_FOR_ENEMY_DAMAGE = MAX_EFFECTIVE_DEFENSE_FOR_DAMAGE;
        inline constexpr int DEFENSE_REDUCED_PLAYER_DAMAGE_FLOOR_DENOMINATOR = DEFENSE_REDUCED_DAMAGE_FLOOR_DENOMINATOR;







        inline constexpr int MIN_SPEED = 1;                          // Speed minimum setelah kalkulasi.
        inline constexpr int MIN_DAMAGE = 1;                         // Damage minimum saat serangan berhasil mengenai target.
        inline constexpr int RESOURCE_FLOOR = 0;                     // Nilai bawah HP/MP saat dikurangi.
        inline constexpr int MIN_RECOVERY_RESOURCE = 1;              // HP/MP minimum setelah recovery kekalahan.
        inline constexpr double LEVEL_UP_EARLY_MULTIPLIER = 0.95;    // Pengali EXP level awal.
        inline constexpr int LEVEL_UP_MID_START_LEVEL = 6;           // Level mulai memakai pengali mid.
        inline constexpr double LEVEL_UP_MID_MULTIPLIER = 0.80;      // Pengali EXP level mid.
        inline constexpr int LEVEL_UP_LATE_START_LEVEL = 11;         // Level mulai memakai pengali late.
        inline constexpr double LEVEL_UP_LATE_MULTIPLIER = 0.68;     // Pengali EXP level late.
        inline constexpr int LEVEL_UP_ENDGAME_START_LEVEL = 21;      // Level mulai memakai pengali endgame.
        inline constexpr double LEVEL_UP_ENDGAME_MULTIPLIER = 0.58;  // Pengali EXP level endgame.
        inline constexpr int EXP_FALLBACK_LINEAR_MULTIPLIER = 100;   // Fallback EXP linear jika tabel balance hilang.
        inline constexpr int EXP_FALLBACK_QUADRATIC_MULTIPLIER = 22; // Fallback EXP kuadrat jika level tidak ada di tabel.
        inline constexpr int MINIMUM_EXP_REQUIREMENT = 80;           // EXP minimum untuk naik level.
    }
}
