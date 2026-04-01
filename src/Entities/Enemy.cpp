#include "Entities/Enemy.h"
#include <algorithm>
#include <utility>

using namespace std;
namespace Entities {

    vector<EnemyTemplate> getAllEnemyTemplates() {
        return {
            {1, "Lesser Imp", "Demon", 1, 10, 15, 45, 110, 380, 5, 15, 4, 40, 0.04f, 1, 2, false},
            {2, "Shadow Lurker", "Demon", 11, 20, 50, 85, 420, 900, 18, 35, 44, 80, 0.06f, 3, 4, false},
            {3, "Horned Ravager", "Demon", 21, 30, 90, 145, 950, 1700, 40, 65, 84, 120, 0.08f, 5, 6, false},
            {4, "Hellbound Hound", "Demon", 31, 40, 155, 220, 1900, 3400, 70, 100, 124, 160, 0.11f, 7, 8, false},
            {5, "Gorgon Guard", "Demon", 41, 50, 230, 315, 3800, 6000, 115, 160, 164, 200, 0.14f, 9, 10, false},
            {6, "Abyssal Brute", "Demon", 51, 60, 330, 430, 6500, 10500, 170, 220, 204, 240, 0.16f, 11, 12, false},
            {7, "Chaos Weaver", "Demon", 61, 70, 450, 580, 11000, 16500, 230, 300, 244, 280, 0.18f, 13, 14, false},
            {8, "Demon Commander", "Demon", 71, 80, 600, 750, 18000, 26000, 320, 410, 284, 320, 0.21f, 15, 16, false},
            {9, "Dread Overlord", "Demon", 81, 90, 780, 1000, 28000, 43000, 420, 580, 324, 360, 0.24f, 17, 19, false},
            {10, "Harbinger of Doom", "Demon", 91, 100, 1050, 1400, 48000, 70000, 600, 850, 364, 400, 0.26f, 20, 20, false},
            
            {11, "Lost Soul", "Spirit", 1, 10, 20, 50, 80, 300, 3, 10, 10, 100, 0.10f, 1, 2, false},
            {12, "Wailing Echo", "Spirit", 11, 20, 55, 95, 350, 750, 12, 25, 110, 200, 0.15f, 3, 4, false},
            {13, "Vengeful Mist", "Spirit", 21, 30, 100, 155, 800, 1400, 30, 50, 210, 300, 0.20f, 5, 6, false},
            {14, "Spectral Monk", "Spirit", 31, 40, 165, 230, 1600, 2800, 55, 80, 310, 400, 0.25f, 7, 8, false},
            {15, "Phantom Assassin", "Spirit", 41, 50, 250, 330, 3000, 4800, 90, 120, 410, 500, 0.30f, 9, 10, false},
            {16, "Cursed Revenant", "Spirit", 51, 60, 350, 450, 5200, 8200, 130, 170, 510, 600, 0.35f, 11, 12, false},
            {17, "Ancestral Wraith", "Spirit", 61, 70, 470, 600, 8800, 13000, 180, 230, 610, 700, 0.40f, 13, 14, false},
            {18, "Banshee Queen", "Spirit", 71, 80, 630, 780, 14000, 21000, 250, 320, 710, 800, 0.45f, 15, 16, false},
            {19, "Soul Eater", "Spirit", 81, 90, 820, 1050, 23000, 35000, 350, 480, 810, 900, 0.50f, 17, 19, false},
            {20, "Eternal Phantasm", "Spirit", 91, 100, 1100, 1450, 40000, 58000, 520, 750, 910, 1000, 0.55f, 20, 20, false},
            
            {21, "Wild Fang", "Beast", 1, 10, 10, 35, 150, 450, 2, 8, 0, 0, 0.04f, 1, 2, false},
            {22, "Cave Crawler", "Beast", 11, 20, 40, 75, 550, 1100, 8, 20, 0, 0, 0.06f, 3, 4, false},
            {23, "Giant Rock-Boar", "Beast", 21, 30, 80, 130, 1200, 2200, 25, 45, 0, 0, 0.08f, 5, 6, false},
            {24, "Thornback Bear", "Beast", 31, 40, 140, 200, 2500, 4500, 40, 70, 0, 0, 0.11f, 7, 8, false},
            {25, "Savage Direwolf", "Beast", 41, 50, 210, 280, 5000, 7500, 75, 100, 0, 0, 0.14f, 9, 10, false},
            {26, "Elder Basilisk", "Beast", 51, 60, 300, 390, 8500, 13000, 110, 150, 0, 0, 0.16f, 11, 12, false},
            {27, "Iron-Claw Raptor", "Beast", 61, 70, 410, 520, 14500, 21000, 160, 210, 0, 0, 0.18f, 13, 14, false},
            {28, "Thunder Mammoth", "Beast", 71, 80, 550, 700, 23000, 35000, 220, 290, 0, 0, 0.21f, 15, 16, false},
            {29, "Ancient Behemoth", "Beast", 81, 90, 730, 920, 38000, 55000, 310, 420, 0, 0, 0.24f, 17, 19, false},
            {30, "Apex Predator", "Beast", 91, 100, 950, 1250, 60000, 85000, 450, 650, 0, 0, 0.26f, 20, 20, false},
            
            {31, "Living Pebble", "Elemental", 1, 10, 12, 40, 80, 250, 10, 25, 8, 80, 0.04f, 1, 2, false},
            {32, "Flame Sprite", "Elemental", 11, 20, 45, 80, 300, 650, 20, 45, 88, 160, 0.06f, 3, 4, false},
            {33, "Mud Golem", "Elemental", 21, 30, 85, 135, 700, 1200, 50, 85, 168, 240, 0.08f, 5, 6, false},
            {34, "Crystal Shardling", "Elemental", 31, 40, 145, 205, 1400, 2400, 90, 130, 248, 320, 0.11f, 7, 8, false},
            {35, "Static Cloud", "Elemental", 41, 50, 215, 290, 2600, 4200, 120, 170, 328, 400, 0.14f, 9, 10, false},
            {36, "Molten Core", "Elemental", 51, 60, 310, 400, 4500, 7000, 180, 250, 408, 480, 0.16f, 11, 12, false},
            {37, "Glacier Sentinel", "Elemental", 61, 70, 420, 530, 7500, 11500, 270, 360, 488, 560, 0.18f, 13, 14, false},
            {38, "Zephyr Spirit", "Elemental", 71, 80, 560, 710, 12500, 18000, 350, 450, 568, 640, 0.21f, 15, 16, false},
            {39, "Magma Colossus", "Elemental", 81, 90, 740, 940, 20000, 30000, 500, 680, 648, 720, 0.24f, 17, 19, false},
            {40, "Ancient Monolith", "Elemental", 91, 100, 1000, 1300, 35000, 50000, 750, 1000, 728, 800, 0.26f, 20, 20, false},
            
            {41, "Novice Thief", "Mercenary", 1, 10, 18, 42, 120, 380, 4, 12, 4, 40, 0.10f, 1, 2, false},
            {42, "Exiled Slinger", "Mercenary", 11, 20, 48, 82, 450, 900, 15, 30, 44, 80, 0.15f, 3, 4, false},
            {43, "Rogue Guard", "Mercenary", 21, 30, 85, 135, 1000, 1800, 35, 60, 84, 120, 0.20f, 5, 6, false},
            {44, "Desert Marauder", "Mercenary", 31, 40, 155, 220, 2000, 3500, 65, 95, 124, 160, 0.25f, 7, 8, false},
            {45, "Fallen Paladin", "Mercenary", 41, 50, 230, 310, 3800, 6000, 100, 140, 164, 200, 0.30f, 9, 10, false},
            {46, "Arcane Assassin", "Mercenary", 51, 60, 330, 430, 6500, 10000, 150, 190, 204, 240, 0.35f, 11, 12, false},
            {47, "Veteran Gladiator", "Mercenary", 61, 70, 450, 580, 11000, 17000, 200, 260, 244, 280, 0.40f, 13, 14, false},
            {48, "Void Cultist", "Mercenary", 71, 80, 600, 750, 18000, 28000, 280, 360, 284, 320, 0.45f, 15, 16, false},
            {49, "Elite Sellsword", "Mercenary", 81, 90, 780, 1000, 30000, 45000, 380, 520, 324, 360, 0.50f, 17, 19, false},
            {50, "Grand Mercenary", "Mercenary", 91, 100, 1050, 1400, 50000, 72000, 550, 800, 364, 400, 0.55f, 20, 20, false},
            
            {51, "The Ashen Warlord", "Demon", 91, 100, 2500, 3200, 250000, 350000, 1500, 1800, 364, 400, 0.15f, 21, 21, true},
            {52, "The Grand Archivist", "Mercenary", 91, 100, 3500, 4200, 180000, 240000, 1200, 1400, 364, 400, 0.15f, 21, 21, true},
            {53, "The Phantom Stalker", "Spirit", 91, 100, 3800, 4500, 150000, 200000, 900, 1100, 910, 1000, 0.15f, 21, 21, true},
            {54, "The Rotting Behemoth", "Beast", 91, 100, 2200, 2800, 450000, 600000, 1300, 1600, 0, 0, 0.15f, 21, 21, true},
            {55, "The Oracle of Ruin", "Elemental", 91, 100, 3200, 4000, 220000, 300000, 2000, 2500, 728, 800, 0.15f, 21, 21, true},
            {56, "The Eclipse Vanguard", "Void (Demon)", 91, 100, 4500, 5200, 380000, 450000, 2000, 2300, 910, 1000, 0.20f, 21, 21, true},
            {57, "Architect of Ruin", "Void (Spirit)", 91, 100, 5000, 5800, 420000, 500000, 2400, 2600, 1365, 1500, 0.20f, 21, 21, true},
            {58, "The Void (Form 1)", "Void (Elemental)", 91, 100, 5800, 6500, 550000, 650000, 2800, 3200, 1365, 1500, 0.20f, 21, 21, true},
            {59, "The Void (Form 2)", "Void (Beast)", 91, 100, 6500, 7500, 750000, 900000, 3500, 4200, 0, 0, 0.20f, 21, 21, true},
            {60, "The Void (Final Form)", "Void (Demon)", 91, 100, 8500, 10000, 1200000, 1500000, 5000, 6000, 9999, 9999, 0.30f, 21, 21, true}
        };
    }

