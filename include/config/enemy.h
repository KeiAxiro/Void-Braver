#pragma once

namespace Config
{
    namespace Enemy
    {
        inline constexpr int FALLBACK_HP_BASE = 50;                      // HP dasar musuh fallback.
        inline constexpr int FALLBACK_HP_PER_DEPTH = 15;                 // Tambahan HP fallback per depth.
        inline constexpr int FALLBACK_MP_BASE = 20;                      // MP dasar musuh fallback.
        inline constexpr int FALLBACK_MP_PER_DEPTH = 4;                  // Tambahan MP fallback per depth.
        inline constexpr int FALLBACK_ATK_BASE = 6;                      // ATK dasar musuh fallback.
        inline constexpr int FALLBACK_ATK_PER_DEPTH = 2;                 // Tambahan ATK fallback per depth.
        inline constexpr int FALLBACK_DEF_BASE = 1;                      // DEF dasar musuh fallback.
        inline constexpr int FALLBACK_DEF_PER_DEPTH = 1;                 // Tambahan DEF fallback per depth.
        inline constexpr int FALLBACK_EXP_BASE = 40;                     // EXP dasar musuh fallback.
        inline constexpr int FALLBACK_EXP_PER_DEPTH = 30;                // Tambahan EXP fallback per depth.
        inline constexpr double FALLBACK_CRIT_RATE = 0.05;               // Critical rate musuh fallback.
        inline constexpr int TEMPLATE_HP_MIN_FALLBACK = 10;              // HP min fallback dari template musuh.
        inline constexpr int TEMPLATE_HP_MAX_FALLBACK = 20;              // HP max fallback dari template musuh.
        inline constexpr int TEMPLATE_MP_MIN_FALLBACK = 0;               // MP min fallback dari template musuh.
        inline constexpr int TEMPLATE_MP_MAX_FALLBACK = 0;               // MP max fallback dari template musuh.
        inline constexpr int TEMPLATE_ATK_MIN_FALLBACK = 1;              // ATK min fallback dari template musuh.
        inline constexpr int TEMPLATE_ATK_MAX_FALLBACK = 2;              // ATK max fallback dari template musuh.
        inline constexpr int TEMPLATE_DEF_MIN_FALLBACK = 0;              // DEF min fallback dari template musuh.
        inline constexpr int TEMPLATE_DEF_MAX_FALLBACK = 1;              // DEF max fallback dari template musuh.
        inline constexpr double TEMPLATE_CRIT_RATE_FALLBACK = 0.05;      // Critical rate fallback dari template musuh.
        inline constexpr int DEFAULT_EXP_DROP = 10;                      // EXP drop default sebelum enemy diisi data spawn.
        inline constexpr int DEFAULT_DROP_RATE = 100;                    // Drop rate fallback dalam persen.
        inline constexpr int DROP_QUANTITY = 1;                          // Jumlah item yang diberikan saat drop berhasil.
        inline constexpr double REGULAR_ENEMY_ATTACK_MULTIPLIER = 0.75;  // Pengali ATK musuh non-boss.
        inline constexpr double BOSS_ENEMY_ATTACK_MULTIPLIER = 1.00;     // Pengali ATK musuh boss.
        inline constexpr double ENEMY_ATTACK_DEBUFF_MULTIPLIER = 0.80;   // Pengali ATK musuh saat debuff.
        inline constexpr double ENEMY_DEFENSE_DEBUFF_MULTIPLIER = 0.60;  // Pengali DEF musuh saat debuff.
        inline constexpr int ENEMY_SPEED_PER_LEVEL = 2;                  // Kontribusi level musuh ke speed.
        inline constexpr int ENEMY_SPEED_ATTACK_DIVISOR = 6;             // Pembagi ATK musuh untuk speed.
        inline constexpr int ENEMY_DAMAGE_JITTER_MIN = -2;               // Variasi damage minimum serangan musuh.
        inline constexpr int ENEMY_DAMAGE_JITTER_MAX = 2;                // Variasi damage maksimum serangan musuh.
        inline constexpr double ENEMY_CRITICAL_DAMAGE_MULTIPLIER = 1.30; // Pengali damage critical musuh.
        inline constexpr int ENEMY_OPENING_AMBUSH_SPEED_BONUS = 5;       // Bonus speed musuh untuk ambush.
        inline constexpr int MIN_DAMAGE = 1;                             // Damage minimum musuh saat menyerang.
        inline constexpr int MIN_DEFENSE = 0;                            // DEF minimum musuh setelah kalkulasi.
        inline constexpr int BOSS_HP_MULTIPLIER_NUMERATOR = 14;          // Pembilang alternatif HP boss jika butuh rasio integer.
        inline constexpr double BOSS_HP_MULTIPLIER = 1.40;               // Pengali HP boss.
        inline constexpr double BOSS_ATK_MULTIPLIER = 1.10;              // Pengali ATK boss.
        inline constexpr double BOSS_DEF_MULTIPLIER = 1.05;              // Pengali DEF boss.
        inline constexpr double BOSS_MINIMUM_CRIT_RATE = 0.08;           // Critical rate minimum boss.
        inline constexpr int EXP_DROP_PER_ENEMY_LEVEL = 26;              // EXP reward per level musuh.
        inline constexpr int EXP_DROP_PER_DEPTH = 20;                    // EXP reward tambahan per depth.
        inline constexpr int BOSS_EXP_BONUS = 250;                       // Bonus EXP saat musuh adalah boss.
        inline constexpr int BATTLE_GOLD_MINIMUM = 22;                   // Gold minimum dari battle.
        inline constexpr int BATTLE_GOLD_PER_ENEMY_LEVEL = 10;           // Gold reward per level musuh.
        inline constexpr int BATTLE_GOLD_ATK_DIVISOR = 2;                // Pembagi ATK musuh untuk bonus gold.
        inline constexpr int DEPTH_CLEAR_GOLD_MINIMUM = 20;              // Gold minimum saat clear depth.
        inline constexpr int DEPTH_CLEAR_EXP_MINIMUM = 25;               // EXP minimum saat clear depth.
        inline constexpr int DEPTH_CLEAR_EXP_NUMERATOR = 2;              // Pembilang rasio EXP clear depth.
        inline constexpr int DEPTH_CLEAR_EXP_DENOMINATOR = 5;            // Penyebut rasio EXP clear depth.
        inline constexpr int DEPTH_CLEAR_GOLD_DIVISOR = 5;               // Pembagi EXP dasar untuk gold clear depth.
    }
}
