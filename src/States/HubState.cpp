#include "HubState.h"
#include "Core/StateManager.h"
#include "BattleState.h" // Contoh langsung masuk ke pertarungan
#include <iostream>
#include <limits>

void HubState::init() {
    menuChoice = 0;
    std::cout << "\n--- LOGIN BERHASIL ---\n";
}

void HubState::handleInput(StateManager& stateManager) {
    std::cout << "\n=== MENU UTAMA (HUB) ===\n";
    std::cout << "1. Main Game (Simulasi)\n";
    std::cout << "2. Cek Status Karakter\n";
    std::cout << "3. Logout (Kembali ke Login)\n";
    std::cout << "Pilih menu (1-3): ";
    
    std::cin >> menuChoice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        menuChoice = -1;
    }
}

void HubState::update(StateManager& stateManager) {
    switch (menuChoice) {
        case 1:
            // Di sini nanti kamu bisa pushState(GameplayState)
            stateManager.pushState(std::make_unique<BattleState>()); // Contoh langsung masuk ke pertarungan
            menuChoice = 0;
            break;
        case 2:
            std::cout << "\n[Aksi] HP: 100/100 | Mana: 50/50\n";
            menuChoice = 0;
            break;
        case 3:
            std::cout << "\n[Aksi] Logout berhasil...\n";
            // Membuang HubState dari tumpukan, otomatis kembali ke LoginState di bawahnya
            stateManager.popState(); 
            break;
        case -1:
            // Input tidak valid (huruf) tidak ngapa-ngapain di logika, hanya ditangani di render
            break;
        default:
            if (menuChoice != 0) {
                std::cout << "\n[Aksi] Pilihan tidak ada di menu.\n";
            }
            menuChoice = 0;
            break;
    }
}

void HubState::render() {
    if (menuChoice == -1) {
         std::cout << "[Sistem] Tolong masukkan angka yang valid!\n";
         menuChoice = 0; // Reset agar pesan error tidak muncul terus
    }
}