    vector<EnemyTemplate> getEnemiesByDungeonAndDepth(int dungeonId, int depth) {
        vector<EnemyTemplate> allEnemies = getAllEnemyTemplates();
        vector<EnemyTemplate> validEnemies;

        // Boss logic at depth 20
        if (depth == 20) {
            int bossId = 50 + dungeonId;
            for (const auto& enemy : allEnemies) {
                if (enemy.enemyId == bossId) {
                    validEnemies.push_back(enemy);
                    return validEnemies;
                }
            }
        }

        // Minion logic filtering by dungeon type
        for (const auto& enemy : allEnemies) {
            if (!enemy.isBoss && depth >= enemy.spawnDepthStart && depth <= enemy.spawnDepthEnd) {
                bool match = false;
                
                if (dungeonId == 1 && enemy.type == "Demon") match = true;
                else if (dungeonId == 2 && enemy.type == "Spirit") match = true;
                else if (dungeonId == 3 && enemy.type == "Beast") match = true;
                else if (dungeonId == 4 && enemy.type == "Elemental") match = true;
                else if (dungeonId == 5 && enemy.type == "Mercenary") match = true;

                if (match) {
                    validEnemies.push_back(enemy);
                }
            }
        }

        // Fallback if no enemies match the exact level/depth criteria
        if (validEnemies.empty()) {
            for (const auto& enemy : allEnemies) {
                if (!enemy.isBoss && depth >= enemy.spawnDepthStart && depth <= enemy.spawnDepthEnd) {
                    validEnemies.push_back(enemy);
                }
            }
        }

        return validEnemies;
    }

