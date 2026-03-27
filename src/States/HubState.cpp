#include "States/HubState.h"
#include "States/MainMenuState.h"
#include "States/CharacterMenuState.h"
#include "States/DungeonState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace States {

    HubState::HubState(Core::GameContext& context) 
        : selectedOption(-1), 
          pName(context.player.name), 
          pJob(context.player.job), 
          pLevel(context.player.level), 
          pExp(context.player.exp), 
          pMaxExp(context.player.getNextLevelExp()) {}

    void HubState::Init() {}

    void HubState::HandleInput(Core::StateManager& stateManager) {
        std::string input;
        std::cout << "\n Pilih menu: ";
        std::cin >> input;

        if (input == "1") selectedOption = 1;
        else if (input == "2") selectedOption = 2;
        else if (input == "3") selectedOption = 3;
        else if (input == "4") selectedOption = 4;
        else if (input == "5") selectedOption = 5;
        else if (input == "0") selectedOption = 0;
        else selectedOption = -1;
    }

    void HubState::Update(Core::StateManager& stateManager) {
        pName = stateManager.GetContext().player.name;
        pJob = stateManager.GetContext().player.job;
        pLevel = stateManager.GetContext().player.level;
        pExp = stateManager.GetContext().player.exp;
        pMaxExp = stateManager.GetContext().player.getNextLevelExp();

        switch (selectedOption) {
            case 1:
                stateManager.PushState(std::make_unique<CharacterMenuState>(stateManager.GetContext()));
                selectedOption = -1;
                break;
            case 2:
                stateManager.PushState(std::make_unique<DungeonState>(stateManager.GetContext()));
                selectedOption = -1;
                break;
            case 3:
                std::cout << "\nEntering Library...\n";
                Utils::ConsoleUI::Pause();
                selectedOption = -1;
                break;
            case 4:
                std::cout << "\nOpening Crafting Menu...\n";
                Utils::ConsoleUI::Pause();
                selectedOption = -1;
                break;
            case 5:
                std::cout << "\nGoing to Shop...\n";
                Utils::ConsoleUI::Pause();
                selectedOption = -1;
                break;
            case 0:
                stateManager.ChangeState(std::make_unique<MainMenuState>());
                break;
            default:
                break;
        }
    }

    void HubState::Render() {
        Utils::ConsoleUI::ClearScreen();

        std::cout << std::string(50, '=') << "\n";
        std::cout << "              V O I D   B R A V E R            \n";
        std::cout << std::string(50, '=') << "\n";

        std::cout << std::left << "  " << std::setw(8) << "Nama" << " : " << std::setw(14) << pName 
                  << " | " << std::setw(5) << "Class" << " : " << pJob << "\n";
        std::cout << std::left << "  " << std::setw(8) << "Level" << " : " << std::setw(14) << pLevel 
                  << " | " << std::setw(5) << "EXP" << " : " << pExp << "/" << pMaxExp << "\n";
        std::cout << std::string(50, '-') << "\n\n";

        std::cout << std::left << "  " << std::setw(26) << "1. Character Menu" << "2. Enter Dungeon\n\n";
        std::cout << std::left << "  " << std::setw(26) << "3. Library" << "4. Crafting\n\n";
        std::cout << std::left << "  " << std::setw(26) << "5. Shop" << "0. Main Menu\n\n";
        
        std::cout << std::string(50, '=') << "\n";
    }
}