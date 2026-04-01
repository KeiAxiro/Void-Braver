#include "States/HubState.h"
#include "States/MainMenuState.h"
#include "States/CharacterMenuState.h"
#include "States/DungeonState.h"
#include "Core/StateManager.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

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
        string input;
        cout << "\n Pilih menu: ";
        cin >> input;

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
                stateManager.PushState(make_unique<CharacterMenuState>(stateManager.GetContext()));
                selectedOption = -1;
                break;
            case 2:
                stateManager.PushState(make_unique<DungeonState>(stateManager.GetContext()));
                selectedOption = -1;
                break;
            case 3:
                cout << "\nEntering Library...\n";
                Utils::ConsoleUI::Pause();
                selectedOption = -1;
                break;
            case 4:
                cout << "\nOpening Crafting Menu...\n";
                Utils::ConsoleUI::Pause();
                selectedOption = -1;
                break;
            case 5:
                cout << "\nGoing to Shop...\n";
                Utils::ConsoleUI::Pause();
                selectedOption = -1;
                break;
            case 0:
                stateManager.ChangeState(make_unique<MainMenuState>());
                break;
            default:
                break;
        }
    }

    void HubState::Render() {
        Utils::ConsoleUI::ClearScreen();

        cout << string(50, '=') << "\n";
        cout << "              V O I D   B R A V E R            \n";
        cout << string(50, '=') << "\n";

        cout << left << "  " << setw(8) << "Nama" << " : " << setw(14) << pName 
                  << " | " << setw(5) << "Class" << " : " << pJob << "\n";
        cout << left << "  " << setw(8) << "Level" << " : " << setw(14) << pLevel 
                  << " | " << setw(5) << "EXP" << " : " << pExp << "/" << pMaxExp << "\n";
        cout << string(50, '-') << "\n\n";

        cout << left << "  " << setw(26) << "1. Character Menu" << "2. Enter Dungeon\n\n";
        cout << left << "  " << setw(26) << "3. Library" << "4. Crafting\n\n";
        cout << left << "  " << setw(26) << "5. Shop" << "0. Main Menu\n\n";
        
        cout << string(50, '=') << "\n";
    }
}