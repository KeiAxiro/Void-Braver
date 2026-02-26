#include "States/RegisterState.h"
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>

void RegisterState::init(StateManager &stateManager) {}

void RegisterState::render()
{
    ConsoleUI::printHeader("REGISTER ACCOUNT");
    std::cout << "Ketik 'batal' pada username untuk kembali ke menu utama.\n\n";
}

void RegisterState::update(StateManager &stateManager)
{
    std::string username, password;

    std::cout << "Masukkan Username: ";
    std::cin >> username;

    if (username == "batal" || password == "batal")
    {
        stateManager.setNotify(ConsoleUI::Green("Registrasi dibatalkan. Kembali ke menu utama."));
        stateManager.popState();
        return;
    }

    std::cout << "Masukkan Password: ";
    std::cin >> password;

    if (username == "batal" || password == "batal")
    {
        stateManager.setNotify(ConsoleUI::Green("Registrasi dibatalkan. Silakan login."));
        stateManager.popState();
        return;
    }

    // MENGGUNAKAN ACCOUNT MANAGER DARI CONTEXT GLOBAL
    if (stateManager.getContext().accountManager.registerAccount(username, password))
    {
        stateManager.setNotify(ConsoleUI::Green("Registrasi berhasil! Silakan login."));
        stateManager.popState(); // Balik ke Main Menu
    }
    else
    {
        stateManager.setNotify(ConsoleUI::Red("Registrasi Gagal! Username '" + username + "' sudah digunakan."));
    }
}