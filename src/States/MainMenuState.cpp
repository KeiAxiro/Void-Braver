#include "States/MainMenuState.h"
#include "States/LoginState.h"
#include "States/RegisterState.h"
#include "Utils/ConsoleUI.h" // Asumsi kamu punya helper UI
#include <iostream>
#include <string>

void MainMenuState::init(StateManager &stateManager)
{
    // Kosong untuk saat ini
}

void MainMenuState::render()
{
    ConsoleUI::printHeader("MAIN MENU");
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "3. Exit\n";
    std::cout << "Pilihan: ";
}

void MainMenuState::update(StateManager &stateManager)
{
    std::string input;
    std::cin >> input;

    if (input == "1")
    {
        stateManager.pushState(std::make_unique<LoginState>());
    }
    else if (input == "2")
    {
        stateManager.pushState(std::make_unique<RegisterState>());
    }
    else if (input == "3")
    {
        stateManager.setNotify(ConsoleUI::Green("Sampai jumpa lagi, Pahlawan!"));
        stateManager.popState();
    }
    else
    {
        stateManager.setNotify(ConsoleUI::Red("[Sistem]: Pilihan '" + input + "' tidak tersedia."));
    }
}