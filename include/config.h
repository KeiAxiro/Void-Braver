#pragma once

// Pusat konfigurasi Void Braver.
// Semua angka balance dan angka kalkulasi gameplay harus diambil dari file ini.
namespace Config
{
    namespace Paths
    {
        inline constexpr const char *GAME_DATA = "data/game_data.json"; // Path relatif untuk file data utama game.
        inline constexpr const char *SAVE_FILE = "data/save.json";      // Path relatif untuk file save pemain.
    }

    namespace Math
    {
        inline constexpr int ZERO = 0;                         // Nilai nol umum untuk counter, clamp bawah, dan fallback angka.
        inline constexpr int ONE = 1;                          // Nilai satu umum untuk jumlah minimal dan konversi indeks.
        inline constexpr int INDEX_OFFSET = 1;                 // Offset dari pilihan UI berbasis 1 ke array berbasis 0.
        inline constexpr double ZERO_RATIO = 0.0;              // Rasio nol saat rentang kalkulasi tidak valid.
        inline constexpr double UNIT_ROLL_MIN = 0.0;           // Nilai minimum roll acak pecahan 0..1.
        inline constexpr double UNIT_ROLL_MAX = 1.0;           // Nilai maksimum roll acak pecahan 0..1.
        inline constexpr int PERCENT_ROLL_MIN = 1;             // Nilai minimum roll persen inklusif.
        inline constexpr int PERCENT_ROLL_MAX = 100;           // Nilai maksimum roll persen inklusif.
        inline constexpr int COIN_FLIP_MIN = 0;                // Nilai minimum random dua pilihan.
        inline constexpr int COIN_FLIP_MAX = 1;                // Nilai maksimum random dua pilihan.
        inline constexpr int COIN_FLIP_LEFT_VALUE = 0;         // Nilai random yang dianggap memilih sisi kiri.
        inline constexpr int MIN_INTERPOLATED_STAT = 1;        // Nilai minimal hasil interpolasi stat musuh.
    }

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

    namespace Defaults
    {
        inline constexpr const char *PLAYER_NAME = "Hero";                 // Nama fallback jika nama karakter kosong.
        inline constexpr const char *PLAYER_ID = "hero";                   // ID fallback jika nama tidak punya karakter valid.
        inline constexpr const char *CLASS_ID = "knight";                  // Class fallback jika class kosong atau tidak valid.
        inline constexpr const char *PRIMARY_STAT = "STR";                 // Primary stat fallback saat class tidak ditemukan.
        inline constexpr int BOOL_TRUE_AS_INT = 1;                         // Nilai integer saat boolean true dikonversi ke angka.
        inline constexpr int BOOL_FALSE_AS_INT = 0;                        // Nilai integer saat boolean false dikonversi ke angka.
        inline constexpr int PLAYER_LEVEL = 1;                             // Level default karakter.
        inline constexpr int PLAYER_EXP = 0;                               // EXP default karakter.
        inline constexpr int PLAYER_GOLD = Rules::STARTING_GOLD;           // Gold default karakter mengikuti starting gold.
        inline constexpr int PLAYER_STAT_STR = 10;                         // STR default karakter.
        inline constexpr int PLAYER_STAT_INT = 10;                         // INT default karakter.
        inline constexpr int PLAYER_STAT_AGI = 10;                         // AGI default karakter.
        inline constexpr int PLAYER_STAT_VIT = 10;                         // VIT default karakter.
        inline constexpr int PLAYER_HP = Rules::STARTING_HP;               // HP default karakter.
        inline constexpr int PLAYER_MP = Rules::STARTING_MP;               // MP default karakter.
        inline constexpr int PLAYER_STAT_POINTS = 0;                       // Stat point default untuk data kosong.
        inline constexpr int INVENTORY_QUANTITY = 1;                       // Jumlah default satu baris inventory.
        inline constexpr int COOLDOWN_TURNS = 0;                           // Cooldown default skill.
        inline constexpr int ACTIVE_CHARACTER_NONE = -1;                   // Penanda tidak ada karakter aktif.
        inline constexpr int ACTIVE_CHARACTER_FIRST_INDEX = 0;             // Index karakter pertama setelah load save.
        inline constexpr int DUPLICATE_ID_SUFFIX_START = 1;                // Suffix awal saat membuat ID karakter duplikat.
        inline constexpr int SAVE_JSON_INDENT = 2;                         // Jumlah spasi indent JSON save.
    }

