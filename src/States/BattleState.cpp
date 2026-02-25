#include "BattleState.h"
#include "../Core/StateManager.h" // Sesuaikan path jika berbeda
#include "InventoryState.h"       // Wajib include agar bisa memanggil tas
#include <iostream>
#include <limits>

void BattleState::init() {
    menuChoice = 0;
    std::cout << "\n=== PERTARUNGAN DIMULAI! ===\n";
    std::cout << "Seekor monster muncul!\n";
}

void BattleState::handleInput(StateManager& stateManager) {
    std::cout << "\n[Status] HP Kamu: " << myPlayer.hp << " | Potion: " << myPlayer.potionCount << "\n";
    std::cout << "1. Serang Monster\n";
    std::cout << "2. Buka Tas (Inventory)\n";
    std::cout << "3. Lari (Kembali ke Menu Utama)\n";
    std::cout << "Pilih aksi: ";
    
    std::cin >> menuChoice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        menuChoice = -1;
    }
}

void BattleState::update(StateManager& stateManager) {
    switch (menuChoice) {
        case 1:
            std::cout << "\n[Aksi] Kamu menyerang monster! Monster kehilangan 10 HP.\n";
            // Simulasi monster membalas dendam
            myPlayer.hp -= 15; 
            std::cout << "[Aksi] Monster menyerang balik! Kamu kehilangan 15 HP.\n";
            menuChoice = 0;
            break;
            
        case 2:
            // INI BAGIAN PENTINGNYA:
            // Kita membuat layar Inventory baru.
            // Kita menyertakan "&myPlayer" (alamat memori karakter kita)
            // agar Inventory bisa mengubah HP dan Potion yang asli.
            stateManager.pushState(std::make_unique<InventoryState>(&myPlayer));
            menuChoice = 0; // Reset menu saat kembali nanti
            break;
            
        case 3:
            std::cout << "\n[Aksi] Kamu lari terbirit-birit...\n";
            stateManager.popState(); // Buang layar Battle, kembali ke layar di bawahnya
            break;
            
        case -1:
            // Error handling untuk input huruf
            break;
            
        default:
            if (menuChoice != 0) {
                std::cout << "\n[Aksi] Pilihan tidak valid.\n";
            }
            menuChoice = 0;
            break;
    }
}

void BattleState::render() {
    if (menuChoice == -1) {
        std::cout << "[Sistem] Tolong masukkan angka yang valid!\n";
        menuChoice = 0; 
    }
}