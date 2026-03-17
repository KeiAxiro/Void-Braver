#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <limits> 

using namespace std;

// ==========================================
// 1. STRUKTUR DATA PENDUKUNG
// ==========================================
struct Equipment {
    string name;
    string slot; 
    int atkBonus = 0;
    int defBonus = 0;
    int hpBonus = 0;
};

struct Skill {
    string name;
    string description;
    int requiredLevel;
    int manaCost;
    float damageMultiplier;
    bool isUnlocked;
};

struct Item {
    string name;
    string type; 
    int quantity;
};

// ==========================================
// MOCK DATABASE: Fungsi penarik data stat item
// ==========================================
Equipment getEquipmentData(string name) {
    // WEAPON (ATK, DEF, HP)
    if (name == "Calamity Breaker") return {"Calamity Breaker", "Weapon", 750, 0, 0};
    if (name == "Void-Iron Cleaver") return {"Void-Iron Cleaver", "Weapon", 210, 0, 0}; 
    if (name == "Rusty Greatsword") return {"Rusty Greatsword", "Weapon", 10, 0, 0};
    
    // HELMET (ATK, DEF, HP) -> Sesuai SQL Baru
    if (name == "Marshall's Iron Face") return {"Marshall's Iron Face", "Helmet", 0, 200, 8000};
    if (name == "Iron Pot Helm") return {"Iron Pot Helm", "Helmet", 0, 5, 45};
    
    // ARMOR (ATK, DEF, HP)
    if (name == "God-King's Bastion") return {"God-King's Bastion", "Armor", 0, 400, 15000};
    if (name == "Knight's Plate") return {"Knight's Plate", "Armor", 0, 55, 480};
    if (name == "Leather Jerkin") return {"Leather Jerkin", "Armor", 0, 10, 95};
    
    // BOOTS (ATK, DEF, HP)
    if (name == "Marshall's Stride") return {"Marshall's Stride", "Boots", 0, 200, 8000};
    if (name == "Traveler's Boots") return {"Traveler's Boots", "Boots", 0, 1, 15};
    
    // ACCESSORY
    if (name == "Vitality Band") return {"Vitality Band", "Accessory", 0, 0, 0};
    if (name == "Iron Ring") return {"Iron Ring", "Accessory", 0, 0, 0};

    return {"Kosong", "None", 0, 0, 0};
}

// ==========================================
// 2. STRUKTUR PEMAIN & RUMUS MATEMATIKA
// ==========================================
struct Player {
    string name;
    string job;
    int level;
    int exp;
    int statPoints; 

    int str, intel, agi, vit;

    Equipment weapon, helmet, armor, boots, accessory;
    vector<Skill> skills;
    vector<Item> inventory;

    int getTotalEquipAtkBase() { return weapon.atkBonus + accessory.atkBonus; }
    int getTotalEquipDef() { return helmet.defBonus + armor.defBonus + boots.defBonus + accessory.defBonus; }
    int getTotalEquipHp() { return helmet.hpBonus + armor.hpBonus + boots.hpBonus + accessory.hpBonus; }

    int getPrimaryStat() {
        if (job == "Knight") return str;
        if (job == "Sorcerer") return intel;
        return agi; 
    }

    float getWeaponEfficiency() { return 1.0f + (getPrimaryStat() / 300.0f); }
    int getSynergizedWeaponAtk() { return (int)(getTotalEquipAtkBase() * getWeaponEfficiency()); }

    int getMaxHp() { 
        int baseHpGrowth = (int)(pow(level, 2) * 2.5f);
        int vitGrowth = (int)(vit * level * 2.0f);
        return 100 + baseHpGrowth + vitGrowth + getTotalEquipHp(); 
    }
    
    int getMaxMp() { return 20 + (intel * 10); }
    
    int getDefense() { 
        float vitMultiplier = (job == "Knight") ? 3.0f : (job == "Wanderer") ? 2.0f : 1.0f;
        return (level * 5) + (int)(vit * vitMultiplier) + getTotalEquipDef(); 
    }
    
