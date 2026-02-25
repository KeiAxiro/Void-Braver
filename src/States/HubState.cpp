#include "HubState.h"
#include "../Core/StateManager.h"
#include <iostream>
#include <string>

void HubState::handleInput(StateManager& stateManager) {
    std::string input;
    std::cout << "\n[MENU HUB]\n";
    std::cout << "Mau ke mana? (dungeon / tas / logout): ";
    std::getline(std::cin, input);

    if (input == "logout") {
        std::cout << "Meninggalkan Menu Utama...\n";
        // POP: Buang layar Hub. Otomatis kita akan 'jatuh' kembali ke layar Login!
        stateManager.popState(); 
    }
    // Nanti kamu bisa tambah: if (input == "tas") { stateManager.pushState(InventoryState); }
}

void HubState::update(StateManager& stateManager) {
    // Logika markas utama
}

void HubState::render() {
    std::cout << "====================================\n";
    std::cout << "           KOTA PERMULAAN           \n";
    std::cout << "====================================\n";
}