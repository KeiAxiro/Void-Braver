#pragma once
#include <string>
#include <vector>

namespace Entities {

    struct EnemyDrop {
        int enemyId;
        int itemId; 
    };

    struct EnemyTemplate {
        int enemyId;
        std::string name;
        std::string type;
        int minLevel; int maxLevel;
        int minAtk; int maxAtk;
        int minHp; int maxHp;
        int minDef; int maxDef;
        int minMp; int maxMp;
        float critRate;
        int spawnDepthStart; int spawnDepthEnd;
        bool isBoss;
    };

    struct DungeonExp {
        int minExp;
        int maxExp;
    };

    std::vector<EnemyTemplate> getAllEnemyTemplates();
    std::vector<EnemyTemplate> getEnemiesByDepth(int depth);
    std::vector<EnemyTemplate> getEnemiesByDungeonAndDepth(int dungeonId, int depth);
    std::vector<EnemyDrop> getEnemyDrops(int enemyId);
    DungeonExp getDungeonDepthExp(int depth);

    class Enemy {
    public:
        int id;
        std::string name;
        std::string type;
        int level;
        int hp;
        int maxHp;
        int mp;
        int maxMp;
        int atk;
        int def;
        float critRate;
        bool isBoss;

        Enemy();
        
        void loadFromTemplate(const EnemyTemplate& tmpl, int targetLevel);
        void takeDamage(int amount);
        bool isDead() const;
    };
}