#include "States/DungeonState.h"
#include "States/BattleState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <random>

namespace States {

    DungeonState::DungeonState(Core::GameContext& ctx) 
        : context(ctx), currentMenu(0), selectedDepth(1), selectedOption(-1), logMessage("Prepare for the expedition.") {}

    void DungeonState::Init() {}

    void DungeonState::HandleInput(Core::StateManager& stateManager) {
        std::string input;
        if (currentMenu == 0) {
            std::cout << "\n Enter Dungeon Depth (1-20) or 0 to Cancel: ";
            std::cin >> input;
            try { selectedOption = std::stoi(input); } 
            catch (...) { selectedOption = -1; }
        } else if (currentMenu == 1) {
            std::cout << "\n Action [1: Move Forward, 0: Escape]: ";
            std::cin >> input;
            try { selectedOption = std::stoi(input); } 
            catch (...) { selectedOption = -1; }
        }
    }

    void DungeonState::Update(Core::StateManager& stateManager) {
        // Otomatis kembali ke Hub jika karakter mati saat pertempuran sebelumnya
        if (context.player.hp <= 0) {
            context.player.hp = static_cast<int>(context.player.getMaxHp() * 0.1f); // Dihidupkan dengan 10% HP
            stateManager.PopState(); 
            return;
        }

        if (currentMenu == 0) {
            if (selectedOption == 0) {
                stateManager.PopState();
            } else if (selectedOption >= 1 && selectedOption <= 20) {
                selectedDepth = selectedOption;
                currentMenu = 1;
                logMessage = "Entered Dungeon Depth " + std::to_string(selectedDepth) + ". It is dark and cold.";
                selectedOption = -1;
            } else if (selectedOption != -1) {
                logMessage = "Invalid depth. Please choose between 1 and 20.";
                selectedOption = -1;
            }
        } else if (currentMenu == 1) {
            if (selectedOption == 0) {
                stateManager.PopState(); // Kabur ke Hub
            } else if (selectedOption == 1) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(1, 100);
                int eventRoll = dist(gen);

                if (eventRoll <= 70) { 
                    logMessage = "You hear footsteps approaching...";
                    // Memanggil BattleState saat bertemu musuh
                    stateManager.PushState(std::make_unique<BattleState>(context, selectedDepth));
                } else if (eventRoll <= 90) { 
                    logMessage = "You moved forward safely. Nothing but dust.";
                } else { 
                    int trapDmg = static_cast<int>(context.player.getMaxHp() * 0.1f);
                    context.player.hp -= trapDmg;
                    if (context.player.hp < 1) context.player.hp = 1; 
                    logMessage = "You triggered a trap! Lost " + std::to_string(trapDmg) + " HP.";
                }
                selectedOption = -1;
            } else if (selectedOption != -1) {
                selectedOption = -1;
            }
        }
    }

    void DungeonState::Render() {
        Utils::ConsoleUI::ClearScreen();
        std::cout << std::string(60, '=') << "\n";
        std::cout << "                      D U N G E O N                       \n";
        std::cout << std::string(60, '=') << "\n\n";
        
        std::cout << "  Player: " << context.player.name << " | HP: " << context.player.hp << "/" << context.player.getMaxHp() << "\n";
        std::cout << std::string(60, '-') << "\n";
        std::cout << "  > " << logMessage << "\n";
        std::cout << std::string(60, '-') << "\n";

        if (currentMenu == 0) {
            std::cout << "  Select the depth you want to explore.\n";
            std::cout << "  Higher depth means stronger enemies and better rewards.\n";
        } else if (currentMenu == 1) {
            std::cout << "  Depth Level: " << selectedDepth << "\n\n";
            std::cout << "  [1] Move Forward\n";
            std::cout << "  [0] Escape to Hub\n";
        }
        std::cout << std::string(60, '=') << "\n";
    }
}