    int getAtkPower() {
        float multiplier = 1.0f;
        float strWeight = 0, intWeight = 0, agiWeight = 0;
        
        int effectiveAgi = agi;
        int excessAgi = 0;
        int effectiveInt = intel;
        int excessInt = 0;
        
        // Pengali untuk status yang melebihi batas (Overcap)
        float overcapMultiplier = (level <= 50) ? 2.0f : 6.0f; 

        if (job == "Knight") {
            multiplier = 2.0f + (level * 0.05f); 
            strWeight = 1.0f; intWeight = 0.2f; agiWeight = 0.4f;
        } else if (job == "Sorcerer") {
            multiplier = 1.5f + (level * 0.04f);
            strWeight = 0.2f; intWeight = 1.0f; agiWeight = 0.3f;
            int intCap = 260;
            effectiveInt = min(intel, intCap);
            excessInt = max(0, intel - intCap);
        } else if (job == "Wanderer") {
            multiplier = 1.0f + (level * 0.03f);
            strWeight = 0.4f; intWeight = 0.2f; agiWeight = 1.0f;
            int agiCap = 260;
            effectiveAgi = min(agi, agiCap);
            excessAgi = max(0, agi - agiCap);
        }
        
        float totalStatValue = (str * strWeight) + (effectiveInt * intWeight) + (effectiveAgi * agiWeight);
        int overcapBonus = (int)(excessAgi * overcapMultiplier) + (int)(excessInt * overcapMultiplier);
        int pureStatPower = (int)(totalStatValue * multiplier) + overcapBonus;
        return pureStatPower + getSynergizedWeaponAtk();
    }

    float getCritRate() {
        float rate = (float)agi / (agi + 60.0f) * 0.8f;
        return min(0.65f, rate); 
    }

    void addItemToInventory(string itemName, string itemType) {
        if (itemName == "Kosong") return;
        for (auto& item : inventory) {
            if (item.name == itemName) {
                item.quantity++;
                return;
            }
        }
        inventory.push_back({itemName, itemType, 1});
    }

    // ==========================================
    // FITUR BARU: KALKULASI EXP & ADD EXP
    // ==========================================
    int getNextLevelExp() {
        // 1. Tentukan kedalaman (Depth) dungeon berdasarkan level pemain
        int depth = (level - 1) / 5 + 1; 
        if (depth > 20) depth = 20;

        // 2. Data Drop EXP per Depth
        int depthExp[] = {0, 50, 120, 250, 450, 700, 1050, 1500, 2100, 2800, 3700, 
                          4800, 6100, 7600, 9400, 11500, 14000, 17000, 20500, 24500, 30000};
        
        int expPerRun = depthExp[depth];

        // 3. Tentukan target jumlah Run
        float runs = 1.0f;
        if (level <= 20) {
            runs = 1.0f + ((level - 1) / 19.0f) * 2.0f;   // Lvl 1(1x) -> Lvl 20(3x)
        } else if (level <= 30) {
            runs = 3.0f + ((level - 20) / 10.0f) * 1.0f;  // Lvl 20(3x) -> Lvl 30(4x)
        } else if (level <= 40) {
            runs = 4.0f + ((level - 30) / 10.0f) * 1.0f;  // Lvl 30(4x) -> Lvl 40(5x)
        } else if (level <= 60) {
            runs = 5.0f + ((level - 40) / 20.0f) * 3.0f;  // Lvl 40(5x) -> Lvl 60(8x)
        } else if (level <= 70) {
            runs = 8.0f + ((level - 60) / 10.0f) * 3.0f;  // Lvl 60(8x) -> Lvl 70(11x)
        } else if (level <= 80) {
            runs = 11.0f + ((level - 70) / 10.0f) * 2.0f; // Lvl 70(11x) -> Lvl 80(13x)
        } else if (level <= 95) {
            runs = 13.0f + ((level - 80) / 15.0f) * 5.0f; // Lvl 80(13x) -> Lvl 95(18x)
        } else {
            runs = 18.0f + ((level - 95) / 5.0f) * 2.0f;  // Lvl 95(18x) -> Lvl 100(20x)
        }

        return (int)(expPerRun * runs);
    }

