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
struct Equipment
{
    string name;
    string slot;
    int atkBonus = 0;
    int defBonus = 0;
    int hpBonus = 0;
};

struct Skill
{
    string name;
    string description;
    int requiredLevel;
    int manaCost;
    float damageMultiplier;
    bool isUnlocked;
};

struct Item
{
    string name;
    string type;
    int quantity;
};

// ==========================================
// MOCK DATABASE: Fungsi penarik data stat item
// ==========================================
Equipment getEquipmentData(string name)
{
    // WEAPON
    if (name == "Calamity Breaker")
        return {"Calamity Breaker", "Weapon", 750, 0, 0};
    if (name == "Void-Iron Cleaver")
        return {"Void-Iron Cleaver", "Weapon", 610, 0, 0};
    if (name == "Rusty Greatsword")
        return {"Rusty Greatsword", "Weapon", 15, 0, 0};
    // HELMET
    if (name == "Marshall's Iron Face")
        return {"Marshall's Iron Face", "Helmet", 0, 1500, 8000};
    if (name == "Iron Pot Helm")
        return {"Iron Pot Helm", "Helmet", 0, 50, 250};
    // ARMOR
    if (name == "God-King's Bastion")
        return {"God-King's Bastion", "Armor", 0, 3000, 15000};
    if (name == "Knight's Plate")
        return {"Knight's Plate", "Armor", 0, 300, 1500};
    if (name == "Leather Jerkin")
        return {"Leather Jerkin", "Armor", 0, 50, 300};
    // BOOTS
    if (name == "Grand Marshall's Stride")
        return {"Grand Marshall's Stride", "Boots", 0, 1500, 8000};
    if (name == "Traveler's Boots")
        return {"Traveler's Boots", "Boots", 0, 25, 100};
    // ACCESSORY
    if (name == "Vitality Band")
        return {"Vitality Band", "Accessory", 0, 0, 0};
    if (name == "Iron Ring")
        return {"Iron Ring", "Accessory", 0, 0, 0};

    return {"Kosong", "None", 0, 0, 0};
}

// ==========================================
// 2. STRUKTUR PEMAIN & RUMUS MATEMATIKA
// ==========================================
struct Player
{
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

    int getPrimaryStat()
    {
        if (job == "Knight")
            return str;
        if (job == "Sorcerer")
            return intel;
        return agi;
    }

    float getWeaponEfficiency() { return 1.0f + (getPrimaryStat() / 300.0f); }
    int getSynergizedWeaponAtk() { return (int)(getTotalEquipAtkBase() * getWeaponEfficiency()); }

    int getMaxHp()
    {
        int baseHpGrowth = (int)(pow(level, 2) * 2.5f);
        int vitGrowth = (int)(vit * level * 2.0f);
        return 100 + baseHpGrowth + vitGrowth + getTotalEquipHp();
    }

    int getMaxMp() { return 20 + (intel * 10); }

    int getDefense()
    {
        float vitMultiplier = (job == "Knight") ? 3.0f : (job == "Wanderer") ? 2.0f
                                                                             : 1.0f;
        return (level * 5) + (int)(vit * vitMultiplier) + getTotalEquipDef();
    }

    int getAtkPower()
    {
        float multiplier = (job == "Knight") ? ((level <= 50) ? 2.0f : 7.0f) : 1.0f;
        float strWeight = 1.0f, intWeight = 0.2f, agiWeight = 0.4f;

        int agiCap = 260;
        int effectiveAgi = min(agi, agiCap);
        int excessAgi = max(0, agi - agiCap);

        float totalStatValue = (str * strWeight) + (intel * intWeight) + (effectiveAgi * agiWeight);
        float overcapMultiplier = (level <= 50) ? 2.0f : 6.0f;

        int pureStatPower = (int)(totalStatValue * multiplier) + (int)(excessAgi * overcapMultiplier);
        return pureStatPower + getSynergizedWeaponAtk();
    }

    float getCritRate()
    {
        float rate = (float)agi / (agi + 60.0f) * 0.8f;
        return min(0.65f, rate);
    }

    // Fungsi helper untuk menambah item ke tas
    void addItemToInventory(string itemName, string itemType)
    {
        if (itemName == "Kosong")
            return;
        for (auto &item : inventory)
        {
            if (item.name == itemName)
            {
                item.quantity++;
                return;
            }
        }
        inventory.push_back({itemName, itemType, 1});
    }
};

// ==========================================
// 2.5 FUNGSI LOGIKA PERMAINAN
// ==========================================
void updatePlayerSkills(Player &p)
{
    for (auto &skill : p.skills)
    {
        if (p.level >= skill.requiredLevel)
        {
            skill.isUnlocked = true;
        }
        else
        {
            skill.isUnlocked = false;
        }
    }
}

