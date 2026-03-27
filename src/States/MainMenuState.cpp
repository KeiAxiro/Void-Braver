#include "States/MainMenuState.h"
#include "Core/StateManager.h"
#include "States/CharSelectionState.h"
#include <iostream>
#include <string>

namespace States
{

    MainMenuState::MainMenuState() : selectedOption(-1) {}

    void MainMenuState::Init()
    {
        // Initialization logic if needed when entering this state
        std::cout << "\033[2J\033[1;1H";

        std::cout << "========================================\n";
        std::cout << "              VOID BRAVER               \n";
        std::cout << "========================================\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
        std::cout << "========================================\n";
    }

    void MainMenuState::HandleInput(Core::StateManager &stateManager)
    {
        std::string input;
        std::cout << "\nEnter your choice: ";
        std::cin >> input;

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
            std::cout << "Transitioning to Character Selection...\n";
            stateManager.ChangeState(std::make_unique<CharSelectionState>());

            selectedOption = -1; // Reset pending action
        }
        else if (selectedOption == 1)
        {
            std::cout << "Transitioning to Load Game...\n";
            // Nanti kita uncomment baris di bawah ini setelah LoadState dibuat
            // stateManager.ChangeState(std::make_unique<LoadState>());

            selectedOption = -1;
        }
        else if (selectedOption == 2)
        {
            std::cout << "Exiting Void Braver. Goodbye!\n";
            stateManager.Quit();
        }
    }

    void MainMenuState::Render()
    {
        std::cout << "\033[2J\033[1;1H";

        std::cout << "========================================\n";
        std::cout << "              VOID BRAVER               \n";
        std::cout << "========================================\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
        std::cout << "========================================\n";
    }

}