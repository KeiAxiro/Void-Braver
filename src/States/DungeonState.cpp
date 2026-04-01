#include "States/DungeonState.h"
#include "States/BattleState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <random>
#include <iomanip>

using namespace std;
namespace States {

    // Konstruktor: Inisialisasi state awal dan mempopulasi vector dungeonList dengan metadata statis
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
        string input;
        // Sanitasi input menggunakan try-catch string-to-int (stoi) untuk mencegah infinite loop/crash jika input bukan angka
        if (currentMenu == 0) {
            cout << "\n Enter Destination ID or 0 to Cancel: ";
            cin >> input;
            try { selectedOption = stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (currentMenu == 1) {
            cout << "\n Enter Dungeon Depth (1-20) or 0 to Go Back: ";
            cin >> input;
            try { selectedOption = stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (currentMenu == 2) {
            cout << "\n Action [1: Move Forward, 0: Escape]: ";
            cin >> input;
            try { selectedOption = stoi(input); } 
            catch (...) { selectedOption = -1; }
        }
    }

    void DungeonState::Update(Core::StateManager& stateManager) {
        // Penanganan kondisi Player mati: Setel HP ke 10% dari Max HP dan pop DungeonState dari stack
        if (context.player.hp <= 0) {
            context.player.hp = static_cast<int>(context.player.getMaxHp() * 0.1f);
            stateManager.PopState(); 
            return;
        }

        // Lambda function dengan capture by reference [&] untuk memvalidasi kelengkapan 5 item Orb di dalam vector inventory
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
                // Logika pembatasan akses dungeon rahasia berdasarkan evaluasi flag hasAllOrbs
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
                // Validasi indeks array unlockedDepths untuk memastikan player tidak melompati depth yang belum diselesaikan
                if (selectedOption <= context.player.unlockedDepths[selectedDungeonID - 1]) {
                    selectedDepth = selectedOption;
                    currentMenu = 2;
                    explorationProgress = 0;
                    logMessage = "Entered " + dungeonList[selectedDungeonID - 1].name + " | Depth " + to_string(selectedDepth);
                } else {
                    logMessage = "Depth " + to_string(selectedOption) + " is still LOCKED. Clear previous depth first.";
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
                
                // Override hardcode progress khusus depth Boss (Depth 20)
                if (selectedDepth == 20) {
                    explorationProgress = 100; 
                } else {
                    explorationProgress += 20; 
                }

                // Handler ketika target progress tercapai
                if (explorationProgress >= 100) {
                    if (selectedDepth == 20) {
                        logMessage = "An overwhelming presence blocks your path... The Boss awaits!";
                        // Inject BattleState ke puncak stack StateManager
                        stateManager.PushState(make_unique<BattleState>(context, selectedDungeonID, selectedDepth));
                    } else {
                        logMessage = "Depth " + to_string(selectedDepth) + " cleared! You found a safe spot.";
                        // Membuka kuncian depth selanjutnya di dalam array jika depth yang diselesaikan adalah depth tertinggi saat ini
                        if (selectedDepth == context.player.unlockedDepths[selectedDungeonID - 1] && selectedDepth < 20) {
                            context.player.unlockedDepths[selectedDungeonID - 1]++;
                        }
                    }
                    currentMenu = 1; 
                    selectedOption = -1;
                } else {
                    // C++ Mersenne Twister RNG generator untuk mengacak event (Encounter/Safe/Trap)
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dist(1, 100);
                    int eventRoll = dist(gen);

                    if (eventRoll <= 60) { 
                        logMessage = "You hear footsteps approaching...";
                        stateManager.PushState(make_unique<BattleState>(context, selectedDungeonID, selectedDepth));
                    } else if (eventRoll <= 85) { 
                        logMessage = "You moved forward safely. Nothing but dust.";
                    } else { 
                        // Logika kalkulasi damage persentase berbasis Max HP untuk Trap
                        int trapDmg = static_cast<int>(context.player.getMaxHp() * 0.1f);
                        context.player.hp -= trapDmg;
                        if (context.player.hp < 1) context.player.hp = 1; 
                        logMessage = "You triggered a trap! Lost " + to_string(trapDmg) + " HP.";
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
        cout << string(75, '=') << "\n";
        cout << "                             D U N G E O N                       \n";
        cout << string(75, '=') << "\n\n";
        
        cout << "  Player: " << context.player.name << " | HP: " << context.player.hp << "/" << context.player.getMaxHp() << "\n";
        cout << string(75, '-') << "\n";
        cout << "  > " << logMessage << "\n";
        cout << string(75, '-') << "\n";

        // Deklarasi ulang lambda checkAllOrbs lokal untuk evaluasi render visibilitas UI
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
            cout << "  Select your destination:\n\n";
            for (size_t i = 0; i < 5; ++i) {
                cout << "  [" << i + 1 << "] " << dungeonList[i].name << "\n";
            }
            
            if (hasAllOrbs) {
                cout << "  [6] " << dungeonList[5].name << "\n";
            } else {
                cout << "  [?] ???\n";
            }
            cout << "\n";
        } else if (currentMenu == 1) {
            const auto& dungeon = dungeonList[selectedDungeonID - 1];
            cout << "  " << dungeon.name << "\n";
            cout << "  Boss: " << dungeon.boss << " | Reward: " << dungeon.reward << "\n\n";
            cout << "  Select depth to explore:\n";
            
            int maxUnlocked = context.player.unlockedDepths[selectedDungeonID - 1];
            for (int i = 1; i <= 10; ++i) {
                cout << "  [" << setw(2) << i << "] Depth " << setw(2) << i 
                          << (i <= maxUnlocked ? " [UNLOCKED]" : " [LOCKED  ]");
                
                int j = i + 10;
                cout << "    |    [" << setw(2) << j << "] Depth " << setw(2) << j 
                          << (j <= maxUnlocked ? " [UNLOCKED]\n" : " [LOCKED  ]\n");
            }
        } else if (currentMenu == 2) {
            cout << "  Location: " << dungeonList[selectedDungeonID - 1].name << "\n";
            cout << "  Depth Level: " << selectedDepth << "\n";
            
            int bars = explorationProgress / 10;
            cout << "  Progress: [" << string(bars, '#') << string(10 - bars, '.') << "] " << explorationProgress << "%\n\n";
            
            cout << "  [1] Move Forward\n";
            cout << "  [0] Escape to Entrance\n";
        }
        cout << string(75, '=') << "\n";
    }
}