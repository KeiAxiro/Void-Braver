#include "States/HubState.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>

void HubState::init(StateManager &stateManager) {}
void HubState::render()
{
    ConsoleUI::printHeader("CLI VOID BRAVER - MAIN HUB");

    std::cout << " Silahkan pilih aksi yang ingin dilakukan:\n\n";
    std::cout << " [" << ConsoleUI::Yellow("1") << "] Pergi ke Toko (Shop)\n";
    std::cout << " [" << ConsoleUI::Red("2") << "] Masuk Dungeon\n";
    std::cout << " [" << ConsoleUI::Cyan("3") << "] Cek Status Karakter\n";
    std::cout << " [" << ConsoleUI::Green("4") << "] Logout\n";
    std::cout << "\n Pilihan: ";
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