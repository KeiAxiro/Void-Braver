#pragma once

namespace Config
{
    namespace Skill
    {
        inline constexpr int FIRST_SKILL_INDEX = 1;                              // Index skill pertama di UI.
        inline constexpr int SKILL_TREE_SPLIT_DIVISOR = 2;                       // Pembagi untuk mencari node tengah skill tree.
        inline constexpr int UNLOCK_LEVEL_SLOT_1 = 1;                            // Level unlock skill slot 1.
        inline constexpr int UNLOCK_LEVEL_SLOT_2 = 3;                            // Level unlock skill slot 2.
        inline constexpr int UNLOCK_LEVEL_SLOT_3 = 6;                            // Level unlock skill slot 3.
        inline constexpr int UNLOCK_LEVEL_SLOT_4 = 20;                           // Level unlock skill slot 4.
        inline constexpr int UNLOCK_LEVEL_SLOT_5 = 25;                           // Level unlock skill slot 5.
        inline constexpr int UNLOCK_LEVEL_SLOT_6 = 31;                           // Level unlock skill slot 6.
        inline constexpr int UNLOCK_LEVEL_SLOT_7 = 50;                           // Level unlock skill slot 7.
        inline constexpr int UNLOCK_LEVEL_SLOT_8 = 58;                           // Level unlock skill slot 8.
        inline constexpr int UNLOCK_LEVEL_SLOT_9 = 67;                           // Level unlock skill slot 9.
        inline constexpr int UNLOCK_LEVELS[] = {UNLOCK_LEVEL_SLOT_1, UNLOCK_LEVEL_SLOT_2, UNLOCK_LEVEL_SLOT_3, UNLOCK_LEVEL_SLOT_4, UNLOCK_LEVEL_SLOT_5, UNLOCK_LEVEL_SLOT_6, UNLOCK_LEVEL_SLOT_7, UNLOCK_LEVEL_SLOT_8, UNLOCK_LEVEL_SLOT_9}; // Tabel level unlock skill berdasarkan urutan slot.
        inline constexpr int UNLOCK_LEVEL_COUNT = sizeof(UNLOCK_LEVELS) / sizeof(UNLOCK_LEVELS[0]); // Jumlah slot di tabel unlock skill.
        inline constexpr int NO_DAMAGE = 0;                                      // Nilai damage untuk skill utility tanpa damage.
        inline constexpr int MIN_DAMAGE = 1;                                     // Damage minimum skill setelah kalkulasi.
        inline constexpr int DEFAULT_DEFENSE_DIVISOR = 2;                        // Pembagi DEF musuh pada banyak formula skill.
        inline constexpr int BLADE_DANCE_DEFENSE_DIVISOR = 3;                    // Pembagi DEF musuh per hit Blade Dance.
        inline constexpr double POMMEL_STRIKE_MULTIPLIER = 1.60;                 // Pengali damage Pommel Strike.
        inline constexpr int POMMEL_STRIKE_STUN_TURNS = 1;                       // Durasi stun Pommel Strike.
        inline constexpr double IRON_CLEAVE_MULTIPLIER = 2.40;                   // Pengali damage Iron Cleave.
        inline constexpr double MANA_BOLT_MULTIPLIER = 2.20;                     // Pengali damage Mana Bolt.
        inline constexpr int CHALLENGER_ROAR_DEFENSE_BUFF_TURNS = 2;             // Durasi buff DEF Challenger Roar.
        inline constexpr double IMPACT_CRATER_MULTIPLIER = 2.70;                 // Pengali damage Impact Crater.
        inline constexpr double BLOOD_AND_IRON_SELF_DAMAGE_RATIO = 0.05;         // Rasio max HP yang dikorbankan Blood and Iron.
        inline constexpr int BLOOD_AND_IRON_ATTACK_BUFF_TURNS = 3;               // Durasi buff ATK Blood and Iron.
        inline constexpr int BLOOD_AND_IRON_DEFENSE_BUFF_TURNS = 3;              // Durasi buff DEF Blood and Iron.
        inline constexpr double SHATTERING_ONSLAUGHT_MULTIPLIER = 1.90;          // Pengali damage Shattering Onslaught.
        inline constexpr int SHATTERING_ONSLAUGHT_DEFENSE_DEBUFF_TURNS = 3;      // Durasi debuff DEF Shattering Onslaught.
        inline constexpr double CALAMITY_END_MULTIPLIER = 3.20;                  // Pengali damage Calamity End.
        inline constexpr double CALAMITY_END_EXECUTE_THRESHOLD = 0.30;           // Ambang HP execute Calamity End.
        inline constexpr int DOMINION_AURA_DEFENSE_BUFF_TURNS = 3;               // Durasi buff DEF Dominion Aura.
        inline constexpr int DOMINION_AURA_ENEMY_ATTACK_DEBUFF_TURNS = 3;        // Durasi debuff ATK Dominion Aura.
        inline constexpr int UNDYING_JUGGERNAUT_INVINCIBLE_TURNS = 1;            // Durasi invincible Undying Juggernaut.
        inline constexpr int UNDYING_JUGGERNAUT_ATTACK_BUFF_TURNS = 2;           // Durasi buff ATK Undying Juggernaut.
        inline constexpr double IGNITE_PRIMARY_MULTIPLIER = 1.20;                // Pengali primary stat damage Ignite.
        inline constexpr double IGNITE_DOT_MULTIPLIER = 0.60;                    // Pengali primary stat DoT Ignite.
        inline constexpr int IGNITE_DOT_TURNS = 3;                               // Durasi DoT Ignite.
        inline constexpr int ARCANE_SHIELD_MINIMUM = 45;                         // Shield minimum Arcane Shield.
        inline constexpr double ARCANE_SHIELD_PRIMARY_MULTIPLIER = 3.0;          // Pengali primary stat shield Arcane Shield.
        inline constexpr double CHAIN_LIGHTNING_MULTIPLIER = 2.20;               // Pengali damage Chain Lightning.
        inline constexpr int CHAIN_LIGHTNING_SILENCE_TURNS = 1;                  // Durasi silence Chain Lightning.
        inline constexpr double FROST_NOVA_MULTIPLIER = 1.80;                    // Pengali damage Frost Nova.
        inline constexpr int FROST_NOVA_STUN_TURNS = 1;                          // Durasi stun Frost Nova.
        inline constexpr double METEOR_FALL_MULTIPLIER = 3.60;                   // Pengali damage Meteor Fall.
        inline constexpr double ABYSSAL_SINGULARITY_MULTIPLIER = 2.60;           // Pengali damage Abyssal Singularity.
        inline constexpr int ABYSSAL_SINGULARITY_MINIMUM_DRAIN = 15;             // Drain HP minimum Abyssal Singularity.
        inline constexpr int ABYSSAL_SINGULARITY_DRAIN_DIVISOR = 4;              // Pembagi damage menjadi drain Abyssal Singularity.
        inline constexpr int ASTRAL_REBIRTH_MINIMUM_MANA_RESTORE = 30;           // Restore MP minimum Astral Rebirth.
        inline constexpr int ASTRAL_REBIRTH_MANA_RESTORE_DIVISOR = 2;            // Pembagi max MP untuk restore Astral Rebirth.
        inline constexpr int ASTRAL_REBIRTH_ATTACK_BUFF_TURNS = 2;               // Durasi buff ATK Astral Rebirth.
        inline constexpr double VOID_APOCALYPSE_MULTIPLIER = 4.20;               // Pengali damage Void Apocalypse.
        inline constexpr double QUICK_DRAW_MULTIPLIER = 1.65;                    // Pengali damage Quick Draw.
        inline constexpr double TOXIC_ARROW_MULTIPLIER = 1.45;                   // Pengali damage Toxic Arrow.
        inline constexpr double TOXIC_ARROW_DOT_AGI_MULTIPLIER = 0.70;           // Pengali AGI DoT Toxic Arrow.
        inline constexpr int TOXIC_ARROW_DOT_TURNS = 3;                          // Durasi DoT Toxic Arrow.
        inline constexpr int AGILITY_BOOST_TURNS = 3;                            // Durasi buff AGI Agility Boost.
        inline constexpr double SHADOW_STRIKE_MULTIPLIER = 2.30;                 // Pengali damage Shadow Strike.
        inline constexpr int PHANTOM_VEIL_EVADE_TURNS = 1;                       // Durasi evade Phantom Veil.
        inline constexpr int BLADE_DANCE_MIN_HITS = 3;                           // Jumlah hit minimum Blade Dance.
        inline constexpr int BLADE_DANCE_MAX_HITS = 6;                           // Jumlah hit maksimum Blade Dance.
        inline constexpr double BLADE_DANCE_HIT_MULTIPLIER = 0.65;               // Pengali damage tiap hit Blade Dance.
        inline constexpr int OBLIVION_EDGE_EXECUTE_CHANCE = 20;                  // Peluang execute Oblivion Edge dalam persen.
        inline constexpr double OBLIVION_EDGE_MULTIPLIER = 2.80;                 // Pengali damage Oblivion Edge.
        inline constexpr double ETERNAL_SILENCE_MULTIPLIER = 2.10;               // Pengali damage Eternal Silence.
        inline constexpr int ETERNAL_SILENCE_TURNS = 2;                          // Durasi silence Eternal Silence.
        inline constexpr int SHADOW_MELD_EVADE_TURNS = 1;                        // Durasi evade Shadow Meld.
        inline constexpr double PLAYER_CRITICAL_DAMAGE_MULTIPLIER = 1.80;        // Pengali critical serangan normal player.
        inline constexpr double SKILL_CRITICAL_DAMAGE_MULTIPLIER = 2.00;         // Pengali critical skill player.
    }
}
