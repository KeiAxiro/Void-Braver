#include "Include/HubState.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>

void HubState::init(StateManager &stateManager) {}

void HubState::render()
{
    ConsoleUI::printHeader("CLI VOID BRAVER - HUB MENU");

    std::cout << "1. Pergi ke Toko (Shop)\n";
    std::cout << "2. Masuk Dungeon\n";
    std::cout << "3. Cek Status Karakter\n";
    std::cout << "4. Logout\n";
    std::cout << "Pilihan: ";
}

void HubState::update(StateManager &stateManager)
{
    std::string input;
    std::cin >> input;

    if (input == "1")
    {
        stateManager.setNotify("Sistem Toko belum tersedia.");
    }
    else if (input == "2")
    {
        stateManager.setNotify("Sistem Dungeon belum tersedia.");
    }
    else if (input == "4")
    {
        stateManager.setNotify(ConsoleUI::Green("Keluar dari Hub dan Logout..."));
        stateManager.getContext().currentUsername = ""; // Reset sesi
        stateManager.popState();
    }
    else
    {
        stateManager.setNotify(ConsoleUI::Red("Pilihan tidak valid."));
    }
}