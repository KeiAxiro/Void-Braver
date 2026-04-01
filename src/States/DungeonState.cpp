#include "States/DungeonState.h"
#include "States/BattleState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <random>
#include <iomanip>

namespace States {

    DungeonState::DungeonState(Core::GameContext& ctx) 
        : context(ctx), currentMenu(0), selectedDungeonID(0), selectedDepth(1), selectedOption(-1), explorationProgress(0), logMessage("Prepare for the expedition.") {
        
        dungeonList = {
            {"Dungeon of Valor and Ember", "Volcanic cavern & ancient fortress ruins.", "The Ashen Warlord", "Crimson Orb"},
            {"Dungeon of Knowledge and Wisdom", "Ancient library distorted by magic & an astral tower.", "The Grand Archivist", "Azure Orb"},
            {"Dungeon of Whispers and Shadows", "Dead forest shrouded in eternal fog & dark abysses.", "The Phantom Stalker", "Obsidian Orb"},
            {"Dungeon of Thorns and Resilience", "Toxic swamp & a labyrinth of massive underground roots.", "The Rotting Behemoth", "Emerald Orb"},
            {"Dungeon of Echoes and Destiny", "Dimension of shattered mirrors & abandoned sacred temples.", "The Oracle of Ruin", "Radiant Orb"},
            {"Void Throne", "The absolute darkness where the ultimate evil resides.", "The Void", "Eternal Peace"}
        };
    }

    void DungeonState::Init() {}

