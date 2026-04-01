#include "Entities/Item.h"
#include <algorithm>

namespace Entities {

    std::vector<ItemTemplate> getAllItemTemplates() {
        return {
            // ================= MATERIAL & CONSUMABLE (ID: 1-1004) =================
            {1, 7, "Iron Scrap", "Material Dasar Universal", 100, 40, 0, 0, 0, 0, 0, ""},
            {2, 7, "Heavy Ore", "Material Dasar Knight", 0, 60, 1, 0, 0, 0, 0, ""},
            {3, 7, "Refined Plate", "Material Olahan Knight", 0, 450, 1, 0, 0, 0, 0, ""},
            {4, 7, "Mana Shard", "Material Dasar Sorcerer", 0, 60, 2, 0, 0, 0, 0, ""},
            {5, 7, "Arcane Crystal", "Material Olahan Sorcerer", 0, 450, 2, 0, 0, 0, 0, ""},
            {6, 7, "Fine Fiber", "Material Dasar Wanderer", 0, 60, 3, 0, 0, 0, 0, ""},
            {7, 7, "Silent Thread", "Material Olahan Wanderer", 0, 450, 3, 0, 0, 0, 0, ""},
            {8, 7, "Void Essence", "Material Rare dari musuh Void", 0, 1500, 0, 0, 0, 0, 0, ""},
            
            {1001, 1, "Healing Potion", "Memulihkan sejumlah HP.", 200, 100, 0, 0, 0, 0, 0, ""},
            {1002, 1, "Mana Elixir", "Memulihkan sejumlah Mana/MP.", 300, 150, 0, 0, 0, 0, 0, ""},
            {1003, 1, "Soul Lantern", "Memberikan Auto-Revive satu kali kesempatan.", 5000, 2500, 0, 0, 0, 0, 0, ""},
            {1004, 1, "Infinity Sphere", "Mengurangi seluruh durasi cooldown skill 1-2 turn.", 3500, 1800, 0, 0, 0, 0, 0, ""},

            // ================= KNIGHT EQUIPMENT (ID: 1101-1140) =================
            {1101, 2, "Rusty Greatsword", "Pedang tua yang sudah berkarat.", 500, 250, 1, 1, 15, 0, 0, ""},
            {1102, 2, "Iron Claymore", "Senjata standar prajurit kerajaan.", 0, 600, 1, 2, 35, 0, 0, ""},
            {1103, 2, "Steel Bastard Sword", "Lebih ringan namun sangat tajam.", 2500, 1500, 1, 3, 65, 0, 0, ""},
            {1104, 2, "Knight’s Heavy Axe", "Kapak berat untuk menghancurkan zirah.", 0, 3000, 1, 4, 110, 0, 0, ""},
            {1105, 2, "Royal Vanguard Blade", "Pedang kehormatan penjaga istana.", 10000, 6000, 1, 5, 175, 0, 0, ""},
            {1106, 2, "Ashen Smasher", "Dibuat dari lava beku Dungeon of Ember.", 0, 12000, 1, 6, 260, 0, 0, ""},
            {1107, 2, "Deep-Earth Maul", "Palu raksasa dari akar Dungeon of Thorns.", 0, 25000, 1, 7, 365, 0, 0, ""},
            {1108, 2, "Executioner’s Pride", "Senjata legendaris para algojo kerajaan.", 0, 45000, 1, 8, 485, 0, 0, ""},
            {1109, 2, "Void-Iron Cleaver", "Besi yang ditempa dengan energi The Void.", 0, 80000, 1, 9, 610, 0, 0, ""},
            {1110, 2, "Calamity Breaker", "Senjata pamungkas untuk sang Warlord.", 0, 150000, 1, 10, 750, 0, 0, ""},

            {1111, 3, "Leather Coif", "Helm kulit pelindung dasar.", 400, 200, 1, 1, 0, 100, 5, ""},
            {1112, 3, "Iron Pot Helm", "Helm besi standar.", 0, 450, 1, 2, 0, 250, 10, ""},
            {1113, 3, "Steel Sallet", "Helm baja menutupi sebagian wajah.", 2000, 1000, 1, 3, 0, 500, 20, ""},
            {1114, 3, "Guard’s Visor", "Helm dengan visor pelindung penuh.", 0, 2200, 1, 4, 0, 900, 30, ""},
            {1115, 3, "Commander’s Crest", "Helm berlambang komandan perang.", 8500, 4500, 1, 5, 0, 1400, 45, ""},
            {1116, 3, "Ember-Steel Mask", "Topeng baja bara api.", 0, 9000, 1, 6, 0, 2000, 65, ""},
            {1117, 3, "Grave-Guardian Helm", "Helm penjaga makam kuno.", 0, 18000, 1, 7, 0, 2750, 90, ""},
            {1118, 3, "Gladiator’s Crown", "Mahkota para gladiator tak terkalahkan.", 0, 35000, 1, 8, 0, 3750, 120, ""},
            {1119, 3, "Abyssal Greathelm", "Helm besar yang ditempa di jurang kegelapan.", 0, 60000, 1, 9, 0, 5000, 155, ""},
            {1120, 3, "Marshall’s Iron Face", "Wajah besi sang jenderal agung.", 0, 120000, 1, 10, 0, 8000, 200, ""},

            {1121, 4, "Padded Tunic", "Tunik kain empuk pelindung dasar.", 800, 400, 1, 1, 0, 200, 10, ""},
            {1122, 4, "Reinforced Leather", "Zirah kulit yang diperkuat.", 0, 900, 1, 2, 0, 500, 20, ""},
            {1123, 4, "Iron Chainmail", "Zirah rantai besi.", 4000, 2000, 1, 3, 0, 1000, 40, ""},
            {1124, 4, "Half-Plate Mail", "Zirah plat baja setengah badan.", 0, 4500, 1, 4, 0, 1800, 60, ""},
            {1125, 4, "Knight’s Plate", "Zirah plat baja penuh standar kesatria.", 15000, 8500, 1, 5, 0, 2800, 90, ""},
            {1126, 4, "Volcanic Cuirass", "Pelindung dada dari batuan vulkanik.", 0, 18000, 1, 6, 0, 4000, 130, ""},
            {1127, 4, "Ancient Oak Armor", "Zirah dari kayu oak kuno berkekuatan sihir.", 0, 35000, 1, 7, 0, 5500, 180, ""},
            {1128, 4, "Dragonscale Hauberk", "Zirah yang terbuat dari sisik naga.", 0, 65000, 1, 8, 0, 7500, 240, ""},
            {1129, 4, "Void-Reaper Plate", "Zirah dari sang malaikat maut hampa.", 0, 110000, 1, 9, 0, 10000, 310, ""},
            {1130, 4, "God-King’s Bastion", "Benteng pelindung raja dewa.", 0, 250000, 1, 10, 0, 15000, 400, ""},

            {1131, 5, "Traveler’s Boots", "Sepatu bot petualang.", 300, 150, 1, 1, 0, 100, 5, ""},
            {1132, 5, "Hardened Soles", "Sepatu dengan sol yang dikeraskan.", 0, 350, 1, 2, 0, 250, 10, ""},
            {1133, 5, "Iron Greaves", "Pelindung kaki dari besi.", 1500, 750, 1, 3, 0, 500, 20, ""},
            {1134, 5, "Plated Sabatons", "Sepatu bot dengan lapisan plat baja.", 0, 1500, 1, 4, 0, 900, 30, ""},
            {1135, 5, "Heavy War-Boots", "Sepatu bot perang kelas berat.", 6000, 3000, 1, 5, 0, 1400, 45, ""},
            {1136, 5, "Magma-Walkers", "Sepatu bot penahan panas magma.", 0, 6500, 1, 6, 0, 2000, 65, ""},
            {1137, 5, "Ever-Root Greaves", "Sepatu akar yang mengikat ke bumi.", 0, 12000, 1, 7, 0, 2750, 90, ""},
            {1138, 5, "Titan’s Footsteps", "Langkah kaki para Titan.", 0, 22000, 1, 8, 0, 3750, 120, ""},
            {1139, 5, "Shadow-Step Sabatons", "Sepatu baja bayangan.", 0, 45000, 1, 9, 0, 5000, 155, ""},
            {1140, 5, "Grand Marshall’s Stride", "Sepatu langkah keagungan.", 0, 90000, 1, 10, 0, 8000, 200, ""},

            // ================= SORCERER EQUIPMENT (ID: 1201-1240) =================
            {1201, 2, "Apprentice Wand", "Tongkat kayu sederhana untuk pemula.", 600, 250, 2, 1, 15, 0, 0, ""},
            {1202, 2, "Birch Staff", "Kayu birch yang telah diberkati mana.", 0, 600, 2, 2, 35, 0, 0, ""},
            {1203, 2, "Crystal Scepter", "Fokus kristal untuk mempertajam mantra.", 3000, 1500, 2, 3, 65, 0, 0, ""},
            {1204, 2, "Scholar's Grimoire", "Buku mantra berisi teori sihir dasar.", 0, 3000, 2, 4, 110, 0, 0, ""},
            {1205, 2, "High Mage Rod", "Senjata standar para penyihir istana.", 12000, 6000, 2, 5, 175, 0, 0, ""},
            {1206, 2, "Ignis Core Staff", "Inti api dari Dungeon of Ember.", 0, 12000, 2, 6, 260, 0, 0, ""},
            {1207, 2, "Tome of Eternity", "Buku kuno dari Dungeon of Wisdom.", 0, 25000, 2, 7, 365, 0, 0, ""},
            {1208, 2, "Celestial Spire", "Tongkat yang menyerap energi bintang.", 0, 45000, 2, 8, 485, 0, 0, ""},
            {1209, 2, "Abyssal Eye", "Orb yang menatap langsung ke kegelapan.", 0, 80000, 2, 9, 610, 0, 0, ""},
            {1210, 2, "Genesis Pillar", "Senjata pamungkas untuk sang Archon.", 0, 150000, 2, 10, 750, 0, 0, ""},

            {1211, 3, "Cotton Hood", "Penutup kepala kain pelindung ringan.", 350, 200, 2, 1, 0, 50, 2, ""},
            {1212, 3, "Silk Circlet", "Lingkar kepala sutra penguat mana.", 0, 450, 2, 2, 0, 125, 4, ""},
            {1213, 3, "Mage’s Cowl", "Tudung kepala penyihir standar.", 1800, 1000, 2, 3, 0, 250, 7, ""},
            {1214, 3, "Sage’s Veil", "Kerudung kebijaksanaan orang bijak.", 0, 2200, 2, 4, 0, 450, 11, ""},
            {1215, 3, "Wizard’s Tricorn", "Topi bersudut tiga khas penyihir agung.", 7000, 4500, 2, 5, 0, 700, 16, ""},
            {1216, 3, "Cinder Hood", "Tudung bara api.", 0, 9000, 2, 6, 0, 1000, 23, ""},
            {1217, 3, "Tiara of Insight", "Mahkota wawasan sihir tertinggi.", 0, 18000, 2, 7, 0, 1400, 32, ""},
            {1218, 3, "Diadem of Stars", "Mahkota bertabur kekuatan bintang.", 0, 35000, 2, 8, 0, 1900, 43, ""},
            {1219, 3, "Crown of the Void", "Mahkota kehampaan.", 0, 60000, 2, 9, 0, 2750, 57, ""},
            {1220, 3, "Astral Halo", "Lingkaran cahaya astral dewa sihir.", 0, 120000, 2, 10, 0, 4500, 75, ""},

            {1221, 4, "Novice Robe", "Jubah pemula sihir.", 700, 400, 2, 1, 0, 100, 4, ""},
            {1222, 4, "Scholar’s Tunic", "Tunik para pelajar ilmu sihir.", 0, 900, 2, 2, 0, 250, 8, ""},
            {1223, 4, "Arcane Garb", "Pakaian misterius berlapis sihir.", 3500, 2000, 2, 3, 0, 500, 14, ""},
            {1224, 4, "Mystic Vestment", "Jubah upacara mistis penyihir.", 0, 4500, 2, 4, 0, 900, 22, ""},
            {1225, 4, "Archmage’s Cloak", "Jubah kebesaran Archmage istana.", 13000, 8500, 2, 5, 0, 1400, 32, ""},
            {1226, 4, "Pyromancer’s Mantle", "Mantel pengendali api.", 0, 18000, 2, 6, 0, 2000, 46, ""},
            {1227, 4, "Elder Root Robe", "Jubah dari akar pohon kebijaksanaan.", 0, 35000, 2, 7, 0, 2800, 64, ""},
            {1228, 4, "Nebula Gown", "Gaun tenunan kosmos dan rasi bintang.", 0, 65000, 2, 8, 0, 3800, 86, ""},
            {1229, 4, "Voidshaper Shroud", "Selubung pembentuk kehampaan.", 0, 110000, 2, 9, 0, 5500, 114, ""},
            {1230, 4, "Eternity Raiment", "Pakaian keabadian sihir kosmik.", 0, 250000, 2, 10, 0, 8000, 150, ""},

            {1231, 5, "Cloth Slippers", "Sandal kain ringan.", 250, 150, 2, 1, 0, 50, 2, ""},
            {1232, 5, "Linen Shoes", "Sepatu linen untuk mobilitas.", 0, 350, 2, 2, 0, 125, 4, ""},
            {1233, 5, "Mage’s Sandals", "Sandal khusus para penyihir.", 1200, 750, 2, 3, 0, 250, 7, ""},
            {1234, 5, "Enchanter’s Boots", "Sepatu bot para pemantra.", 0, 1500, 2, 4, 0, 450, 11, ""},
            {1235, 5, "Sorcerer’s Stride", "Sepatu bot langkah sorcerer.", 5000, 3000, 2, 5, 0, 700, 16, ""},
            {1236, 5, "Flame-Walk Shoes", "Sepatu pijakan api.", 0, 6500, 2, 6, 0, 1000, 23, ""},
            {1237, 5, "Ley-Line Greaves", "Pelindung kaki pengumpul energi leylines.", 0, 12000, 2, 7, 0, 1400, 32, ""},
            {1238, 5, "Phase Boots", "Sepatu bergeser dimensi.", 0, 22000, 2, 8, 0, 1900, 43, ""},
            {1239, 5, "Abyssal Walkers", "Langkah menyusuri jurang kegelapan.", 0, 45000, 2, 9, 0, 2750, 57, ""},
            {1240, 5, "God-Stepper Soles", "Sol kaki penapak tingkat dewa.", 0, 90000, 2, 10, 0, 4500, 75, ""},

            // ================= WANDERER EQUIPMENT (ID: 1301-1340) =================
            {1301, 2, "Short Bow", "Busur kayu ringan untuk berburu.", 550, 250, 3, 1, 15, 0, 0, ""},
            {1302, 2, "Hunter's Crossbow", "Crossbow standar.", 0, 600, 3, 2, 35, 0, 0, ""},
            {1303, 2, "Composite Bow", "Busur campuran kayu dan tulang.", 2800, 1500, 3, 3, 65, 0, 0, ""},
            {1304, 2, "Heavy Arbalest", "Crossbow berat dengan mekanisme besi.", 0, 3000, 3, 4, 110, 0, 0, ""},
            {1305, 2, "Ranger's Longbow", "Busur panjang standar pasukan pengintai.", 11000, 6000, 3, 5, 175, 0, 0, ""},
            {1306, 2, "Flamestrike Bow", "Busur yang talinya terbuat dari serat api.", 0, 12000, 3, 6, 260, 0, 0, ""},
            {1307, 2, "Verdant Crossbow", "Crossbow organik dari kayu.", 0, 25000, 3, 7, 365, 0, 0, ""},
            {1308, 2, "Eagle-Eye Piercer", "Busur legendaris dengan akurasi mutlak.", 0, 45000, 3, 8, 485, 0, 0, ""},
            {1309, 2, "Void-Stinger", "Crossbow yang menembakkan energi hampa.", 0, 80000, 3, 9, 610, 0, 0, ""},
            {1310, 2, "Final Nightfall", "Senjata pamungkas untuk sang Nightfall.", 0, 150000, 3, 10, 750, 0, 0, ""},

            {1311, 3, "Cloth Bandana", "Bandana kain ikat kepala.", 380, 200, 3, 1, 0, 75, 3, ""},
            {1312, 3, "Leather Cap", "Topi kulit ringan.", 0, 450, 3, 2, 0, 175, 6, ""},
            {1313, 3, "Scout Hood", "Tudung pengintai.", 1900, 1000, 3, 3, 0, 350, 11, ""},
            {1314, 3, "Reinforced Coif", "Tudung pelindung yang diperkuat.", 0, 2200, 3, 4, 0, 600, 18, ""},
            {1315, 3, "Tracker’s Mask", "Topeng penyamar pelacak tapak.", 7500, 4500, 3, 5, 0, 950, 28, ""},
            {1316, 3, "Cinder Hood", "Tudung penjaga bayangan api.", 0, 9000, 3, 6, 0, 1400, 40, ""},
            {1317, 3, "Silent Nightcap", "Tudung malam sunyi.", 0, 18000, 3, 7, 0, 2000, 55, ""},
            {1318, 3, "Assassin’s Visage", "Topeng wajah sang pembunuh.", 0, 35000, 3, 8, 0, 2750, 74, ""},
            {1319, 3, "Specter’s Gaze", "Tatapan sang hantu.", 0, 60000, 3, 9, 0, 3900, 96, ""},
            {1320, 3, "Shroud of Nightfall", "Selubung pelindung jatuhnya malam.", 0, 120000, 3, 10, 0, 6000, 125, ""},

            {1321, 4, "Rough Tunic", "Tunik kasar untuk bergerak lincah.", 750, 400, 3, 1, 0, 150, 6, ""},
            {1322, 4, "Leather Jerkin", "Rompi kulit tanpa lengan.", 0, 900, 3, 2, 0, 350, 12, ""},
            {1323, 4, "Studded Vest", "Rompi bertatahkan logam pelindung.", 3800, 2000, 3, 3, 0, 700, 22, ""},
            {1324, 4, "Ranger’s Garb", "Pakaian penjelajah alam liar.", 0, 4500, 3, 4, 0, 1200, 36, ""},
            {1325, 4, "Duelist’s Leather", "Zirah kulit ringan khusus pertarungan.", 14000, 8500, 3, 5, 0, 1900, 56, ""},
            {1326, 4, "Salamander Hide", "Pakaian dari kulit salamander tahan panas.", 0, 18000, 3, 6, 0, 2800, 80, ""},
            {1327, 4, "Nature’s Embrace", "Zirah pelukan alam terbuka.", 0, 35000, 3, 7, 0, 4000, 110, ""},
            {1328, 4, "Mistwalker Tunic", "Tunik pejalan kabut putih.", 0, 65000, 3, 8, 0, 5500, 148, ""},
            {1329, 4, "Abyssal Stealth-Suit", "Baju penyamaran stealth di jurang dalam.", 0, 110000, 3, 9, 0, 7800, 192, ""},
            {1330, 4, "Eternal Night-Skin", "Kulit keabadian malam.", 0, 250000, 3, 10, 0, 11000, 250, ""},

            {1331, 5, "Soft Sandals", "Sandal empuk untuk mengendap.", 280, 150, 3, 1, 0, 75, 3, ""},
            {1332, 5, "Leather Boots", "Sepatu kulit dasar petualang.", 0, 350, 3, 2, 0, 175, 6, ""},
            {1333, 5, "Padded Shoes", "Sepatu tebal anti selip.", 1300, 750, 3, 3, 0, 350, 11, ""},
            {1334, 5, "Scout Greaves", "Pelindung kaki ringan pengintai.", 0, 1500, 3, 4, 0, 600, 18, ""},
            {1335, 5, "Silent Striders", "Sepatu langkah tanpa suara.", 5500, 3000, 3, 5, 0, 950, 28, ""},
            {1336, 5, "Magma-Step Boots", "Sepatu bot loncat magma.", 0, 6500, 3, 6, 0, 1400, 40, ""},
            {1337, 5, "Wind-Runner Boots", "Sepatu lari penunggang angin.", 0, 12000, 3, 7, 0, 2000, 55, ""},
            {1338, 5, "Shadow-Step Treads", "Alas kaki langkah bayangan.", 0, 22000, 3, 8, 0, 2750, 74, ""},
            {1339, 5, "Ghost-Walkers", "Sepatu pejalan tembus rintangan.", 0, 45000, 3, 9, 0, 3900, 96, ""},
            {1340, 5, "Nightfall’s Path", "Alas penyusur jalan sang Nightfall.", 0, 90000, 3, 10, 0, 6000, 125, ""},

            // ================= ACCESSORY (ID: 1401-1410) =================
            {1401, 6, "Iron Ring", "Aksesoris penambah STR", 0, 2000, 0, 1, 0, 0, 0, "STR_+10%"},
            {1402, 6, "Glass Monocle", "Aksesoris penambah INT", 0, 2000, 0, 2, 0, 0, 0, "INT_+10%"},
            {1403, 6, "Feather Charm", "Aksesoris penambah AGI", 0, 2000, 0, 3, 0, 0, 0, "AGI_+10%"},
            {1404, 6, "Vitality Band", "Aksesoris penambah persentase HP", 0, 4500, 0, 4, 0, 0, 0, "MAXHP_+15%"},
            {1405, 6, "Mana Crystal", "Aksesoris penambah persentase MP", 0, 5000, 0, 5, 0, 0, 0, "MAXMP_+20%"},
            {1406, 6, "Warrior’s Emblem", "Aksesoris peningkatan Damage", 0, 15000, 0, 6, 0, 0, 0, "DMG_+8%"},
            {1407, 6, "Steel Gauntlet", "Aksesoris peningkatan Defense", 0, 20000, 0, 7, 0, 0, 0, "DEF_+15%"},
            {1408, 6, "Medallion of Life", "Aksesoris regenerasi HP", 0, 35000, 0, 8, 0, 0, 0, "HPREGEN_5%"},
            {1409, 6, "Silver Rosary", "Aksesoris peningkatan efek penyembuh", 0, 35000, 0, 9, 0, 0, 0, "POTION_+20%"},
            {1410, 6, "Void Pendulum", "Aksesoris manipulasi waktu (Cooldown)", 0, 100000, 0, 10, 0, 0, 0, "CD_-1"},

            // ================= BOSS REWARD ORBS (ID: 1411-1415) =================
            {1411, 6, "Crimson Orb", "Simbol keberanian dan kekuatan fisik dari The Ashen Warlord.", 0, 150000, 1, 10, 1500, 0, 0, "STR_+20%"},
            {1412, 6, "Azure Orb", "Simbol kecerdasan dan mana tak terbatas dari The Grand Archivist.", 0, 150000, 2, 10, 0, 0, 0, "MAXMP_+50%"},
            {1413, 6, "Obsidian Orb", "Simbol kelincahan dan misteri dari The Phantom Stalker.", 0, 150000, 3, 10, 500, 0, 0, "AGI_+20%"},
            {1414, 6, "Emerald Orb", "Simbol ketahanan dan keabadian alam dari The Rotting Behemoth.", 0, 150000, 0, 10, 0, 8000, 800, "MAXHP_+30%"},
            {1415, 6, "Radiant Orb", "Simbol takdir dan kesatuan jiwa dari The Oracle of Ruin.", 0, 250000, 0, 10, 1000, 2000, 200, "CD_-1"}

        };
    }

    ItemTemplate getItemTemplate(int id) {
        auto items = getAllItemTemplates();
        for (const auto& item : items) {
            if (item.id == id) return item;
        }
        return {0, 0, "Kosong", "Tidak ada item", 0, 0, 0, 0, 0, 0, 0, ""};
    }

    ItemTemplate getItemTemplateByName(const std::string& name) {
        auto items = getAllItemTemplates();
        for (const auto& item : items) {
            if (item.name == name) return item;
        }
        return {0, 0, "Kosong", "Tidak ada item", 0, 0, 0, 0, 0, 0, 0, ""};
    }

    std::string getCategoryName(int categoryId) {
        switch(categoryId) {
            case 1: return "Consumable";
            case 2: return "Weapon";
            case 3: return "Helmet";
            case 4: return "Armor";
            case 5: return "Boots";
            case 6: return "Accessory";
            case 7: return "Material";
            default: return "Unknown";
        }
    }

}