    // Logika penambahan EXP dan Kenaikan Level berantai
    void addExp(int amount) {
        exp += amount;
        int initialLevel = level;
        
        // Loop ini memungkinkan pemain naik beberapa level sekaligus jika EXP yang dimasukkan sangat besar
        while (exp >= getNextLevelExp() && level < 100) { 
            exp -= getNextLevelExp(); // Sisa EXP disimpan untuk level berikutnya
            level++;
            statPoints += 5;
        }

        int levelsGained = level - initialLevel;

        cout << "\n " << string(52, '=') << endl;
        cout << " [ CHEAT DIAKTIFKAN ] Memperoleh " << amount << " EXP!" << endl;
        cout << " " << string(52, '-') << endl;
        
        if (levelsGained > 0) {
            cout << " LEVEL UP! Karakter naik " << levelsGained << " Level." << endl;
            cout << " Stat Points     : +" << (levelsGained * 5) << " (Total: " << statPoints << ")" << endl;
        }
        
        cout << " Level Saat Ini  : " << level << " (Maks. 100)" << endl;
        cout << " Progress EXP    : " << exp << " / " << getNextLevelExp() << endl;
        cout << " Kapasitas HP    : " << getMaxHp() << " HP" << endl;
        
        if(job == "Knight") {
            cout << " Multiplier ATK  : " << fixed << setprecision(2) << (2.0f + (level * 0.05f)) << "x" << endl;
        }
        cout << " " << string(52, '=') << endl;
    }
};

// ==========================================
// 2.5 FUNGSI LOGIKA PERMAINAN
// ==========================================
void updatePlayerSkills(Player& p) {
    for (auto& skill : p.skills) {
        if (p.level >= skill.requiredLevel) {
            skill.isUnlocked = true;
        } else {
            skill.isUnlocked = false;
        }
    }
}

// ==========================================
// 3. FUNGSI-FUNGSI MENU UI
// ==========================================
void clearScreen() { cout << string(50, '\n'); }