    vector<EnemyDrop> getEnemyDrops(int enemyId) {
        static const vector<pair<int, int>> dropMap = {
            {1,2},{1,1},{1,4}, {2,4},{2,2},{2,6}, {3,2},{3,6},{3,1}, {4,6},{4,4},{4,2},
            {5,1},{5,2},{5,4}, {6,2},{6,4},{6,6}, {7,4},{7,6},{7,2}, {8,6},{8,2},{8,4},
            {9,2},{9,4},{9,6}, {10,2},{10,6},{10,4}, {11,4},{11,1},{11,6}, {12,4},{12,2},{12,1},
            {13,6},{13,4},{13,2}, {14,2},{14,6},{14,4}, {15,6},{15,4},{15,1}, {16,2},{16,4},{16,6},
            {17,4},{17,6},{17,2}, {18,6},{18,2},{18,4}, {19,2},{19,4},{19,6}, {20,4},{20,6},{20,2},
            {21,6},{21,1},{21,2}, {22,2},{22,6},{22,1}, {23,2},{23,4},{23,6}, {24,6},{24,4},{24,2},
            {25,6},{25,1},{25,4}, {26,4},{26,2},{26,6}, {27,2},{27,6},{27,4}, {28,4},{28,2},{28,6},
            {29,2},{29,4},{29,6}, {30,6},{30,2},{30,4}, {31,2},{31,1},{31,4}, {32,4},{32,6},{32,1},
            {33,2},{33,4},{33,6}, {34,4},{34,1},{34,6}, {35,6},{35,4},{35,2}, {36,2},{36,4},{36,1},
            {37,2},{37,6},{37,4}, {38,6},{38,4},{38,2}, {39,2},{39,4},{39,6}, {40,2},{40,4},{40,6},
            {41,1},{41,6},{41,4}, {42,6},{42,1},{42,2}, {43,2},{43,1},{43,4}, {44,1},{44,6},{44,2},
            {45,2},{45,4},{45,6}, {46,4},{46,6},{46,1}, {47,2},{47,1},{47,6}, {48,4},{48,2},{48,6},
            {49,1},{49,6},{49,2}, {50,2},{50,4},{50,6}, {51,2},{51,1},{51,6}, {52,4},{52,1},{52,2},
            {53,6},{53,4},{53,1}, {54,2},{54,6},{54,4}, {55,4},{55,2},{55,6}, {56,8},{56,2},{56,1},
            {57,8},{57,4},{57,1}, {58,8},{58,6},{58,1}, {59,8},{59,2},{59,4}, {60,8},{60,1},{60,2},{60,4},{60,6}
        };
        
        vector<EnemyDrop> results;
        for (const auto& pair : dropMap) {
            if (pair.first == enemyId) {
                results.push_back({pair.first, pair.second});
            }
        }
        return results;
    }

