#include "LoginState.h"
#include "../Core/StateManager.h"
#include "HubState.h" // Kita butuh HubState untuk pindah layar
#include <iostream>
#include <string>

void LoginState::handleInput(StateManager& stateManager) {
    std::string input;
    std::cout << "\n[LAYAR LOGIN]\n";
    std::cout << "Ketik 'masuk' untuk login, atau 'keluar' untuk tutup game: ";
    std::getline(std::cin, input);

    if (input == "masuk") {
        // PUSH: Kita tumpuk layar HubState di atas layar Login!
        stateManager.pushState(std::make_unique<HubState>());
    } 
    else if (input == "keluar") {
        // POP: Kita buang layar Login. Karena tumpukan kosong, game akan tamat.
        stateManager.popState();
    }
}

void LoginState::update(StateManager& stateManager) {
    // Kosong dulu, logika validasi password (hash) nanti ditaruh di sini
}

void LoginState::render() {
    std::cout << "------------------------------------\n";
    std::cout << "  SELAMAT DATANG DI RPG CLI C++!  \n";
    std::cout << "------------------------------------\n";
}