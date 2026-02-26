#include "States/LoginState.h"
#include "States/HubState.h" // Pindah ke pemilihan karakter setelah login
#include "Utils/ConsoleUI.h"
#include <iostream>
#include <string>

void LoginState::init(StateManager &stateManager) {}

void LoginState::render()
{
    ConsoleUI::printHeader("MENU LOGIN");
    std::cout << "Ketik 'batal' pada username untuk kembali.\n\n";
}

void LoginState::update(StateManager &stateManager)
{
    std::string u, p;

    std::cout << "Username: ";
    std::cin >> u;

    if (u == "batal")
    {
        stateManager.popState();
        return;
    }

    std::cout << "Password: ";
    std::cin >> p;

    // MENGGUNAKAN ACCOUNT MANAGER DARI CONTEXT GLOBAL
    if (stateManager.getContext().accountManager.loginAccount(u, p))
    {
        // Simpan username yang sedang login ke context global
        stateManager.getContext().currentUsername = u;

        stateManager.setNotify(ConsoleUI::Green("Login Berhasil! Selamat datang, " + u));

        // Ganti state login ini dengan HubState (Replace = true)
        stateManager.pushState(std::make_unique<HubState>(), true);
    }
    else
    {
        stateManager.setNotify(ConsoleUI::Red("Login Gagal! Username atau password salah."));
    }
}