    void DungeonState::HandleInput(Core::StateManager& stateManager) {
        std::string input;
        if (currentMenu == 0) {
            std::cout << "\n Enter Destination ID or 0 to Cancel: ";
            std::cin >> input;
            try { selectedOption = std::stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (currentMenu == 1) {
            std::cout << "\n Enter Dungeon Depth (1-20) or 0 to Go Back: ";
            std::cin >> input;
            try { selectedOption = std::stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (currentMenu == 2) {
            std::cout << "\n Action [1: Move Forward, 0: Escape]: ";
            std::cin >> input;
            try { selectedOption = std::stoi(input); } 
            catch (...) { selectedOption = -1; }
        }
    }

    void DungeonState::Update(Core::StateManager& stateManager) {
        if (context.player.hp <= 0) {
            context.player.hp = static_cast<int>(context.player.getMaxHp() * 0.1f);
            stateManager.PopState(); 
            return;
        }

        // Fungsi pengecekan 5 Orb di inventory
        auto checkAllOrbs = [&]() {
            int count = 0;
            for (const auto& item : context.player.inventory) {
                if (item.name == "Crimson Orb" || item.name == "Azure Orb" || 
                    item.name == "Obsidian Orb" || item.name == "Emerald Orb" || 
                    item.name == "Radiant Orb") {
                    count++;
                }
            }
            return count >= 5;
        };

        bool hasAllOrbs = checkAllOrbs();

        if (currentMenu == 0) {
            if (selectedOption == 0) {
                stateManager.PopState();
            } else if (selectedOption >= 1 && selectedOption <= 5) {
                selectedDungeonID = selectedOption;
                currentMenu = 1;
                logMessage = "Target: " + dungeonList[selectedDungeonID - 1].name;
                selectedOption = -1;
            } else if (selectedOption == 6) {
                if (hasAllOrbs) {
                    selectedDungeonID = selectedOption;
                    currentMenu = 1;
                    logMessage = "Target: " + dungeonList[selectedDungeonID - 1].name;
                } else {
                    logMessage = "A mysterious force seals this path. You lack the Orbs of Power.";
                }
                selectedOption = -1;
            } else if (selectedOption != -1) {
                logMessage = "Invalid destination.";
                selectedOption = -1;
            }
        } else if (currentMenu == 1) {
            if (selectedOption == 0) {
                currentMenu = 0; 
                logMessage = "Prepare for the expedition.";
                selectedOption = -1;
            } else if (selectedOption >= 1 && selectedOption <= 20) {
                if (selectedOption <= context.player.unlockedDepths[selectedDungeonID - 1]) {
                    selectedDepth = selectedOption;
                    currentMenu = 2;
                    explorationProgress = 0;
                    logMessage = "Entered " + dungeonList[selectedDungeonID - 1].name + " | Depth " + std::to_string(selectedDepth);
                } else {
                    logMessage = "Depth " + std::to_string(selectedOption) + " is still LOCKED. Clear previous depth first.";
                }
                selectedOption = -1;
            } else if (selectedOption != -1) {
                logMessage = "Invalid depth. Please choose between 1 and 20.";
                selectedOption = -1;
            }
        } else if (currentMenu == 2) {
            if (selectedOption == 0) {
                stateManager.PopState(); 
            } else if (selectedOption == 1) {
                
                if (selectedDepth == 20) {
                    explorationProgress = 100; 
                } else {
                    explorationProgress += 20; 
                }

                if (explorationProgress >= 100) {
                    if (selectedDepth == 20) {
                        logMessage = "An overwhelming presence blocks your path... The Boss awaits!";
                        stateManager.PushState(std::make_unique<BattleState>(context, selectedDungeonID, selectedDepth));
                    } else {
                        logMessage = "Depth " + std::to_string(selectedDepth) + " cleared! You found a safe spot.";
                        if (selectedDepth == context.player.unlockedDepths[selectedDungeonID - 1] && selectedDepth < 20) {
                            context.player.unlockedDepths[selectedDungeonID - 1]++;
                        }
                    }
                    currentMenu = 1; 
                    selectedOption = -1;
                } else {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(1, 100);
                    int eventRoll = dist(gen);

                    if (eventRoll <= 60) { 
                        logMessage = "You hear footsteps approaching...";
                        stateManager.PushState(std::make_unique<BattleState>(context, selectedDungeonID, selectedDepth));
                    } else if (eventRoll <= 85) { 
                        logMessage = "You moved forward safely. Nothing but dust.";
                    } else { 
                        int trapDmg = static_cast<int>(context.player.getMaxHp() * 0.1f);
                        context.player.hp -= trapDmg;
                        if (context.player.hp < 1) context.player.hp = 1; 
                        logMessage = "You triggered a trap! Lost " + std::to_string(trapDmg) + " HP.";
                    }
                    selectedOption = -1;
                }
            } else if (selectedOption != -1) {
                selectedOption = -1;
            }
        }
    }

    void DungeonState::Render() {
        Utils::ConsoleUI::ClearScreen();
        std::cout << std::string(75, '=') << "\n";
        std::cout << "                             D U N G E O N                       \n";
        std::cout << std::string(75, '=') << "\n\n";
        
        std::cout << "  Player: " << context.player.name << " | HP: " << context.player.hp << "/" << context.player.getMaxHp() << "\n";
        std::cout << std::string(75, '-') << "\n";
        std::cout << "  > " << logMessage << "\n";
        std::cout << std::string(75, '-') << "\n";

        // Pengecekan ulang untuk kebutuhan UI
        auto checkAllOrbs = [&]() {
            int count = 0;
            for (const auto& item : context.player.inventory) {
                if (item.name == "Crimson Orb" || item.name == "Azure Orb" || 
                    item.name == "Obsidian Orb" || item.name == "Emerald Orb" || 
                    item.name == "Radiant Orb") {
                    count++;
                }
            }
            return count >= 5;
        };

        bool hasAllOrbs = checkAllOrbs();

        if (currentMenu == 0) {
            std::cout << "  Select your destination:\n\n";
            for (size_t i = 0; i < 5; ++i) {
                std::cout << "  [" << i + 1 << "] " << dungeonList[i].name << "\n";
            }
            
            if (hasAllOrbs) {
                std::cout << "  [6] " << dungeonList[5].name << "\n";
            } else {
                std::cout << "  [?] ???\n";
            }
            std::cout << "\n";
        } else if (currentMenu == 1) {
            const auto& dungeon = dungeonList[selectedDungeonID - 1];
            std::cout << "  " << dungeon.name << "\n";
            std::cout << "  Boss: " << dungeon.boss << " | Reward: " << dungeon.reward << "\n\n";
            std::cout << "  Select depth to explore:\n";
            
            int maxUnlocked = context.player.unlockedDepths[selectedDungeonID - 1];
            for (int i = 1; i <= 10; ++i) {
                std::cout << "  [" << std::setw(2) << i << "] Depth " << std::setw(2) << i 
                          << (i <= maxUnlocked ? " [UNLOCKED]" : " [LOCKED  ]");
                
                int j = i + 10;
                std::cout << "    |    [" << std::setw(2) << j << "] Depth " << std::setw(2) << j 
                          << (j <= maxUnlocked ? " [UNLOCKED]\n" : " [LOCKED  ]\n");
            }
        } else if (currentMenu == 2) {
            std::cout << "  Location: " << dungeonList[selectedDungeonID - 1].name << "\n";
            std::cout << "  Depth Level: " << selectedDepth << "\n";
            
            int bars = explorationProgress / 10;
            std::cout << "  Progress: [" << std::string(bars, '#') << std::string(10 - bars, '.') << "] " << explorationProgress << "%\n\n";
            
            std::cout << "  [1] Move Forward\n";
            std::cout << "  [0] Escape to Entrance\n";
        }
        std::cout << std::string(75, '=') << "\n";
    }
}