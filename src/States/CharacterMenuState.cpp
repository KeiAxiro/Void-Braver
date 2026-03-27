#include "States/CharacterMenuState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

namespace States {

    CharacterMenuState::CharacterMenuState(Core::GameContext& ctx)
        : context(ctx), currentView(0), selectedOption(-1) {}

    void CharacterMenuState::Init() {}

    void CharacterMenuState::HandleInput(Core::StateManager& stateManager) {
        std::string input;
        if (currentView == 0) {
            std::cout << "\n Pilih menu: ";
            std::cin >> input;
            if (input == "1") selectedOption = 1;
            else if (input == "2") selectedOption = 2;
            else if (input == "3") selectedOption = 3;
            else if (input == "4") selectedOption = 4;
            else if (input == "99") selectedOption = 99;
            else if (input == "0") selectedOption = 0;
            else selectedOption = -1;
        } else if (currentView == 1) {
            std::cout << "\n Pilih aksi (1. Alokasi Stats, 2. Kembali): ";
            std::cin >> input;
            if (input == "1") selectedOption = 1;
            else if (input == "2") selectedOption = 2;
            else selectedOption = -1;
        } else if (currentView == 2) {
            std::cout << "\n Pilih aksi (1. Pasang / Ubah, 2. Kembali): ";
            std::cin >> input;
            if (input == "1") selectedOption = 1;
            else if (input == "2") selectedOption = 2;
            else selectedOption = -1;
        } else if (currentView == 3 || currentView == 4) {
            std::cout << "\n Pilih aksi (0. Kembali): ";
            std::cin >> input;
            if (input == "0") selectedOption = 0;
            else selectedOption = -1;
        }
    }