    namespace Progress
    {
        inline constexpr int START_DEPTH = 1;                  // Depth awal dungeon.
        inline constexpr int NO_DEPTH_CLEARED = 0;             // Nilai awal depth yang belum pernah diselesaikan.
        inline constexpr int NO_DEPTH_UNLOCKED = 0;            // Nilai awal max depth global yang belum terbuka.
        inline constexpr int DEPTH_UNLOCK_STEP = 1;            // Tambahan depth yang terbuka setelah clear.
        inline constexpr int LEVEL_RANGE_MIN_FALLBACK = 1;     // Min level fallback jika data depth kosong.
        inline constexpr int LEVEL_RANGE_MAX_FALLBACK = 999;   // Max level fallback jika data depth kosong.
        inline constexpr int MISSING_LEVEL = -1;                // Sentinel level saat data balance tidak ditemukan.
        inline constexpr const char *FINAL_DUNGEON_ID = "tahta_kehampaan"; // ID dungeon final yang dikunci oleh progress.
        inline constexpr int GRAPH_MAX_NODES = 16;             // Kapasitas node graph dungeon.
        inline constexpr int ROUTE_INFINITY = 1000000;         // Nilai jarak tak hingga untuk Dijkstra.
        inline constexpr int MIN_ROUTE_WEIGHT = 1;             // Bobot rute minimal antar dungeon.
    }

    namespace StarterKit
    {
        inline constexpr const char *KNIGHT_CLASS_ID = "knight";                // ID class Knight untuk starter kit.
        inline constexpr const char *SORCERER_CLASS_ID = "sorcerer";            // ID class Sorcerer untuk starter kit.
        inline constexpr const char *WANDERER_CLASS_ID = "wanderer";            // ID class Wanderer untuk starter kit.
        inline constexpr const char *HEALING_POTION_ID = "healing_potion";       // ID potion HP awal.
        inline constexpr int HEALING_POTION_QTY = 3;                             // Jumlah potion HP awal.
        inline constexpr const char *MANA_ELIXIR_ID = "mana_elixir";             // ID potion MP awal.
        inline constexpr int MANA_ELIXIR_QTY = 2;                                // Jumlah potion MP awal.
        inline constexpr const char *KNIGHT_HELMET_ID = "leather_coif";          // Helmet awal Knight.
        inline constexpr const char *KNIGHT_ARMOR_ID = "padded_tunic";           // Armor awal Knight.
        inline constexpr const char *KNIGHT_WEAPON_ID = "rusty_greatsword";      // Weapon awal Knight.
        inline constexpr const char *KNIGHT_BOOTS_ID = "traveler_s_boots";       // Boots awal Knight.
        inline constexpr const char *SORCERER_WEAPON_ID = "apprentice_wand";     // Weapon awal Sorcerer.
        inline constexpr const char *SORCERER_ARMOR_ID = "novice_robe";          // Armor awal Sorcerer.
        inline constexpr const char *WANDERER_WEAPON_ID = "short_bow";           // Weapon awal Wanderer.
        inline constexpr const char *WANDERER_HELMET_ID = "leather_cap";         // Helmet awal Wanderer.
        inline constexpr const char *WANDERER_ARMOR_ID = "rough_tunic";          // Armor awal Wanderer.
        inline constexpr int EQUIPMENT_QTY = 1;                                  // Jumlah setiap equipment starter.
    }

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

