#pragma once
#include <string>
#include <vector>

namespace Entities {
    struct Equipment {
        std::string name;
        std::string slot; 
        int atkBonus;
        int defBonus;
        int hpBonus;
    };

struct Skill {
        std::string name;
        std::string description;
        int requiredLevel;
        int manaCost;
        float damageMultiplier;
        int maxCooldown;
        int currentCd;
        bool isUnlocked;
    };

    struct Item {
        std::string name;
        std::string type; 
        int quantity;
    };

    Equipment getEquipmentData(const std::string& name);

    class Player {
    public:
        std::string name;
        std::string job;
        int level;
        int exp;
        int hp;
        int mp;
        int statPoints; 
        int str, intel, agi, vit;

        Equipment weapon, helmet, armor, boots, accessory;
        std::vector<Skill> skills;
        std::vector<Item> inventory;
        std::vector<int> unlockedDepths = {1, 1, 1, 1, 1, 1};

        Player();

        int getTotalEquipAtkBase() const;
        int getTotalEquipDef() const;
        int getTotalEquipHp() const;
        int getPrimaryStat() const;
        float getWeaponEfficiency() const;
        int getSynergizedWeaponAtk() const;
        int getMaxHp() const;
        int getMaxMp() const;
        int getDefense() const;
        int getAtkPower() const;
        float getCritRate() const;
        int getNextLevelExp() const;

        void addExp(int amount);
        void addItemToInventory(const std::string& itemName, const std::string& itemType);
        void updateSkills();
    };
}