    void CharacterMenuState::Update(Core::StateManager& stateManager) {
        if (currentView == 0) {
            switch (selectedOption) {
                case 1: currentView = 1; selectedOption = -1; break;
                case 2: currentView = 2; selectedOption = -1; break;
                case 3: currentView = 3; selectedOption = -1; break;
                case 4: currentView = 4; selectedOption = -1; break;
                case 99: 
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
                if (context.player.statPoints > 0) {
                    int statChoice, pts;
                    std::cout << " Pilih stat (1.STR | 2.INT | 3.AGI | 4.VIT | 0.Batal): ";
                    std::cin >> statChoice;
                    if (statChoice >= 1 && statChoice <= 4) {
                        std::cout << " Berapa poin ditambahkan? (Maks " << context.player.statPoints << "): ";
                        std::cin >> pts;
                        if (pts > 0 && pts <= context.player.statPoints) {
                            if (statChoice == 1) context.player.str += pts;
                            else if (statChoice == 2) context.player.intel += pts;
                            else if (statChoice == 3) context.player.agi += pts;
                            else if (statChoice == 4) context.player.vit += pts;
                            context.player.statPoints -= pts;
                        }
                    }
                } else {
                    std::cout << " Tidak ada Stat Points tersisa!\n";
                    Utils::ConsoleUI::Pause();
                }
                selectedOption = -1;
            }
        } else if (currentView == 2) {
            if (selectedOption == 2) {
                currentView = 0; selectedOption = -1;
            } else if (selectedOption == 1) {
                int slotChoice;
                std::cout << "\n [ Pilih Slot yang Ingin Diubah ]\n";
                std::cout << " 1.Weapon | 2.Helmet | 3.Armor | 4.Boots | 5.Acc\n";
                std::cout << " Pilih (1-5, 0 batal): ";
                std::cin >> slotChoice;

                if (slotChoice >= 1 && slotChoice <= 5) {
                    std::string targetType = (slotChoice == 1) ? "Weapon" : (slotChoice == 2) ? "Helmet" : 
                                             (slotChoice == 3) ? "Armor" : (slotChoice == 4) ? "Boots" : "Accessory";
                    Entities::Equipment* targetSlot = (slotChoice == 1) ? &context.player.weapon : 
                                                      (slotChoice == 2) ? &context.player.helmet : 
                                                      (slotChoice == 3) ? &context.player.armor : 
                                                      (slotChoice == 4) ? &context.player.boots : &context.player.accessory;

                    std::cout << std::string(50, '-') << "\n";
                    std::cout << " [ ISI TAS : " << targetType << " ]\n";
                    std::cout << " 0. Lepas (Kosongkan Slot)\n";
                    
                    std::vector<int> validIndices;
                    int dIdx = 1;
                    for (int i = 0; i < static_cast<int>(context.player.inventory.size()); i++) {
                        if (context.player.inventory[i].type == targetType) {
                            std::cout << " " << dIdx << ". " << std::setw(25) << context.player.inventory[i].name 
                                      << " (x" << context.player.inventory[i].quantity << ")\n";
                            validIndices.push_back(i);
                            dIdx++;
                        }
                    }
                    if (validIndices.empty()) std::cout << " (Tidak ada item cadangan di tas)\n";

                    int itemChoice;
                    std::cout << " Pilih: ";
                    std::cin >> itemChoice;

                    if (itemChoice == 0 && targetSlot->name != "Kosong") {
                        context.player.addItemToInventory(targetSlot->name, targetType);
                        *targetSlot = Entities::getEquipmentData("Kosong");
                        std::cout << "\n Berhasil melepas equipment!\n";
                        Utils::ConsoleUI::Pause();
                    } else if (itemChoice > 0 && itemChoice <= static_cast<int>(validIndices.size())) {
                        int invIdx = validIndices[itemChoice - 1];
                        std::string newName = context.player.inventory[invIdx].name;
                        if (targetSlot->name != "Kosong") {
                            context.player.addItemToInventory(targetSlot->name, targetType);
                        }
                        context.player.inventory[invIdx].quantity--;
                        if (context.player.inventory[invIdx].quantity <= 0) {
                            context.player.inventory.erase(context.player.inventory.begin() + invIdx);
                        }
                        *targetSlot = Entities::getEquipmentData(newName);
                        std::cout << "\n Berhasil memasang " << newName << "!\n";
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
            std::cout << std::string(50, '=') << "\n";
            std::cout << "            M E N U   K A R A K T E R             \n";
            std::cout << std::string(50, '=') << "\n";

            std::cout << std::left << "  " << std::setw(8) << "Nama"  << " : " << std::setw(14) << p.name 
                      << " | " << std::setw(7) << "Job" << " : " << p.job << "\n";
            std::cout << std::left << "  " << std::setw(8) << "Level" << " : " << std::setw(14) << p.level 
                      << " | " << std::setw(7) << "EXP" << " : " << p.exp << "/" << p.getNextLevelExp() << "\n";
            std::cout << std::string(50, '-') << "\n\n";

            std::cout << std::left << "  " << std::setw(26) << "1. Status & Alokasi" << "2. Cek Equipment\n\n";
            std::cout << std::left << "  " << std::setw(26) << "3. Daftar Skill" << "4. Tas Inventory\n\n";
            std::cout << std::string(50, '=') << "\n\n";
            std::cout << std::left << "  " << std::setw(25) << "0. Kembali" << "99. Cheat Tambah EXP\n";
            std::cout << std::string(50, '-') << "\n";

        } else if (currentView == 1) {
            std::cout << std::string(50, '=') << "\n";
            std::cout << "                 STATUS KARAKTER                  \n";
            std::cout << std::string(50, '=') << "\n";
            
            std::cout << " [ Atribut Tempur ]\n";
            std::cout << std::left << std::setw(9) << " Max HP" << " : " << std::setw(7) << p.getMaxHp()   << " | (Base + Equip " << p.getTotalEquipHp() << ")\n";
            std::cout << std::left << std::setw(9) << " Max MP" << " : " << std::setw(7) << p.getMaxMp()   << " |\n";
            std::cout << std::left << std::setw(9) << " ATK"    << " : " << std::setw(7) << p.getAtkPower()<< " | (Equip ATK Bersinergi +" << p.getSynergizedWeaponAtk() << ")\n";
            std::cout << std::left << std::setw(9) << " DEF"    << " : " << std::setw(7) << p.getDefense() << " | (Equip +" << p.getTotalEquipDef() << ")\n";
            std::cout << std::left << std::setw(9) << " Crit"   << " : " << std::fixed << std::setprecision(1) << p.getCritRate() * 100 << "%" << std::setw(1) << "   |\n";
            std::cout << "--------------------------------------------------\n";

            std::cout << " [ Weapon Synergy ]\n";
            std::cout << std::left << std::setw(26) << " Stat Mastery (Primary)" << "  : " << p.getPrimaryStat() << "\n";
            std::cout << std::left << std::setw(26) << " Weapon Efficiency"      << "  : " << std::fixed << std::setprecision(0) << (p.getWeaponEfficiency() * 100) << "%\n";
            std::cout << "--------------------------------------------------\n";

            std::cout << " [ Base Stats ] - Poin Tersisa: " << p.statPoints << "\n";
            std::cout << std::left << std::setw(6) << " STR" << " : " << std::setw(11) << p.str << "| " << std::setw(5) << " INT" << " : " << p.intel << "\n";
            std::cout << std::left << std::setw(6) << " AGI" << " : " << std::setw(11) << p.agi << "| " << std::setw(5) << " VIT" << " : " << p.vit << "\n";
            std::cout << "==================================================\n";
        } else if (currentView == 2) {
            std::cout << std::string(60, '=') << "\n";
            std::cout << "                     EQUIPMENT SAAT INI\n";
            std::cout << std::string(60, '=') << "\n";
            
            std::cout << std::left << "  " << std::setw(11) << "SLOT" << " | " << std::setw(25) << "NAMA ITEM" << " | BONUS\n";
            std::cout << std::string(60, '-') << "\n";
            
            auto printRow = [](const std::string& slot, const Entities::Equipment& e) {
                std::cout << std::left << "  " << std::setw(11) << slot << " | " << std::setw(25) << e.name << " | ";
                if (e.atkBonus > 0) std::cout << "ATK +" << e.atkBonus;
                else if (e.defBonus > 0 || e.hpBonus > 0) std::cout << "D+" << e.defBonus << " H+" << e.hpBonus;
                else if (e.name != "Kosong") std::cout << "Special";
                std::cout << "\n";
            };

            printRow("Weapon", p.weapon);
            printRow("Helmet", p.helmet);
            printRow("Armor", p.armor);
            printRow("Boots", p.boots);
            printRow("Accessory", p.accessory);

            std::cout << std::string(60, '=') << "\n";
        } else if (currentView == 3) {
            p.updateSkills(); 
            std::cout << std::string(50, '=') << "\n";
            std::cout << "                   DAFTAR SKILL                   \n";
            std::cout << std::string(50, '=') << "\n";
            std::cout << std::left << std::setw(15) << " Level" << " : " << p.level << "\n";
            std::cout << std::left << std::setw(15) << " Mana Saat Ini" << " : " << p.getMaxMp() << " MP\n";
            std::cout << std::string(50, '-') << "\n";

            if (p.skills.empty()) {
                std::cout << " Karakter belum memiliki daftar skill.\n";
            } else {
                for (const auto& s : p.skills) {
                    if (s.isUnlocked) std::cout << " [ " << s.name << " ]\n";
                    else std::cout << " [ ??? (Terkunci) ] - Req. Lvl " << s.requiredLevel << "\n";

                    if (s.isUnlocked) {
                        std::cout << "   > Biaya Mana : " << s.manaCost << " MP\n";
                        std::cout << "   > Multiplier : " << std::fixed << std::setprecision(0) << (s.damageMultiplier * 100) << "% ATK\n";
                        std::cout << "   > Efek       : " << s.description << "\n";
                    }
                    std::cout << "\n";
                }
            }
            std::cout << std::string(50, '=') << "\n";
        } else if (currentView == 4) {
            std::cout << std::string(50, '=') << "\n";
            std::cout << "                 TAS INVENTORY                    \n";
            std::cout << std::string(50, '=') << "\n";

            if (p.inventory.empty()) {
                std::cout << " Tas Anda kosong.\n";
            } else {
                bool hasEquipment = false;
                for (const auto& item : p.inventory) {
                    if (item.type != "Consumable" && item.type != "Material") {
                        if (!hasEquipment) {
                            std::cout << " [ EQUIPMENT ]\n";
                            std::cout << std::left << std::setw(23) << " Nama Item" << "| " << std::setw(12) << "Tipe" << " | Jumlah\n";
                            std::cout << std::string(50, '-') << "\n";
                            hasEquipment = true;
                        }
                        std::cout << " " << std::setw(21) << item.name << " | " << std::setw(12) << item.type << " | x" << item.quantity << "\n";
                    }
                }
                if (hasEquipment) std::cout << std::string(50, '-') << "\n";

                bool hasConsumable = false;
                for (const auto& item : p.inventory) {
                    if (item.type == "Consumable") {
                        if (!hasConsumable) {
                            std::cout << " [ CONSUMABLE ]\n";
                            std::cout << std::left << std::setw(23) << " Nama Item" << "| " << std::setw(12) << "Tipe" << " | Jumlah\n";
                            std::cout << std::string(50, '-') << "\n";
                            hasConsumable = true;
                        }
                        std::cout << " " << std::setw(21) << item.name << " | " << std::setw(12) << item.type << " | x" << item.quantity << "\n";
                    }
                }
                if (hasConsumable) std::cout << std::string(50, '-') << "\n";

                bool hasMaterial = false;
                for (const auto& item : p.inventory) {
                    if (item.type == "Material") {
                        if (!hasMaterial) {
                            std::cout << " [ MATERIAL ]\n";
                            std::cout << std::left << std::setw(23) << " Nama Item" << "| " << std::setw(12) << "Tipe" << " | Jumlah\n";
                            std::cout << std::string(50, '-') << "\n";
                            hasMaterial = true;
                        }
                        std::cout << " " << std::setw(21) << item.name << " | " << std::setw(12) << item.type << " | x" << item.quantity << "\n";
                    }
                }
            }
            std::cout << std::string(50, '=') << "\n";
        }
    }
}