    namespace Item
    {
        inline constexpr int EQUIPMENT_SPLIT_THRESHOLD = 1;      // Quantity di atas angka ini dipecah saat equip/unequip.
        inline constexpr int EQUIPMENT_TRANSFER_QUANTITY = 1;    // Jumlah item yang dipindah saat equip/unequip satu item.
        inline constexpr int CONSUMABLE_USE_QUANTITY = 1;        // Jumlah consumable yang habis setiap dipakai.
        inline constexpr int CONSUMABLE_HP_MIN_HEAL = 30;        // Heal HP minimum dari consumable HP.
        inline constexpr int CONSUMABLE_MP_MIN_HEAL = 20;        // Heal MP minimum dari consumable MP.
        inline constexpr int CONSUMABLE_HEAL_DIVISOR = 4;        // Pembagi max HP/MP untuk heal consumable.
        inline constexpr int CRAFT_RESULT_QUANTITY = 1;          // Jumlah item hasil crafting.
    }

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

    namespace Ui
    {
        inline constexpr int DEFAULT_LINE_WIDTH = 40;        // Lebar default garis pemisah console.
        inline constexpr int MIN_TITLE_BOX_WIDTH = 40;       // Lebar minimum kotak judul.
        inline constexpr int TITLE_BOX_PADDING = 4;          // Padding kiri/kanan kotak judul.
        inline constexpr int TITLE_BOX_BORDER_WIDTH = 2;     // Lebar border horizontal kotak judul.
        inline constexpr int DIALOG_LINE_WIDTH = 55;         // Lebar garis pemisah dialog.
        inline constexpr int HUB_HEADER_WIDTH = 64;          // Lebar garis header hub.
    }

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

// ============================================================================
// JEMBATAN NAMESPACE LAMA
// ============================================================================
namespace game_paths {
    inline constexpr const char *kGameDataPath = Config::Paths::GAME_DATA;
    inline constexpr const char *kSaveFilePath = Config::Paths::SAVE_FILE;
}
namespace game_rules {
    inline constexpr int kStartingGold = Config::Rules::STARTING_GOLD;
    inline constexpr int kStartingHp = Config::Rules::STARTING_HP;
    inline constexpr int kStartingMp = Config::Rules::STARTING_MP;
    inline constexpr int kStartingStatPoints = Config::Rules::STARTING_STAT_POINTS;
    inline constexpr int kStatPointsPerLevel = Config::Rules::STAT_POINTS_PER_LEVEL;
    inline constexpr int kDefaultPageSize = Config::Rules::DEFAULT_PAGE_SIZE;
    inline constexpr int kMoveStepsPerDepth = Config::Rules::MOVE_STEPS_PER_DEPTH;
    inline constexpr int kMinEncountersPerMove = Config::Rules::MIN_ENCOUNTERS_PER_MOVE;
    inline constexpr int kMaxEncountersPerMove = Config::Rules::MAX_ENCOUNTERS_PER_MOVE;
    inline constexpr const char *kMenuSeparator = Config::Rules::MENU_SEPARATOR;
    inline constexpr const char *kTitleSeparator = Config::Rules::TITLE_SEPARATOR;
}
namespace player_balance {
    inline constexpr int kMaxEffectiveDefenseForDamage = Config::Player::MAX_EFFECTIVE_DEFENSE_FOR_DAMAGE;
    inline constexpr int kMaxEffectiveDefenseForEnemyDamage = Config::Player::MAX_EFFECTIVE_PLAYER_DEFENSE_FOR_ENEMY_DAMAGE;
    inline constexpr int kDefenseReducedPlayerDamageFloorDenominator = Config::Player::DEFENSE_REDUCED_PLAYER_DAMAGE_FLOOR_DENOMINATOR;



