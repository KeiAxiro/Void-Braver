#include "States/MainMenuState.h"
#include "Core/StateManager.h"
#include "States/CharSelectionState.h"
#include <iostream>
#include <string>
using namespace std;

namespace States
{

    MainMenuState::MainMenuState() : selectedOption(-1) {}

    void MainMenuState::Init()
    {
        // Initialization logic if needed when entering this state
        cout << "\033[2J\033[1;1H";

        cout << "========================================\n";
        cout << "              VOID BRAVER               \n";
        cout << "========================================\n";
        cout << "1. New Game\n";
        cout << "2. Load Game\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
    }

    void MainMenuState::HandleInput(Core::StateManager &stateManager)
    {
        string input;
        cout << "\nEnter your choice: ";
        cin >> input;

        if (input == "1")
        {
            selectedOption = 0;
        }
        else if (input == "2")
        {
            selectedOption = 1;
        }
        else if (input == "3")
        {
            selectedOption = 2;
        }
        else
        {
            selectedOption = -1; // Invalid input
        }
    }

    void MainMenuState::Update(Core::StateManager &stateManager)
    {
        if (selectedOption == 0)
        {
            cout << "Transitioning to Character Selection...\n";
            stateManager.ChangeState(make_unique<CharSelectionState>());

            selectedOption = -1; // Reset pending action
        }
        else if (selectedOption == 1)
        {
            cout << "Transitioning to Load Game...\n";
            // Nanti kita uncomment baris di bawah ini setelah LoadState dibuat
            // stateManager.ChangeState(make_unique<LoadState>());

            selectedOption = -1;
        }
        else if (selectedOption == 2)
        {
            cout << "Exiting Void Braver. Goodbye!\n";
            stateManager.Quit();
        }
    }

    void MainMenuState::Render()
    {
        cout << "\033[2J\033[1;1H";

        cout << "========================================\n";
        cout << "              VOID BRAVER               \n";
        cout << "========================================\n";
        cout << "1. New Game\n";
        cout << "2. Load Game\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
    }

}