    DungeonExp getDungeonDepthExp(int depth) {
        static const vector<DungeonExp> depthExpMap = {
            {0, 0},
            {48, 52},       {114, 126},     {238, 262},     {428, 472},     {665, 735},
            {998, 1102},    {1425, 1575},   {1995, 2205},   {2660, 2940},   {3515, 3885},
            {4560, 5040},   {5795, 6405},   {7220, 7980},   {8930, 9870},   {10925, 12075},
            {13300, 14700}, {16150, 17850}, {19475, 21525}, {23275, 25725}, {28500, 31500}
        };
        
        if (depth < 1) return depthExpMap[1];
        if (depth > 20) return depthExpMap[20];
        return depthExpMap[depth];
    }

    Enemy::Enemy() : id(0), name(""), type(""), level(1), hp(0), maxHp(0), mp(0), maxMp(0), atk(0), def(0), critRate(0.0f), isBoss(false) {}

    void Enemy::loadFromTemplate(const EnemyTemplate& tmpl, int targetLevel) {
        id = tmpl.enemyId;
        name = tmpl.name;
        type = tmpl.type;
        isBoss = tmpl.isBoss;
        critRate = tmpl.critRate;
        
        level = max(tmpl.minLevel, min(targetLevel, tmpl.maxLevel));

        float scale = 0.0f;
        if (tmpl.maxLevel > tmpl.minLevel) {
            scale = static_cast<float>(level - tmpl.minLevel) / (tmpl.maxLevel - tmpl.minLevel);
        }

        maxHp = tmpl.minHp + static_cast<int>((tmpl.maxHp - tmpl.minHp) * scale);
        hp = maxHp;
        
        maxMp = tmpl.minMp + static_cast<int>((tmpl.maxMp - tmpl.minMp) * scale);
        mp = maxMp;
        
        atk = tmpl.minAtk + static_cast<int>((tmpl.maxAtk - tmpl.minAtk) * scale);
        def = tmpl.minDef + static_cast<int>((tmpl.maxDef - tmpl.minDef) * scale);
    }

    void Enemy::takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    bool Enemy::isDead() const {
        return hp <= 0;
    }
}