    inline constexpr int kBaseHpStart = Config::Player::BASE_HP_START;
    inline constexpr int kBaseHpPerLevel = Config::Player::BASE_HP_PER_LEVEL;
    inline constexpr int kBaseHpPerVit = Config::Player::BASE_HP_PER_VIT;
    inline constexpr int kBaseHpPerStr = Config::Player::BASE_HP_PER_STR;
    inline constexpr int kBaseMpStart = Config::Player::BASE_MP_START;
    inline constexpr int kBaseMpPerLevel = Config::Player::BASE_MP_PER_LEVEL;
    inline constexpr int kBaseMpPerInt = Config::Player::BASE_MP_PER_INT;
    inline constexpr int kBaseMpAgiDivisor = Config::Player::BASE_MP_AGI_DIVISOR;
    inline constexpr int kBaseMpPerTwoAgi = Config::Player::BASE_MP_PER_TWO_AGI;
    inline constexpr int kAttackBasePerLevel = Config::Player::ATTACK_BASE_PER_LEVEL;
    inline constexpr int kAttackPrimaryDivisor = Config::Player::ATTACK_PRIMARY_DIVISOR;
    inline constexpr int kAttackStrDivisor = Config::Player::ATTACK_STR_DIVISOR;
    inline constexpr int kAttackAgiDivisor = Config::Player::ATTACK_AGI_DIVISOR;
    inline constexpr double kAttackEquipmentMultiplier = Config::Player::ATTACK_EQUIPMENT_MULTIPLIER;
    inline constexpr double kAttackBuffMultiplier = Config::Player::ATTACK_BUFF_MULTIPLIER;
    inline constexpr int kAttackJitterMin = Config::Player::ATTACK_JITTER_MIN;
    inline constexpr int kAttackJitterMax = Config::Player::ATTACK_JITTER_MAX;
    inline constexpr int kDefenseBasePerLevel = Config::Player::DEFENSE_BASE_PER_LEVEL;
    inline constexpr int kDefenseVitDivisor = Config::Player::DEFENSE_VIT_DIVISOR;
    inline constexpr int kDefenseStrDivisor = Config::Player::DEFENSE_STR_DIVISOR;
    inline constexpr double kDefenseEquipmentMultiplier = Config::Player::DEFENSE_EQUIPMENT_MULTIPLIER;
    inline constexpr double kDefenseBuffMultiplier = Config::Player::DEFENSE_BUFF_MULTIPLIER;
    inline constexpr int kSpeedBasePerLevel = Config::Player::SPEED_BASE_PER_LEVEL;
    inline constexpr int kSpeedPerAgi = Config::Player::SPEED_PER_AGI;
    inline constexpr double kSpeedBuffMultiplier = Config::Player::SPEED_BUFF_MULTIPLIER;
    inline constexpr double kCritBaseRate = Config::Player::CRIT_BASE_RATE;
    inline constexpr double kCritPerAgi = Config::Player::CRIT_PER_AGI;
    inline constexpr double kCritCap = Config::Player::CRIT_CAP;
    inline constexpr int kPrimaryStatBonusOnNewGame = Config::Player::PRIMARY_STAT_BONUS_ON_NEW_GAME;
    inline constexpr int kDefeatRecoveryDivisor = Config::Player::DEFEAT_RECOVERY_DIVISOR;
    inline constexpr int kMinAttack = Config::Player::MIN_ATTACK;
    inline constexpr int kMinDefense = Config::Player::MIN_DEFENSE;
    inline constexpr int kMinSpeed = Config::Player::MIN_SPEED;
    inline constexpr int kMinDamage = Config::Player::MIN_DAMAGE;
    inline constexpr int kResourceFloor = Config::Player::RESOURCE_FLOOR;
    inline constexpr int kMinRecoveryResource = Config::Player::MIN_RECOVERY_RESOURCE;
    inline constexpr double kLevelUpEarlyMultiplier = Config::Player::LEVEL_UP_EARLY_MULTIPLIER;
    inline constexpr int kLevelUpMidStartLevel = Config::Player::LEVEL_UP_MID_START_LEVEL;
    inline constexpr double kLevelUpMidMultiplier = Config::Player::LEVEL_UP_MID_MULTIPLIER;
    inline constexpr int kLevelUpLateStartLevel = Config::Player::LEVEL_UP_LATE_START_LEVEL;
    inline constexpr double kLevelUpLateMultiplier = Config::Player::LEVEL_UP_LATE_MULTIPLIER;
    inline constexpr int kLevelUpEndgameStartLevel = Config::Player::LEVEL_UP_ENDGAME_START_LEVEL;
    inline constexpr double kLevelUpEndgameMultiplier = Config::Player::LEVEL_UP_ENDGAME_MULTIPLIER;
    inline constexpr int kExpFallbackLinearMultiplier = Config::Player::EXP_FALLBACK_LINEAR_MULTIPLIER;
    inline constexpr int kExpFallbackQuadraticMultiplier = Config::Player::EXP_FALLBACK_QUADRATIC_MULTIPLIER;
    inline constexpr int kMinimumExpRequirement = Config::Player::MINIMUM_EXP_REQUIREMENT;
}
namespace enemy_balance {
    inline constexpr int kFallbackHpBase = Config::Enemy::FALLBACK_HP_BASE;
    inline constexpr int kFallbackHpPerDepth = Config::Enemy::FALLBACK_HP_PER_DEPTH;
    inline constexpr int kFallbackMpBase = Config::Enemy::FALLBACK_MP_BASE;
    inline constexpr int kFallbackMpPerDepth = Config::Enemy::FALLBACK_MP_PER_DEPTH;
    inline constexpr int kFallbackAtkBase = Config::Enemy::FALLBACK_ATK_BASE;
    inline constexpr int kFallbackAtkPerDepth = Config::Enemy::FALLBACK_ATK_PER_DEPTH;
    inline constexpr int kFallbackDefBase = Config::Enemy::FALLBACK_DEF_BASE;
    inline constexpr int kFallbackDefPerDepth = Config::Enemy::FALLBACK_DEF_PER_DEPTH;
    inline constexpr int kFallbackExpBase = Config::Enemy::FALLBACK_EXP_BASE;
    inline constexpr int kFallbackExpPerDepth = Config::Enemy::FALLBACK_EXP_PER_DEPTH;
    inline constexpr double kFallbackCritRate = Config::Enemy::FALLBACK_CRIT_RATE;
    inline constexpr int kTemplateHpMinFallback = Config::Enemy::TEMPLATE_HP_MIN_FALLBACK;
    inline constexpr int kTemplateHpMaxFallback = Config::Enemy::TEMPLATE_HP_MAX_FALLBACK;
    inline constexpr int kTemplateMpMinFallback = Config::Enemy::TEMPLATE_MP_MIN_FALLBACK;
    inline constexpr int kTemplateMpMaxFallback = Config::Enemy::TEMPLATE_MP_MAX_FALLBACK;
    inline constexpr int kTemplateAtkMinFallback = Config::Enemy::TEMPLATE_ATK_MIN_FALLBACK;
    inline constexpr int kTemplateAtkMaxFallback = Config::Enemy::TEMPLATE_ATK_MAX_FALLBACK;
    inline constexpr int kTemplateDefMinFallback = Config::Enemy::TEMPLATE_DEF_MIN_FALLBACK;
    inline constexpr int kTemplateDefMaxFallback = Config::Enemy::TEMPLATE_DEF_MAX_FALLBACK;
    inline constexpr double kTemplateCritRateFallback = Config::Enemy::TEMPLATE_CRIT_RATE_FALLBACK;
    inline constexpr int kDefaultExpDrop = Config::Enemy::DEFAULT_EXP_DROP;
    inline constexpr int kDefaultDropRate = Config::Enemy::DEFAULT_DROP_RATE;
    inline constexpr int kDropQuantity = Config::Enemy::DROP_QUANTITY;
    inline constexpr double kRegularEnemyAttackMultiplier = Config::Enemy::REGULAR_ENEMY_ATTACK_MULTIPLIER;
    inline constexpr double kBossEnemyAttackMultiplier = Config::Enemy::BOSS_ENEMY_ATTACK_MULTIPLIER;
    inline constexpr double kEnemyAttackDebuffMultiplier = Config::Enemy::ENEMY_ATTACK_DEBUFF_MULTIPLIER;
    inline constexpr double kEnemyDefenseDebuffMultiplier = Config::Enemy::ENEMY_DEFENSE_DEBUFF_MULTIPLIER;
    inline constexpr int kEnemySpeedPerLevel = Config::Enemy::ENEMY_SPEED_PER_LEVEL;
    inline constexpr int kEnemySpeedAttackDivisor = Config::Enemy::ENEMY_SPEED_ATTACK_DIVISOR;
    inline constexpr int kEnemyDamageJitterMin = Config::Enemy::ENEMY_DAMAGE_JITTER_MIN;
    inline constexpr int kEnemyDamageJitterMax = Config::Enemy::ENEMY_DAMAGE_JITTER_MAX;
    inline constexpr double kEnemyCriticalDamageMultiplier = Config::Enemy::ENEMY_CRITICAL_DAMAGE_MULTIPLIER;
    inline constexpr int kEnemyOpeningAmbushSpeedBonus = Config::Enemy::ENEMY_OPENING_AMBUSH_SPEED_BONUS;
    inline constexpr int kMinDamage = Config::Enemy::MIN_DAMAGE;
    inline constexpr int kMinDefense = Config::Enemy::MIN_DEFENSE;
    inline constexpr double kBossHpMultiplier = Config::Enemy::BOSS_HP_MULTIPLIER;
    inline constexpr double kBossAtkMultiplier = Config::Enemy::BOSS_ATK_MULTIPLIER;
    inline constexpr double kBossDefMultiplier = Config::Enemy::BOSS_DEF_MULTIPLIER;
    inline constexpr double kBossMinimumCritRate = Config::Enemy::BOSS_MINIMUM_CRIT_RATE;
    inline constexpr int kExpDropPerEnemyLevel = Config::Enemy::EXP_DROP_PER_ENEMY_LEVEL;
    inline constexpr int kExpDropPerDepth = Config::Enemy::EXP_DROP_PER_DEPTH;
    inline constexpr int kBossExpBonus = Config::Enemy::BOSS_EXP_BONUS;
    inline constexpr int kBattleGoldMinimum = Config::Enemy::BATTLE_GOLD_MINIMUM;
    inline constexpr int kBattleGoldPerEnemyLevel = Config::Enemy::BATTLE_GOLD_PER_ENEMY_LEVEL;
    inline constexpr int kBattleGoldAtkDivisor = Config::Enemy::BATTLE_GOLD_ATK_DIVISOR;
    inline constexpr int kDepthClearGoldMinimum = Config::Enemy::DEPTH_CLEAR_GOLD_MINIMUM;
    inline constexpr int kDepthClearExpMinimum = Config::Enemy::DEPTH_CLEAR_EXP_MINIMUM;
    inline constexpr int kDepthClearExpNumerator = Config::Enemy::DEPTH_CLEAR_EXP_NUMERATOR;
    inline constexpr int kDepthClearExpDenominator = Config::Enemy::DEPTH_CLEAR_EXP_DENOMINATOR;
    inline constexpr int kDepthClearGoldDivisor = Config::Enemy::DEPTH_CLEAR_GOLD_DIVISOR;
}
namespace skill_balance {
    inline constexpr int kFirstSkillIndex = Config::Skill::FIRST_SKILL_INDEX;
    inline constexpr int kSkillTreeSplitDivisor = Config::Skill::SKILL_TREE_SPLIT_DIVISOR;
    inline constexpr const int *kUnlockLevels = Config::Skill::UNLOCK_LEVELS;
    inline constexpr int kUnlockLevelCount = Config::Skill::UNLOCK_LEVEL_COUNT;
    inline constexpr int kNoDamage = Config::Skill::NO_DAMAGE;
    inline constexpr int kMinDamage = Config::Skill::MIN_DAMAGE;
    inline constexpr int kDefaultDefenseDivisor = Config::Skill::DEFAULT_DEFENSE_DIVISOR;
    inline constexpr int kBladeDanceDefenseDivisor = Config::Skill::BLADE_DANCE_DEFENSE_DIVISOR;
    inline constexpr double kPommelStrikeMultiplier = Config::Skill::POMMEL_STRIKE_MULTIPLIER;
    inline constexpr int kPommelStrikeStunTurns = Config::Skill::POMMEL_STRIKE_STUN_TURNS;
    inline constexpr double kIronCleaveMultiplier = Config::Skill::IRON_CLEAVE_MULTIPLIER;
    inline constexpr double kManaBoltMultiplier = Config::Skill::MANA_BOLT_MULTIPLIER;
    inline constexpr int kChallengerRoarDefenseBuffTurns = Config::Skill::CHALLENGER_ROAR_DEFENSE_BUFF_TURNS;
    inline constexpr double kImpactCraterMultiplier = Config::Skill::IMPACT_CRATER_MULTIPLIER;
    inline constexpr double kBloodAndIronSelfDamageRatio = Config::Skill::BLOOD_AND_IRON_SELF_DAMAGE_RATIO;
    inline constexpr int kBloodAndIronAttackBuffTurns = Config::Skill::BLOOD_AND_IRON_ATTACK_BUFF_TURNS;
    inline constexpr int kBloodAndIronDefenseBuffTurns = Config::Skill::BLOOD_AND_IRON_DEFENSE_BUFF_TURNS;
    inline constexpr double kShatteringOnslaughtMultiplier = Config::Skill::SHATTERING_ONSLAUGHT_MULTIPLIER;
    inline constexpr int kShatteringOnslaughtDefenseDebuffTurns = Config::Skill::SHATTERING_ONSLAUGHT_DEFENSE_DEBUFF_TURNS;
    inline constexpr double kCalamityEndMultiplier = Config::Skill::CALAMITY_END_MULTIPLIER;
    inline constexpr double kCalamityEndExecuteThreshold = Config::Skill::CALAMITY_END_EXECUTE_THRESHOLD;
    inline constexpr int kDominionAuraDefenseBuffTurns = Config::Skill::DOMINION_AURA_DEFENSE_BUFF_TURNS;
    inline constexpr int kDominionAuraEnemyAttackDebuffTurns = Config::Skill::DOMINION_AURA_ENEMY_ATTACK_DEBUFF_TURNS;
    inline constexpr int kUndyingJuggernautInvincibleTurns = Config::Skill::UNDYING_JUGGERNAUT_INVINCIBLE_TURNS;
    inline constexpr int kUndyingJuggernautAttackBuffTurns = Config::Skill::UNDYING_JUGGERNAUT_ATTACK_BUFF_TURNS;
    inline constexpr double kIgnitePrimaryMultiplier = Config::Skill::IGNITE_PRIMARY_MULTIPLIER;
    inline constexpr double kIgniteDotMultiplier = Config::Skill::IGNITE_DOT_MULTIPLIER;
    inline constexpr int kIgniteDotTurns = Config::Skill::IGNITE_DOT_TURNS;
    inline constexpr int kArcaneShieldMinimum = Config::Skill::ARCANE_SHIELD_MINIMUM;
    inline constexpr double kArcaneShieldPrimaryMultiplier = Config::Skill::ARCANE_SHIELD_PRIMARY_MULTIPLIER;
    inline constexpr double kChainLightningMultiplier = Config::Skill::CHAIN_LIGHTNING_MULTIPLIER;
    inline constexpr int kChainLightningSilenceTurns = Config::Skill::CHAIN_LIGHTNING_SILENCE_TURNS;
    inline constexpr double kFrostNovaMultiplier = Config::Skill::FROST_NOVA_MULTIPLIER;
    inline constexpr int kFrostNovaStunTurns = Config::Skill::FROST_NOVA_STUN_TURNS;
    inline constexpr double kMeteorFallMultiplier = Config::Skill::METEOR_FALL_MULTIPLIER;
    inline constexpr double kAbyssalSingularityMultiplier = Config::Skill::ABYSSAL_SINGULARITY_MULTIPLIER;
    inline constexpr int kAbyssalSingularityMinimumDrain = Config::Skill::ABYSSAL_SINGULARITY_MINIMUM_DRAIN;
    inline constexpr int kAbyssalSingularityDrainDivisor = Config::Skill::ABYSSAL_SINGULARITY_DRAIN_DIVISOR;
    inline constexpr int kAstralRebirthMinimumManaRestore = Config::Skill::ASTRAL_REBIRTH_MINIMUM_MANA_RESTORE;
    inline constexpr int kAstralRebirthManaRestoreDivisor = Config::Skill::ASTRAL_REBIRTH_MANA_RESTORE_DIVISOR;
    inline constexpr int kAstralRebirthAttackBuffTurns = Config::Skill::ASTRAL_REBIRTH_ATTACK_BUFF_TURNS;
    inline constexpr double kVoidApocalypseMultiplier = Config::Skill::VOID_APOCALYPSE_MULTIPLIER;
    inline constexpr double kQuickDrawMultiplier = Config::Skill::QUICK_DRAW_MULTIPLIER;
    inline constexpr double kToxicArrowMultiplier = Config::Skill::TOXIC_ARROW_MULTIPLIER;
    inline constexpr double kToxicArrowDotAgiMultiplier = Config::Skill::TOXIC_ARROW_DOT_AGI_MULTIPLIER;
    inline constexpr int kToxicArrowDotTurns = Config::Skill::TOXIC_ARROW_DOT_TURNS;
    inline constexpr int kAgilityBoostTurns = Config::Skill::AGILITY_BOOST_TURNS;
    inline constexpr double kShadowStrikeMultiplier = Config::Skill::SHADOW_STRIKE_MULTIPLIER;
    inline constexpr int kPhantomVeilEvadeTurns = Config::Skill::PHANTOM_VEIL_EVADE_TURNS;
    inline constexpr int kBladeDanceMinHits = Config::Skill::BLADE_DANCE_MIN_HITS;
    inline constexpr int kBladeDanceMaxHits = Config::Skill::BLADE_DANCE_MAX_HITS;
    inline constexpr double kBladeDanceHitMultiplier = Config::Skill::BLADE_DANCE_HIT_MULTIPLIER;
    inline constexpr int kOblivionEdgeExecuteChance = Config::Skill::OBLIVION_EDGE_EXECUTE_CHANCE;
    inline constexpr double kOblivionEdgeMultiplier = Config::Skill::OBLIVION_EDGE_MULTIPLIER;
    inline constexpr double kEternalSilenceMultiplier = Config::Skill::ETERNAL_SILENCE_MULTIPLIER;
    inline constexpr int kEternalSilenceTurns = Config::Skill::ETERNAL_SILENCE_TURNS;
    inline constexpr int kShadowMeldEvadeTurns = Config::Skill::SHADOW_MELD_EVADE_TURNS;
    inline constexpr double kPlayerCriticalDamageMultiplier = Config::Skill::PLAYER_CRITICAL_DAMAGE_MULTIPLIER;
    inline constexpr double kSkillCriticalDamageMultiplier = Config::Skill::SKILL_CRITICAL_DAMAGE_MULTIPLIER;
}
