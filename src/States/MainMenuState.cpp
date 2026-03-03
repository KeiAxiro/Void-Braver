#include "States/MainMenuState.h"
#include "States/LoginState.h"
#include "States/RegisterState.h"
#include "Utils/ConsoleUI.h" 
#include <iostream>
#include <string>

void MainMenuState::init(StateManager &stateManager)
{
    // Kosong untuk saat ini
}

void MainMenuState::render()
{
    // Menggunakan Raw String Literal R"( ... )" untuk ASCII Art judul Void Braver
    std::cout << R"(
================================================================
__   __  ___   ___  ___       ___   ___    _   _   _  ___  ___ 
\ \ / / / _ \ |_ _||   \     | _ ) | _ \  /_\ | | | || __|| _ \
 \ V / | (_) | | | | |) |    | _ \ |   / / _ \| |_| || _| |   /
  \_/   \___/ |___||___/     |___/ |_|_\/_/ \_\\___/ |___||_|_\
                                                                   
================================================================
)" << '\n';

    std::cout << "                        [ MAIN MENU ]\n\n";
    std::cout << "                [1] Mulai Petualangan (Login)\n";
    std::cout << "                [2] Buat Akun Baru (Register)\n";
    std::cout << "                [3] Keluar dari Dunia (Exit)\n";
    std::cout << "\n================================================================\n";
    std::cout << "\nPilihanmu: ";
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
