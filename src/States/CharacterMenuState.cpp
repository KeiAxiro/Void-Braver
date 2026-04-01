#include "States/CharacterMenuState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;
namespace States {

    // Konstruktor inisialisasi context dan default state variable
    CharacterMenuState::CharacterMenuState(Core::GameContext& ctx)
        : context(ctx), currentView(0), selectedOption(-1) {}

    void CharacterMenuState::Init() {}

    void CharacterMenuState::HandleInput(Core::StateManager& stateManager) {
        string input;
        if (currentView == 0) {
            cout << "\n Pilih menu: ";
            cin >> input;
            // Sanitasi input manual dari string ke integer state untuk mencegah infinite loop tipe data
            if (input == "1") selectedOption = 1;
            else if (input == "2") selectedOption = 2;
            else if (input == "3") selectedOption = 3;
            else if (input == "4") selectedOption = 4;
            else if (input == "99") selectedOption = 99;
            else if (input == "0") selectedOption = 0;
            else selectedOption = -1;
        } else if (currentView == 1) {
            cout << "\n Pilih aksi (1. Alokasi Stats, 2. Kembali): ";
            cin >> input;
            if (input == "1") selectedOption = 1;
            else if (input == "2") selectedOption = 2;
            else selectedOption = -1;
        } else if (currentView == 2) {
            cout << "\n Pilih aksi (1. Pasang / Ubah, 2. Kembali): ";
            cin >> input;
            if (input == "1") selectedOption = 1;
            else if (input == "2") selectedOption = 2;
            else selectedOption = -1;
        } else if (currentView == 3 || currentView == 4) {
            cout << "\n Pilih aksi (0. Kembali): ";
            cin >> input;
            if (input == "0") selectedOption = 0;
            else selectedOption = -1;
        }
    }

    void CharacterMenuState::Update(Core::StateManager& stateManager) {
        if (currentView == 0) {
            // State machine controller untuk routing sub-menu
            switch (selectedOption) {
                case 1: currentView = 1; selectedOption = -1; break;
                case 2: currentView = 2; selectedOption = -1; break;
                case 3: currentView = 3; selectedOption = -1; break;
                case 4: currentView = 4; selectedOption = -1; break;
                case 99: 
                    // Debug trigger untuk inject EXP dan sync skill secara instan
                    context.player.addExp(500); 
                    context.player.updateSkills(); 
                    Utils::ConsoleUI::Pause(); 
                    selectedOption = -1; 
                    break;
                case 0: stateManager.PopState(); break;
                default: break;
            }
        } else if (currentView == 1) {
            if (selectedOption == 2) {
                currentView = 0; selectedOption = -1;
            } else if (selectedOption == 1) {
                // Blok logika validasi dan mutasi alokasi atribut base status
                if (context.player.statPoints > 0) {
                    int statChoice, pts;
                    cout << " Pilih stat (1.STR | 2.INT | 3.AGI | 4.VIT | 0.Batal): ";
                    cin >> statChoice;
                    if (statChoice >= 1 && statChoice <= 4) {
                        cout << " Berapa poin ditambahkan? (Maks " << context.player.statPoints << "): ";
                        cin >> pts;
                        if (pts > 0 && pts <= context.player.statPoints) {
                            if (statChoice == 1) context.player.str += pts;
                            else if (statChoice == 2) context.player.intel += pts;
                            else if (statChoice == 3) context.player.agi += pts;
                            else if (statChoice == 4) context.player.vit += pts;
                            context.player.statPoints -= pts;
                        }
                    }
                } else {
                    cout << " Tidak ada Stat Points tersisa!\n";
                    Utils::ConsoleUI::Pause();
                }
                selectedOption = -1;
            }
        } else if (currentView == 2) {
            if (selectedOption == 2) {
                currentView = 0; selectedOption = -1;
            } else if (selectedOption == 1) {
                int slotChoice;
                cout << "\n [ Pilih Slot yang Ingin Diubah ]\n";
                cout << " 1.Weapon | 2.Helmet | 3.Armor | 4.Boots | 5.Acc\n";
                cout << " Pilih (1-5, 0 batal): ";
                cin >> slotChoice;

                if (slotChoice >= 1 && slotChoice <= 5) {
                    // Mapping string tipe target dan pointer memory slot spesifik untuk memanipulasi data tanpa hardcode berulang
                    string targetType = (slotChoice == 1) ? "Weapon" : (slotChoice == 2) ? "Helmet" : 
                                             (slotChoice == 3) ? "Armor" : (slotChoice == 4) ? "Boots" : "Accessory";
                    Entities::Equipment* targetSlot = (slotChoice == 1) ? &context.player.weapon : 
                                                      (slotChoice == 2) ? &context.player.helmet : 
                                                      (slotChoice == 3) ? &context.player.armor : 
                                                      (slotChoice == 4) ? &context.player.boots : &context.player.accessory;

                    cout << string(50, '-') << "\n";
                    cout << " [ ISI TAS : " << targetType << " ]\n";
                    cout << " 0. Lepas (Kosongkan Slot)\n";
                    
                    // Menyimpan index asli dari vector inventory untuk sinkronisasi elemen yang difilter saat dirender
                    vector<int> validIndices;
                    int dIdx = 1;
                    for (int i = 0; i < static_cast<int>(context.player.inventory.size()); i++) {
                        if (context.player.inventory[i].type == targetType) {
                            cout << " " << dIdx << ". " << setw(25) << context.player.inventory[i].name 
                                      << " (x" << context.player.inventory[i].quantity << ")\n";
                            validIndices.push_back(i);
                            dIdx++;
                        }
                    }
                    if (validIndices.empty()) cout << " (Tidak ada item cadangan di tas)\n";

                    int itemChoice;
                    cout << " Pilih: ";
                    cin >> itemChoice;

                    // Logika Swap/Unequip Data
                    if (itemChoice == 0 && targetSlot->name != "Kosong") {
                        // Push item yang terpasang kembali ke vector inventory
                        context.player.addItemToInventory(targetSlot->name, targetType);
                        // Reset pointer reference ke template Kosong
                        *targetSlot = Entities::getEquipmentData("Kosong");
                        cout << "\n Berhasil melepas equipment!\n";
                        Utils::ConsoleUI::Pause();
                    } else if (itemChoice > 0 && itemChoice <= static_cast<int>(validIndices.size())) {
                        // Resolusi index tampilan ke index memory vector inventory sebenarnya
                        int invIdx = validIndices[itemChoice - 1];
                        string newName = context.player.inventory[invIdx].name;
                        
                        // Push item yang sedang dipakai (jika ada) kembali ke inventory sebelum swap
                        if (targetSlot->name != "Kosong") {
                            context.player.addItemToInventory(targetSlot->name, targetType);
                        }
                        
                        // Mutasi quantity item baru dan pop elemen dari vector jika quantity menyentuh angka 0
                        context.player.inventory[invIdx].quantity--;
                        if (context.player.inventory[invIdx].quantity <= 0) {
                            context.player.inventory.erase(context.player.inventory.begin() + invIdx);
                        }
                        
                        // Timpa memori pada target slot dengan objek data equipment baru
                        *targetSlot = Entities::getEquipmentData(newName);
                        cout << "\n Berhasil memasang " << newName << "!\n";
                        Utils::ConsoleUI::Pause();
                    }
                }
                selectedOption = -1;
            }
        } else if (currentView == 3 || currentView == 4) {
            if (selectedOption == 0) {
                currentView = 0; selectedOption = -1;
            }
        }
    }