void pauseMenu() {
    cout << "\n Tekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void showStatsMenu(Player& p) {
    int choice;
    do {
        clearScreen();
        cout << string(50, '=') << endl;
        cout << "                 STATUS KARAKTER                  " << endl;
        cout << string(50, '=') << endl;
        
        cout << " [ Atribut Tempur ]" << endl;
        cout << left << setw(9) << " Max HP" << " : " << setw(7) << p.getMaxHp()   << " | (Base + Equip " << p.getTotalEquipHp() << ")" << endl;
        cout << left << setw(9) << " Max MP" << " : " << setw(7) << p.getMaxMp()   << " |" << endl;
        cout << left << setw(9) << " ATK"    << " : " << setw(7) << p.getAtkPower()<< " | (Equip ATK Bersinergi +" << p.getSynergizedWeaponAtk() << ")" << endl;
        cout << left << setw(9) << " DEF"    << " : " << setw(7) << p.getDefense() << " | (Equip +" << p.getTotalEquipDef() << ")" << endl;
        cout << left << setw(9) << " Crit"   << " : " << fixed << setprecision(1) << p.getCritRate() * 100 << "%" << setw(1) << "   |" << endl;
        cout << "--------------------------------------------------" << endl;

        cout << " [ Weapon Synergy ]" << endl;
        cout << left << setw(26) << " Stat Mastery (Primary)" << "  : " << p.getPrimaryStat() << endl;
        cout << left << setw(26) << " Weapon Efficiency"      << "  : " << fixed << setprecision(0) << (p.getWeaponEfficiency() * 100) << "%" << endl;
        cout << "--------------------------------------------------" << endl;

        cout << " [ Base Stats ] - Poin Tersisa: " << p.statPoints << endl;
        cout << left << setw(6) << " STR" << " : " << setw(11) << p.str << "| " << setw(5) << " INT" << " : " << p.intel << endl;
        cout << left << setw(6) << " AGI" << " : " << setw(11) << p.agi << "| " << setw(5) << " VIT" << " : " << p.vit << endl;
        cout << "==================================================" << endl << endl;
        cout << left << "  " << setw(27) << "1. Alokasi Stats" << "2. Kembali" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << " Pilih aksi: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (choice == 1 && p.statPoints > 0) {
            int statChoice;
            cout << "\n [ Alokasi Poin ]" << endl;
            cout << " Pilih stat (1.STR | 2.INT | 3.AGI | 4.VIT | 0.Batal): ";
            if (!(cin >> statChoice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            if (statChoice >= 1 && statChoice <= 4) {
                int pts;
                cout << " Berapa poin ditambahkan? (Maks " << p.statPoints << "): ";
                if (!(cin >> pts)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
                if (pts > 0 && pts <= p.statPoints) {
                    switch(statChoice) {
                        case 1: p.str += pts; break; case 2: p.intel += pts; break;
                        case 3: p.agi += pts; break; case 4: p.vit += pts; break;
                    }
                    p.statPoints -= pts;
                }
            }
        }
    } while (choice != 2);
}

void showEquipmentMenu(Player& p) {
    int choice;
    do {
        clearScreen();
        cout << string(60, '=') << endl;
        cout << "                     EQUIPMENT SAAT INI" << endl;
        cout << string(60, '=') << endl;
        
        cout << left << "  " << setw(11) << "SLOT" << " | " << setw(25) << "NAMA ITEM" << " | BONUS" << endl;
        cout << string(60, '-') << endl;
        
        auto printRow = [](string slot, Equipment e) {
            cout << left << "  " << setw(11) << slot << " | " << setw(25) << e.name << " | ";
            if (e.atkBonus > 0) cout << "ATK +" << e.atkBonus;
            else if (e.defBonus > 0 || e.hpBonus > 0) cout << "D+" << e.defBonus << " H+" << e.hpBonus;
            else if (e.name != "Kosong") cout << "Special";
            cout << endl;
        };

        printRow("Weapon", p.weapon);
        printRow("Helmet", p.helmet);
        printRow("Armor", p.armor);
        printRow("Boots", p.boots);
        printRow("Accessory", p.accessory);

        cout << string(60, '=') << endl << endl;
        
        cout << left << "  " << setw(31) << "1. Pasang / Ubah" << "2. Kembali" << endl;
        cout << string(60, '-') << endl;
        cout << " Pilih aksi: ";
              
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            int slotChoice;
            cout << "\n [ Pilih Slot yang Ingin Diubah ]" << endl;
            cout << " 1.Weapon | 2.Helmet | 3.Armor | 4.Boots | 5.Acc" << endl;
            cout << " Pilih (1-5, 0 batal): ";
            if (!(cin >> slotChoice) || slotChoice == 0) continue;

            string targetType = (slotChoice == 1) ? "Weapon" : (slotChoice == 2) ? "Helmet" : 
                                (slotChoice == 3) ? "Armor" : (slotChoice == 4) ? "Boots" : "Accessory";
            Equipment* targetSlot = (slotChoice == 1) ? &p.weapon : (slotChoice == 2) ? &p.helmet : 
                                     (slotChoice == 3) ? &p.armor : (slotChoice == 4) ? &p.boots : &p.accessory;

            cout << string(50, '-') << endl;
            cout << " [ ISI TAS : " << targetType << " ]" << endl;
            cout << " 0. Lepas (Kosongkan Slot)" << endl;
            
            vector<int> validIndices;
            int dIdx = 1;
            for (int i = 0; i < (int)p.inventory.size(); i++) {
                if (p.inventory[i].type == targetType) {
                    cout << " " << dIdx << ". " << setw(25) << p.inventory[i].name << " (x" << p.inventory[i].quantity << ")" << endl;
                    validIndices.push_back(i);
                    dIdx++;
                }
            }
            if (validIndices.empty()) cout << " (Tidak ada item cadangan di tas)" << endl;

            int itemChoice;
            cout << " Pilih: ";
            cin >> itemChoice;

            if (itemChoice == 0 && targetSlot->name != "Kosong") {
                p.addItemToInventory(targetSlot->name, targetType);
                *targetSlot = {"Kosong", targetType, 0, 0, 0};
                cout << "\n Berhasil melepas equipment!" << endl;
                pauseMenu();
            } else if (itemChoice > 0 && itemChoice <= (int)validIndices.size()) {
                int invIdx = validIndices[itemChoice - 1];
                string newName = p.inventory[invIdx].name;
                if (targetSlot->name != "Kosong") p.addItemToInventory(targetSlot->name, targetType);
                p.inventory[invIdx].quantity--;
                if (p.inventory[invIdx].quantity <= 0) p.inventory.erase(p.inventory.begin() + invIdx);
                *targetSlot = getEquipmentData(newName);
                cout << "\n Berhasil memasang " << newName << "!" << endl;
                pauseMenu();
            }
        }
    } while (choice != 2);
}

void showSkillsMenu(Player& p) {
    updatePlayerSkills(p); 
    clearScreen();
    cout << string(50, '=') << endl;
    cout << "                   DAFTAR SKILL                   " << endl;
    cout << string(50, '=') << endl;
    cout << left << setw(15) << " Level" << " : " << p.level << endl;
    cout << left << setw(15) << " Mana Saat Ini" << " : " << p.getMaxMp() << " MP" << endl;
    cout << string(50, '-') << endl;

    if (p.skills.empty()) {
        cout << " Karakter belum memiliki daftar skill." << endl;
    } else {
        for (const auto& s : p.skills) {
            if (s.isUnlocked) {
                cout << " [ " << s.name << " ]" << endl;
            } else {
                cout << " [ ??? (Terkunci) ] - Req. Lvl " << s.requiredLevel << endl;
            }

            if (s.isUnlocked) {
                cout << "   > Biaya Mana : " << s.manaCost << " MP" << endl;
                cout << "   > Multiplier : " << fixed << setprecision(0) << (s.damageMultiplier * 100) << "% ATK" << endl;
                cout << "   > Efek       : " << s.description << endl;
            }
            cout << endl;
        }
    }
    cout << string(50, '=') << endl;
    pauseMenu();
}

void showInventoryMenu(Player& p) {
    clearScreen();
    cout << "==================================================" << endl;
    cout << "                 TAS INVENTORY                    " << endl;
    cout << "==================================================" << endl;

    if (p.inventory.empty()) {
        cout << " Tas Anda kosong." << endl;
        cout << "==================================================" << endl;
        pauseMenu();
        return;
    }

    bool hasEquipment = false;
    for (const auto& item : p.inventory) {
        if (item.type != "Consumable" && item.type != "Material") {
            if (!hasEquipment) {
                cout << " [ EQUIPMENT ]" << endl;
                cout << left << setw(23) << " Nama Item" << "| " << setw(12) << "Tipe" << " | Jumlah" << endl;
                cout << "--------------------------------------------------" << endl;
                hasEquipment = true;
            }
            cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << endl;
        }
    }
    if (hasEquipment) cout << "--------------------------------------------------" << endl;

    bool hasConsumable = false;
    for (const auto& item : p.inventory) {
        if (item.type == "Consumable") {
            if (!hasConsumable) {
                cout << " [ CONSUMABLE ]" << endl;
                cout << left << setw(23) << " Nama Item" << "| " << setw(12) << "Tipe" << " | Jumlah" << endl;
                cout << "--------------------------------------------------" << endl;
                hasConsumable = true;
            }
            cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << endl;
        }
    }
    if (hasConsumable) cout << "--------------------------------------------------" << endl;

    bool hasMaterial = false;
    for (const auto& item : p.inventory) {
        if (item.type == "Material") {
            if (!hasMaterial) {
                cout << " [ MATERIAL ]" << endl;
                cout << left << setw(23) << " Nama Item" << "| " << setw(12) << "Tipe" << " | Jumlah" << endl;
                cout << "--------------------------------------------------" << endl;
                hasMaterial = true;
            }
            cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << endl;
        }
    }

    cout << "==================================================" << endl;
    pauseMenu();
}

// ==========================================
// 4. MAIN FUNCTION 
// ==========================================
int main() {
    Player p1;
    p1.name = "Arthur";
    p1.job = "Knight"; 
    p1.level = 1;    
    p1.exp = 0;
    p1.statPoints = 5; 
    p1.str = 10; p1.intel = 5; p1.agi = 10; p1.vit = 40; 

    // Setup Awal
    p1.weapon = {"Calamity Breaker", "Weapon", 750, 0, 0}; 
    p1.helmet = {"Marshall's Iron Face", "Helmet", 0, 200, 8000}; 
    p1.armor = {"God-King's Bastion", "Armor", 0, 400, 15000};
    p1.boots = {"Marshall's Stride", "Boots", 0, 200, 8000};
    p1.accessory = {"Vitality Band", "Accessory", 0, 0, 0}; 

    p1.skills.push_back({"Heavy Slash", "Menebas musuh dengan 150% ATK.", 1, 15, 1.5f, true});
    p1.skills.push_back({"Shield Bash", "Menyerang musuh dengan perisai (Stun).", 5, 35, 3.0f, false});
    p1.skills.push_back({"Calamity Strike", "Serangan pamungkas penghancur Void.", 50, 120, 25.0f, false});

    p1.inventory.push_back({"Health Potion", "Consumable", 5});
    p1.inventory.push_back({"Heavy Ore", "Material", 12});
    p1.inventory.push_back({"Rusty Greatsword", "Weapon", 1});
    p1.inventory.push_back({"Void-Iron Cleaver", "Weapon", 1});
    p1.inventory.push_back({"Iron Pot Helm", "Helmet", 1});
    p1.inventory.push_back({"Knight's Plate", "Armor", 1});
    p1.inventory.push_back({"Leather Jerkin", "Armor", 1});
    p1.inventory.push_back({"Traveler's Boots", "Boots", 2});
    p1.inventory.push_back({"Iron Ring", "Accessory", 1});

    int choice;
    do {
        clearScreen();
        cout << string(50, '=') << endl;
        cout << "            M E N U   K A R A K T E R             " << endl;
        cout << string(50, '=') << endl;

        cout << left << "  " << setw(8) << "Nama"  << " : " << setw(14) << p1.name 
             << " | " << setw(7) << "Job" << " : " << p1.job << endl;
        cout << left << "  " << setw(8) << "Level" << " : " << setw(14) << p1.level 
             << " | " << setw(7) << "EXP" << " : " << p1.exp << "/" << p1.getNextLevelExp() << endl;
        cout << string(50, '-') << endl;

        cout << endl;
        cout << left << "  " << setw(26) << "1. Status & Alokasi" << "2. Cek Equipment" << endl;
        cout << endl;
        cout << left << "  " << setw(26) << "3. Daftar Skill" << "4. Tas Inventory" << endl;
        cout << endl;
        
        cout << string(50, '=') << endl << endl;
        
        cout << left << "  " << setw(25) << "0. Kembali" << "99. Cheat Tambah EXP" << endl;
        
        cout << string(50, '-') << endl;
        cout << "\n Pilih menu: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch(choice) {
            case 1: showStatsMenu(p1); break;
            case 2: showEquipmentMenu(p1); break;
            case 3: showSkillsMenu(p1); break;
            case 4: showInventoryMenu(p1); break;
            
            // Logika Pemanggilan Fungsi Add EXP
            case 99: {
                int expAmount;
                cout << "\n [ CHEAT ] Masukkan jumlah EXP yang ingin ditambahkan: ";
                if (!(cin >> expAmount) || expAmount <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << " Input tidak valid!" << endl;
                } else {
                    p1.addExp(expAmount); 
                    updatePlayerSkills(p1); // Cek skill baru setelah mendapat EXP
                }
                pauseMenu();
                break;
            }
                
            case 0: cout << "\n Kembali ke layar sebelumnya..." << endl; break;
            default: cout << "\n Pilihan tidak valid!" << endl; pauseMenu(); break;
        }
    } while (choice != 0);
    
    return 0;
}
