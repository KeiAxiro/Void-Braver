#include "Entities/Player.h"
#include "Entities/Item.h" 
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;
namespace Entities {

    // Factory function untuk membangkitkan objek Equipment baru berdasarkan string nama item dari database
    Equipment getEquipmentData(const string& name) {
        if (name == "empty" || name == "Kosong") return {"empty", "None", 0, 0, 0};
        
        ItemTemplate item = getItemTemplateByName(name);
        if (item.name != "Kosong") {
            string slotName = getCategoryName(item.categoryId);
            return {item.name, slotName, item.atkBonus, item.defBonus, item.hpBonus};
        }
        
        return {"empty", "None", 0, 0, 0};
    }

    // Konstruktor inisialisasi default player state dan empty slot equipment untuk mencegah null pointer exceptions
    Player::Player() : name(""), job(""), level(1), exp(0), statPoints(0), str(0), intel(0), agi(0), vit(0), hp(100), mp(50) {
        weapon = {"empty", "Weapon", 0, 0, 0};
        helmet = {"empty", "Helmet", 0, 0, 0};
        armor = {"empty", "Armor", 0, 0, 0};
        boots = {"empty", "Boots", 0, 0, 0};
        accessory = {"empty", "Accessory", 0, 0, 0};
    }

    // Kalkulasi agregasi flat stat dari semua slot equipment yang sedang aktif (terpasang)
    int Player::getTotalEquipAtkBase() const { return weapon.atkBonus + accessory.atkBonus; }
    int Player::getTotalEquipDef() const { return helmet.defBonus + armor.defBonus + boots.defBonus + accessory.defBonus; }
    int Player::getTotalEquipHp() const { return helmet.hpBonus + armor.hpBonus + boots.hpBonus + accessory.hpBonus; }

    // Evaluator pengambil stat utama berdasarkan flag Job Class
    int Player::getPrimaryStat() const {
        if (job == "Knight") return str;
        if (job == "Sorcerer") return intel;
        return agi; 
    }

    // Scaling multiplier senjata: Tiap 300 poin primary stat = +100% (1.0f) efisiensi weapon base ATK
    float Player::getWeaponEfficiency() const { return 1.0f + (getPrimaryStat() / 300.0f); }
    int Player::getSynergizedWeaponAtk() const { return static_cast<int>(getTotalEquipAtkBase() * getWeaponEfficiency()); }

    // Kalkulasi Kapasitas HP Maksimal: Memiliki basis kurva pangkat kuadrat (pow) terhadap level
    int Player::getMaxHp() const { 
        int baseHpGrowth = static_cast<int>(pow(level, 2) * 2.5f);
        int vitGrowth = static_cast<int>(vit * level * 2.0f); // Scaling VIT linear berdasar level
        return 100 + baseHpGrowth + vitGrowth + getTotalEquipHp(); 
    }
    
    // Kalkulasi Kapasitas MP Maksimal (Hanya dipengaruhi stat INT secara linear)
    int Player::getMaxMp() const { return 20 + (intel * 10); }
    
    // Kalkulasi Total Defense dengan multiplier rasio efisiensi VIT per Job
    int Player::getDefense() const { 
        float vitMultiplier = (job == "Knight") ? 3.0f : (job == "Wanderer") ? 2.0f : 1.0f;
        return (level * 5) + static_cast<int>(vit * vitMultiplier) + getTotalEquipDef(); 
    }
    