    void CharacterMenuState::Render() {
        Utils::ConsoleUI::ClearScreen();
        auto& p = context.player;

        if (currentView == 0) {
            cout << string(50, '=') << "\n";
            cout << "            M E N U   K A R A K T E R             \n";
            cout << string(50, '=') << "\n";

            cout << left << "  " << setw(8) << "Nama"  << " : " << setw(14) << p.name 
                      << " | " << setw(7) << "Job" << " : " << p.job << "\n";
            cout << left << "  " << setw(8) << "Level" << " : " << setw(14) << p.level 
                      << " | " << setw(7) << "EXP" << " : " << p.exp << "/" << p.getNextLevelExp() << "\n";
            cout << string(50, '-') << "\n\n";

            cout << left << "  " << setw(26) << "1. Status & Alokasi" << "2. Cek Equipment\n\n";
            cout << left << "  " << setw(26) << "3. Daftar Skill" << "4. Tas Inventory\n\n";
            cout << string(50, '=') << "\n\n";
            cout << left << "  " << setw(25) << "0. Kembali" << "99. Cheat Tambah EXP\n";
            cout << string(50, '-') << "\n";

        } else if (currentView == 1) {
            cout << string(50, '=') << "\n";
            cout << "                 STATUS KARAKTER                  \n";
            cout << string(50, '=') << "\n";
            
            cout << " [ Atribut Tempur ]\n";
            cout << left << setw(9) << " Max HP" << " : " << setw(7) << p.getMaxHp()   << " | (Base + Equip " << p.getTotalEquipHp() << ")\n";
            cout << left << setw(9) << " Max MP" << " : " << setw(7) << p.getMaxMp()   << " |\n";
            cout << left << setw(9) << " ATK"    << " : " << setw(7) << p.getAtkPower()<< " | (Equip ATK Bersinergi +" << p.getSynergizedWeaponAtk() << ")\n";
            cout << left << setw(9) << " DEF"    << " : " << setw(7) << p.getDefense() << " | (Equip +" << p.getTotalEquipDef() << ")\n";
            cout << left << setw(9) << " Crit"   << " : " << fixed << setprecision(1) << p.getCritRate() * 100 << "%" << setw(1) << "   |\n";
            cout << "--------------------------------------------------\n";

            cout << " [ Weapon Synergy ]\n";
            cout << left << setw(26) << " Stat Mastery (Primary)" << "  : " << p.getPrimaryStat() << "\n";
            cout << left << setw(26) << " Weapon Efficiency"      << "  : " << fixed << setprecision(0) << (p.getWeaponEfficiency() * 100) << "%\n";
            cout << "--------------------------------------------------\n";

            cout << " [ Base Stats ] - Poin Tersisa: " << p.statPoints << "\n";
            cout << left << setw(6) << " STR" << " : " << setw(11) << p.str << "| " << setw(5) << " INT" << " : " << p.intel << "\n";
            cout << left << setw(6) << " AGI" << " : " << setw(11) << p.agi << "| " << setw(5) << " VIT" << " : " << p.vit << "\n";
            cout << "==================================================\n";
        } else if (currentView == 2) {
            cout << string(60, '=') << "\n";
            cout << "                    EQUIPMENT SAAT INI\n";
            cout << string(60, '=') << "\n";
            
            cout << left << "  " << setw(11) << "SLOT" << " | " << setw(25) << "NAMA ITEM" << " | BONUS\n";
            cout << string(60, '-') << "\n";
            
            // Penggunaan lambda function lokal untuk reusability code saat merender baris equipment
            auto printRow = [](const string& slot, const Entities::Equipment& e) {
                cout << left << "  " << setw(11) << slot << " | " << setw(25) << e.name << " | ";
                if (e.atkBonus > 0) cout << "ATK +" << e.atkBonus;
                else if (e.defBonus > 0 || e.hpBonus > 0) cout << "D+" << e.defBonus << " H+" << e.hpBonus;
                else if (e.name != "Kosong") cout << "Special";
                cout << "\n";
            };

            printRow("Weapon", p.weapon);
            printRow("Helmet", p.helmet);
            printRow("Armor", p.armor);
            printRow("Boots", p.boots);
            printRow("Accessory", p.accessory);

            cout << string(60, '=') << "\n";
        } else if (currentView == 3) {
            p.updateSkills(); 
            cout << string(50, '=') << "\n";
            cout << "                   DAFTAR SKILL                   \n";
            cout << string(50, '=') << "\n";
            cout << left << setw(15) << " Level" << " : " << p.level << "\n";
            cout << left << setw(15) << " Mana Saat Ini" << " : " << p.getMaxMp() << " MP\n";
            cout << string(50, '-') << "\n";

            if (p.skills.empty()) {
                cout << " Karakter belum memiliki daftar skill.\n";
            } else {
                for (const auto& s : p.skills) {
                    if (s.isUnlocked) cout << " [ " << s.name << " ]\n";
                    else cout << " [ ??? (Terkunci) ] - Req. Lvl " << s.requiredLevel << "\n";

                    if (s.isUnlocked) {
                        cout << "   > Biaya Mana : " << s.manaCost << " MP\n";
                        cout << "   > Multiplier : " << fixed << setprecision(0) << (s.damageMultiplier * 100) << "% ATK\n";
                        cout << "   > Efek       : " << s.description << "\n";
                    }
                    cout << "\n";
                }
            }
            cout << string(50, '=') << "\n";
        } else if (currentView == 4) {
            cout << string(50, '=') << "\n";
            cout << "                TAS INVENTORY                    \n";
            cout << string(50, '=') << "\n";

            if (p.inventory.empty()) {
                cout << " Tas Anda kosong.\n";
            } else {
                bool hasEquipment = false;
                for (const auto& item : p.inventory) {
                    if (item.type != "Consumable" && item.type != "Material") {
                        if (!hasEquipment) {
                            cout << " [ EQUIPMENT ]\n";
                            cout << left << setw(23) << " Nama Item" << "| " << setw(12) << "Tipe" << " | Jumlah\n";
                            cout << string(50, '-') << "\n";
                            hasEquipment = true;
                        }
                        cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << "\n";
                    }
                }
                if (hasEquipment) cout << string(50, '-') << "\n";

                bool hasConsumable = false;
                for (const auto& item : p.inventory) {
                    if (item.type == "Consumable") {
                        if (!hasConsumable) {
                            cout << " [ CONSUMABLE ]\n";
                            cout << left << setw(23) << " Nama Item" << "| " << setw(12) << "Tipe" << " | Jumlah\n";
                            cout << string(50, '-') << "\n";
                            hasConsumable = true;
                        }
                        cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << "\n";
                    }
                }
                if (hasConsumable) cout << string(50, '-') << "\n";

                bool hasMaterial = false;
                for (const auto& item : p.inventory) {
                    if (item.type == "Material") {
                        if (!hasMaterial) {
                            cout << " [ MATERIAL ]\n";
                            cout << left << setw(23) << " Nama Item" << "| " << setw(12) << "Tipe" << " | Jumlah\n";
                            cout << string(50, '-') << "\n";
                            hasMaterial = true;
                        }
                        cout << " " << setw(21) << item.name << " | " << setw(12) << item.type << " | x" << item.quantity << "\n";
                    }
                }
            }
            cout << string(50, '=') << "\n";
        }
    }
}