// ==========================================
// 3. FUNGSI-FUNGSI MENU UI
// ==========================================
void clearScreen() { cout << string(50, '\n'); }

void pauseMenu()
{
    cout << "\n Tekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void showStatsMenu(Player &p)
{
    int choice;
    do
    {
        clearScreen();
        cout << string(50, '=') << endl;
        cout << "                 STATUS KARAKTER                  " << endl;
        cout << string(50, '=') << endl;

        // Atribut Tempur
        cout << " [ Atribut Tempur ]" << endl;
        cout << left << setw(9) << " Max HP" << " : " << setw(7) << p.getMaxHp() << " | (Base + Equip " << p.getTotalEquipHp() << ")" << endl;
        cout << left << setw(9) << " Max MP" << " : " << setw(7) << p.getMaxMp() << " |" << endl;
        cout << left << setw(9) << " ATK" << " : " << setw(7) << p.getAtkPower() << " | (Equip ATK Bersinergi +" << p.getSynergizedWeaponAtk() << ")" << endl;
        cout << left << setw(9) << " DEF" << " : " << setw(7) << p.getDefense() << " | (Equip +" << p.getTotalEquipDef() << ")" << endl;
        cout << left << setw(9) << " Crit" << " : " << fixed << setprecision(1) << p.getCritRate() * 100 << "%" << setw(1) << "  |" << endl;
        cout << "--------------------------------------------------" << endl;

        // Weapon Synergy (Ditampilkan kembali)
        cout << " [ Weapon Synergy ]" << endl;
        cout << left << setw(26) << " Stat Mastery (Primary)" << " : " << p.getPrimaryStat() << endl;
        cout << left << setw(26) << " Weapon Efficiency" << " : " << fixed << setprecision(0) << (p.getWeaponEfficiency() * 100) << "%" << endl;
        cout << "--------------------------------------------------" << endl;

        // Base Stats
        cout << " [ Base Stats ] - Poin Tersisa: " << p.statPoints << endl;
        cout << left << setw(6) << " STR" << " : " << setw(11) << p.str << " | " << setw(5) << "INT" << " : " << p.intel << endl;
        cout << left << setw(6) << " AGI" << " : " << setw(11) << p.agi << " | " << setw(5) << "VIT" << " : " << p.vit << endl;
        cout << "==================================================" << endl;
        cout << " 1. Alokasi stats" << endl;
        cout << " 2. Kembali" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << " Pilih aksi: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // ... (Logika alokasi stats tetap sama) ...
        if (choice == 1 && p.statPoints > 0)
        {
            int statChoice;
            cout << "\n [ Alokasi Poin ]" << endl;
            cout << " Pilih stat (1.STR | 2.INT | 3.AGI | 4.VIT | 0.Batal): ";
            if (!(cin >> statChoice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (statChoice >= 1 && statChoice <= 4)
            {
                int pts;
                cout << " Berapa poin ditambahkan? (Maks " << p.statPoints << "): ";
                if (!(cin >> pts))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                if (pts > 0 && pts <= p.statPoints)
                {
                    switch (statChoice)
                    {
                    case 1:
                        p.str += pts;
                        break;
                    case 2:
                        p.intel += pts;
                        break;
                    case 3:
                        p.agi += pts;
                        break;
                    case 4:
                        p.vit += pts;
                        break;
                    }
                    p.statPoints -= pts;
                }
            }
        }
    } while (choice != 2);
}

void showEquipmentMenu(Player &p)
{
    int choice;
    do
    {
        clearScreen();
        cout << string(50, '=') << endl;
        cout << "                EQUIPMENT SAAT INI                " << endl;
        cout << string(50, '=') << endl;

        // Header Kolom
        cout << left << "  " << setw(11) << "SLOT" << " | " << setw(20) << "NAMA ITEM" << " | BONUS" << endl;
        cout << string(50, '-') << endl;

        // Baris Item - Menggunakan format yang lebih lega dan sejajar
        auto printRow = [](string slot, Equipment e)
        {
            cout << left << "  " << setw(11) << slot << " : " << setw(20) << e.name;
            if (e.atkBonus > 0)
                cout << " ATK +" << e.atkBonus;
            else if (e.defBonus > 0 || e.hpBonus > 0)
                cout << " D+" << e.defBonus << " H+" << e.hpBonus;
            else if (e.name != "Kosong")
                cout << " Special";
            cout << endl;
        };

        printRow("Weapon", p.weapon);
        printRow("Helmet", p.helmet);
        printRow("Armor", p.armor);
        printRow("Boots", p.boots);
        printRow("Accessory", p.accessory);

        cout << string(50, '=') << endl;

        // Pilihan Menu 2 Kolom (Agar konsisten dengan dashboard)
        cout << left << "  " << setw(26) << "1. Pasang / Ubah" << endl
             << "  " << "2. Kembali" << endl;

        cout << string(50, '-') << endl;
        cout << " Pilih aksi: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1)
        {
            // ... (Logika pemilihan slot dan item tetap sama seperti sebelumnya) ...
            // Pastikan UI di dalam logika "Pilih Slot" juga menggunakan string(50, '-')
            // agar lebarnya tetap konsisten 50 karakter.

            int slotChoice;
            cout << "\n [ Pilih Slot yang Ingin Diubah ]" << endl;
            cout << " 1.Weapon | 2.Helmet | 3.Armor | 4.Boots | 5.Acc" << endl;
            cout << " Pilih (1-5, 0 batal): ";
            if (!(cin >> slotChoice) || slotChoice == 0)
                continue;

            string targetType = (slotChoice == 1) ? "Weapon" : (slotChoice == 2) ? "Helmet"
                                                           : (slotChoice == 3)   ? "Armor"
                                                           : (slotChoice == 4)   ? "Boots"
                                                                                 : "Accessory";
            Equipment *targetSlot = (slotChoice == 1) ? &p.weapon : (slotChoice == 2) ? &p.helmet
                                                                : (slotChoice == 3)   ? &p.armor
                                                                : (slotChoice == 4)   ? &p.boots
                                                                                      : &p.accessory;

            cout << string(50, '-') << endl;
            cout << " [ ISI TAS : " << targetType << " ]" << endl;
            cout << " 0. Lepas (Kosongkan Slot)" << endl;

            vector<int> validIndices;
            int dIdx = 1;
            for (int i = 0; i < p.inventory.size(); i++)
            {
                if (p.inventory[i].type == targetType)
                {
                    cout << " " << dIdx << ". " << setw(20) << p.inventory[i].name << " (x" << p.inventory[i].quantity << ")" << endl;
                    validIndices.push_back(i);
                    dIdx++;
                }
            }
            if (validIndices.empty())
                cout << " (Tidak ada item cadangan di tas)" << endl;

            int itemChoice;
            cout << " Pilih: ";
            cin >> itemChoice;

            if (itemChoice == 0 && targetSlot->name != "Kosong")
            {
                p.addItemToInventory(targetSlot->name, targetType);
                *targetSlot = {"Kosong", targetType, 0, 0, 0};
                cout << "\n Berhasil melepas equipment!" << endl;
                pauseMenu();
            }
            else if (itemChoice > 0 && itemChoice <= validIndices.size())
            {
                int invIdx = validIndices[itemChoice - 1];
                string newName = p.inventory[invIdx].name;
                if (targetSlot->name != "Kosong")
                    p.addItemToInventory(targetSlot->name, targetType);
                p.inventory[invIdx].quantity--;
                if (p.inventory[invIdx].quantity <= 0)
                    p.inventory.erase(p.inventory.begin() + invIdx);
                *targetSlot = getEquipmentData(newName);
                cout << "\n Berhasil memasang " << newName << "!" << endl;
                pauseMenu();
            }
        }
    } while (choice != 2);
}

void showSkillsMenu(Player &p)
{
    updatePlayerSkills(p); // Pastikan status unlock terbaru
    clearScreen();
    cout << string(50, '=') << endl;
    cout << "                   DAFTAR SKILL                   " << endl;
    cout << string(50, '=') << endl;
    cout << left << setw(15) << " Level" << " : " << p.level << endl;
    cout << left << setw(15) << " Mana Saat Ini" << " : " << p.getMaxMp() << " MP" << endl;
    cout << string(50, '-') << endl;

    if (p.skills.empty())
    {
        cout << " Karakter belum memiliki daftar skill." << endl;
    }
    else
    {
        for (const auto &s : p.skills)
        {
            // Header Skill
            if (s.isUnlocked)
            {
                cout << " [ " << s.name << " ]" << endl;
            }
            else
            {
                cout << " [ ??? (Terkunci) ] - Req. Lvl " << s.requiredLevel << endl;
            }

            // Detail Skill (Hanya tampil jika terbuka)
            if (s.isUnlocked)
            {
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

void showInventoryMenu(Player &p)
{
    clearScreen();
    cout << "==================================================" << endl;
    cout << "                 TAS INVENTORY                    " << endl;
    cout << "==================================================" << endl;

    if (p.inventory.empty())
    {
        cout << " Tas Anda kosong." << endl;
        cout << "==================================================" << endl;
        pauseMenu();
        return;
    }

    bool hasEquipment = false;
    for (const auto &item : p.inventory)
    {
        if (item.type != "Consumable" && item.type != "Material")
        {
            if (!hasEquipment)
            {
                cout << " [ EQUIPMENT ]" << endl;
                cout << left << setw(23) << " Nama Item" << " | " << setw(12) << "Tipe" << " | Jumlah" << endl;
                cout << "--------------------------------------------------" << endl;
                hasEquipment = true;
            }
            cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << endl;
        }
    }
    if (hasEquipment)
        cout << "--------------------------------------------------" << endl;

    bool hasConsumable = false;
    for (const auto &item : p.inventory)
    {
        if (item.type == "Consumable")
        {
            if (!hasConsumable)
            {
                cout << " [ CONSUMABLE ]" << endl;
                cout << left << setw(23) << " Nama Item" << " | " << setw(12) << "Tipe" << " | Jumlah" << endl;
                cout << "--------------------------------------------------" << endl;
                hasConsumable = true;
            }
            cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << endl;
        }
    }
    if (hasConsumable)
        cout << "--------------------------------------------------" << endl;

    bool hasMaterial = false;
    for (const auto &item : p.inventory)
    {
        if (item.type == "Material")
        {
            if (!hasMaterial)
            {
                cout << " [ MATERIAL ]" << endl;
                cout << left << setw(23) << " Nama Item" << " | " << setw(12) << "Tipe" << " | Jumlah" << endl;
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
int main()
{
    Player p1;
    p1.name = "Arthur";
    p1.job = "Knight";
    p1.level = 100;
    p1.exp = 0;
    p1.statPoints = 500;
    p1.str = 10;
    p1.intel = 5;
    p1.agi = 10;
    p1.vit = 40;

    // Setup Awal (Dipakai)
    p1.weapon = {"Calamity Breaker", "Weapon", 750, 0, 0};
    p1.helmet = {"Marshall's Iron Face", "Helmet", 0, 1500, 8000};
    p1.armor = {"God-King's Bastion", "Armor", 0, 3000, 15000};
    p1.boots = {"Grand Marshall's Stride", "Boots", 0, 1500, 8000};
    p1.accessory = {"Vitality Band", "Accessory", 0, 0, 0};

    p1.skills.push_back({"Heavy Slash", "Menebas musuh dengan 150% ATK.", 1, true});
    p1.skills.push_back({"Shield Bash", "Menyerang musuh dengan perisai (Stun).", 5, false});
    p1.skills.push_back({"Calamity Strike", "Serangan pamungkas penghancur Void.", 50, false});

    // Isi Tas (Untuk Tes Swap/Pasang Equipment)
    p1.inventory.push_back({"Health Potion", "Consumable", 5});
    p1.inventory.push_back({"Heavy Ore", "Material", 12});

    // --> DUMMY EQUIPMENT DI TAS
    p1.inventory.push_back({"Rusty Greatsword", "Weapon", 1});
    p1.inventory.push_back({"Void-Iron Cleaver", "Weapon", 1});
    p1.inventory.push_back({"Iron Pot Helm", "Helmet", 1});
    p1.inventory.push_back({"Knight's Plate", "Armor", 1});
    p1.inventory.push_back({"Leather Jerkin", "Armor", 1});
    p1.inventory.push_back({"Traveler's Boots", "Boots", 2});
    p1.inventory.push_back({"Iron Ring", "Accessory", 1});

    int choice;
    do
    {
        clearScreen();
        cout << string(50, '=') << endl;
        cout << "            M E N U   K A R A K T E R             " << endl;
        cout << string(50, '=') << endl;

        // Info Karakter Utama (Dipindahkan ke sini)
        cout << left << "  " << setw(8) << "Nama" << " : " << setw(14) << p1.name
             << " | " << setw(7) << "Job" << " : " << p1.job << endl;
        cout << left << "  " << setw(8) << "Level" << " : " << setw(14) << p1.level
             << " | " << setw(7) << "EXP" << " : " << p1.exp << "/100" << endl;
        cout << string(50, '-') << endl;

        // --- GANTI BAGIAN OPSI MENU DENGAN KODE INI ---
        cout << endl;
        // Baris pertama menu (Opsi 1 dan 2)
        cout << left << "  " << setw(26) << "1. Status & Alokasi" << "2. Cek Equipment" << endl;
        cout << endl;

        // Baris kedua menu (Opsi 3 dan 4)
        cout << left << "  " << setw(26) << "3. Daftar Skill" << "4. Tas Inventory" << endl;
        cout << endl;

        cout << string(50, '=') << endl;
        cout << "  0. Kembali" << endl;
        // ----------------------------------------------
        cout << string(50, '-') << endl;
        cout << "\n Pilih menu: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            showStatsMenu(p1);
            break;
        case 2:
            showEquipmentMenu(p1);
            break;
        case 3:
            showSkillsMenu(p1);
            break;
        case 4:
            showInventoryMenu(p1);
            break;
        case 0:
            cout << "\n Kembali ke layar sebelumnya..." << endl;
            break;
        default:
            cout << "\n Pilihan tidak valid!" << endl;
            pauseMenu();
            break;
        }
    } while (choice != 0);

    return 0;
}
