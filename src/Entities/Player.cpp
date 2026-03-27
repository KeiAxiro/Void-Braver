#include "Entities/Player.h"
#include "Entities/Item.h" 
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

namespace Entities {

    Equipment getEquipmentData(const std::string& name) {
        if (name == "empty" || name == "Kosong") return {"empty", "None", 0, 0, 0};
        
        ItemTemplate item = getItemTemplateByName(name);
        if (item.name != "Kosong") {
            std::string slotName = getCategoryName(item.categoryId);
            return {item.name, slotName, item.atkBonus, item.defBonus, item.hpBonus};
        }
        
        return {"empty", "None", 0, 0, 0};
    }

    Player::Player() : name(""), job(""), level(1), exp(0), statPoints(0), str(0), intel(0), agi(0), vit(0), hp(100), mp(50) {
        weapon = {"empty", "Weapon", 0, 0, 0};
        helmet = {"empty", "Helmet", 0, 0, 0};
        armor = {"empty", "Armor", 0, 0, 0};
        boots = {"empty", "Boots", 0, 0, 0};
        accessory = {"empty", "Accessory", 0, 0, 0};
    }

    int Player::getTotalEquipAtkBase() const { return weapon.atkBonus + accessory.atkBonus; }
    int Player::getTotalEquipDef() const { return helmet.defBonus + armor.defBonus + boots.defBonus + accessory.defBonus; }
    int Player::getTotalEquipHp() const { return helmet.hpBonus + armor.hpBonus + boots.hpBonus + accessory.hpBonus; }

    int Player::getPrimaryStat() const {
        if (job == "Knight") return str;
        if (job == "Sorcerer") return intel;
        return agi; 
    }

    float Player::getWeaponEfficiency() const { return 1.0f + (getPrimaryStat() / 300.0f); }
    int Player::getSynergizedWeaponAtk() const { return static_cast<int>(getTotalEquipAtkBase() * getWeaponEfficiency()); }

    int Player::getMaxHp() const { 
        int baseHpGrowth = static_cast<int>(std::pow(level, 2) * 2.5f);
        int vitGrowth = static_cast<int>(vit * level * 2.0f);
        return 100 + baseHpGrowth + vitGrowth + getTotalEquipHp(); 
    }
    
    int Player::getMaxMp() const { return 20 + (intel * 10); }
    
    int Player::getDefense() const { 
        float vitMultiplier = (job == "Knight") ? 3.0f : (job == "Wanderer") ? 2.0f : 1.0f;
        return (level * 5) + static_cast<int>(vit * vitMultiplier) + getTotalEquipDef(); 
    }
    
    int Player::getAtkPower() const {
        float multiplier = 1.0f;
        float strWeight = 0, intWeight = 0, agiWeight = 0;
        
        int effectiveAgi = agi;
        int excessAgi = 0;
        int effectiveInt = intel;
        int excessInt = 0;
        
        float overcapMultiplier = (level <= 50) ? 2.0f : 6.0f; 

        if (job == "Knight") {
            multiplier = 2.0f + (level * 0.05f); 
            strWeight = 1.0f; intWeight = 0.2f; agiWeight = 0.4f;
        } else if (job == "Sorcerer") {
            multiplier = 1.5f + (level * 0.04f);
            strWeight = 0.2f; intWeight = 1.0f; agiWeight = 0.3f;
            int intCap = 260;
            effectiveInt = std::min(intel, intCap);
            excessInt = std::max(0, intel - intCap);
        } else if (job == "Wanderer") {
            multiplier = 1.0f + (level * 0.03f);
            strWeight = 0.4f; intWeight = 0.2f; agiWeight = 1.0f;
            int agiCap = 260;
            effectiveAgi = std::min(agi, agiCap);
            excessAgi = std::max(0, agi - agiCap);
        }
        
        float totalStatValue = (str * strWeight) + (effectiveInt * intWeight) + (effectiveAgi * agiWeight);
        int overcapBonus = static_cast<int>(excessAgi * overcapMultiplier) + static_cast<int>(excessInt * overcapMultiplier);
        int pureStatPower = static_cast<int>(totalStatValue * multiplier) + overcapBonus;
        return pureStatPower + getSynergizedWeaponAtk();
    }

    float Player::getCritRate() const {
        float rate = static_cast<float>(agi) / (agi + 60.0f) * 0.8f;
        return std::min(0.65f, rate); 
    }

    void Player::addItemToInventory(const std::string& itemName, const std::string& itemType) {
        if (itemName == "empty") return;
        for (auto& item : inventory) {
            if (item.name == itemName) {
                item.quantity++;
                return;
            }
        }
        inventory.push_back({itemName, itemType, 1});
    }

    int Player::getNextLevelExp() const {
        static const int levelExpTable[101] = {
            0, 
            100, 348, 722, 1212, 1811, 2514, 3315, 4211, 5197, 6309,
            7458, 8711, 10050, 11470, 12970, 14560, 16230, 17980, 19800, 21700,
            23680, 25740, 27880, 30100, 32300, 34700, 37100, 39600, 42100, 44800,
            47700, 50800, 53900, 57100, 60500, 63600, 66800, 70000, 73400, 76800,
            80200, 83700, 87300, 91000, 94700, 98400, 102300, 106100, 110100, 114100,
            118200, 122300, 126500, 130800, 135100, 139500, 143900, 148400, 152900, 157500,
            162200, 166900, 171600, 176500, 181300, 186300, 191300, 196400, 201700, 206900,
            212100, 217400, 222800, 228200, 233700, 239300, 244900, 250500, 256300, 262000,
            267900, 273700, 279700, 285700, 291700, 297800, 303900, 310100, 316300, 322600,
            328900, 335300, 341700, 348100, 354700, 361200, 367900, 374500, 381200, 398100
        };

        if (level < 1) return levelExpTable[1];
        if (level >= 100) return 0;
        return levelExpTable[level];
    }

    void Player::addExp(int amount) {
        exp += amount;
        int initialLevel = level;
        
        while (exp >= getNextLevelExp() && level < 100) { 
            exp -= getNextLevelExp();
            level++;
            statPoints += 5;
        }

        int levelsGained = level - initialLevel;

        std::cout << "\n " << std::string(52, '=') << "\n";
        std::cout << " [ SYSTEM ] Memperoleh " << amount << " EXP!\n";
        std::cout << " " << std::string(52, '-') << "\n";
        
        if (levelsGained > 0) {
            std::cout << " LEVEL UP! Karakter naik " << levelsGained << " Level.\n";
            std::cout << " Stat Points     : +" << (levelsGained * 5) << " (Total: " << statPoints << ")\n";
        }
        
        std::cout << " Level Saat Ini  : " << level << " (Maks. 100)\n";
        std::cout << " Progress EXP    : " << exp << " / " << getNextLevelExp() << "\n";
        std::cout << " Kapasitas HP    : " << getMaxHp() << " HP\n";
        
        if(job == "Knight") {
            std::cout << " Multiplier ATK  : " << std::fixed << std::setprecision(2) << (2.0f + (level * 0.05f)) << "x\n";
        }
        std::cout << " " << std::string(52, '=') << "\n";
    }

    void Player::updateSkills() {
        for (auto& skill : skills) {
            if (level >= skill.requiredLevel) {
                skill.isUnlocked = true;
            } else {
                skill.isUnlocked = false;
            }
        }
    }
}