    // Core sistem kalkulasi Attack Power karakter yang mengkalkulasikan bobot stat (weight), limit (cap), dan Job multiplier
    int Player::getAtkPower() const {
        float multiplier = 1.0f;
        float strWeight = 0, intWeight = 0, agiWeight = 0;
        
        int effectiveAgi = agi;
        int excessAgi = 0;
        int effectiveInt = intel;
        int excessInt = 0;
        
        // Bonus multiplier tinggi (6.0f) saat level > 50 untuk stat overcap
        float overcapMultiplier = (level <= 50) ? 2.0f : 6.0f; 

        if (job == "Knight") {
            // STR murni scaling tanpa hardcap
            multiplier = 2.0f + (level * 0.05f); 
            strWeight = 1.0f; intWeight = 0.2f; agiWeight = 0.4f;
        } else if (job == "Sorcerer") {
            multiplier = 1.5f + (level * 0.04f);
            strWeight = 0.2f; intWeight = 1.0f; agiWeight = 0.3f;
            int intCap = 260; // Hardcap INT dasar untuk Sorcerer (mencegah eksploitasi flat damage linear)
            effectiveInt = min(intel, intCap);
            excessInt = max(0, intel - intCap); // Menyaring kelebihan INT untuk diproses sebagai overcap bonus
        } else if (job == "Wanderer") {
            multiplier = 1.0f + (level * 0.03f);
            strWeight = 0.4f; intWeight = 0.2f; agiWeight = 1.0f;
            int agiCap = 260; // Hardcap AGI dasar untuk Wanderer
            effectiveAgi = min(agi, agiCap);
            excessAgi = max(0, agi - agiCap); // Menyaring kelebihan AGI
        }
        
        float totalStatValue = (str * strWeight) + (effectiveInt * intWeight) + (effectiveAgi * agiWeight);
        // Mengalikan stat yang melewati cap dengan overcap multiplier khusus
        int overcapBonus = static_cast<int>(excessAgi * overcapMultiplier) + static_cast<int>(excessInt * overcapMultiplier);
        int pureStatPower = static_cast<int>(totalStatValue * multiplier) + overcapBonus;
        
        // Final damage = Total stat (termasuk bonus) + Serangan Senjata (setelah dihitung efisiensi stat utama)
        return pureStatPower + getSynergizedWeaponAtk();
    }

    // Kalkulasi peluang Critical: Bersifat Diminishing Returns menggunakan kurva asimtot berdasarkan nilai AGI. Hardcap absolut di 65% (0.65f)
    float Player::getCritRate() const {
        float rate = static_cast<float>(agi) / (agi + 60.0f) * 0.8f;
        return min(0.65f, rate); 
    }

    // Fungsi utilitas untuk memanipulasi vector inventory: Stack item (menambah qty) jika item dengan id/nama yang sama ditemukan, push baru jika tidak
    void Player::addItemToInventory(const string& itemName, const string& itemType) {
        if (itemName == "empty") return;
        for (auto& item : inventory) {
            if (item.name == itemName) {
                item.quantity++;
                return;
            }
        }
        inventory.push_back({itemName, itemType, 1});
    }

    // Membaca array statis yang mendefinisikan batas kebutuhan EXP dari level 1 hingga 100
    int Player::getNextLevelExp() const {
        static const int levelExpTable[101] = {
            0, // Index 0 tidak terpakai
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
        if (level >= 100) return 0; // Menghentikan laju progres di batas level maksimal
        return levelExpTable[level];
    }

    // Fungsi mutasi poin EXP dan pemrosesan rekursif Level Up 
    void Player::addExp(int amount) {
        exp += amount;
        int initialLevel = level;
        
        // Loop kondisi memungkinkan multiple level up sekaligus jika lonjakan EXP yang didapat sangat besar (misal: trigger fitur cheat)
        while (exp >= getNextLevelExp() && level < 100) { 
            exp -= getNextLevelExp();
            level++;
            statPoints += 5; // Reward default tiap level
        }

        int levelsGained = level - initialLevel;

        cout << "\n " << string(52, '=') << "\n";
        cout << " [ SYSTEM ] Memperoleh " << amount << " EXP!\n";
        cout << " " << string(52, '-') << "\n";
        
        if (levelsGained > 0) {
            cout << " LEVEL UP! Karakter naik " << levelsGained << " Level.\n";
            cout << " Stat Points     : +" << (levelsGained * 5) << " (Total: " << statPoints << ")\n";
        }
        
        cout << " Level Saat Ini  : " << level << " (Maks. 100)\n";
        cout << " Progress EXP    : " << exp << " / " << getNextLevelExp() << "\n";
        cout << " Kapasitas HP    : " << getMaxHp() << " HP\n";
        
        if(job == "Knight") {
            cout << " Multiplier ATK  : " << fixed << setprecision(2) << (2.0f + (level * 0.05f)) << "x\n";
        }
        cout << " " << string(52, '=') << "\n";
    }

    // Fungsi sinkronisasi status (unlock) skill berdasarkan kondisi stat level terkini dari player. Dieksekusi sebelum menu skill dirender.
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