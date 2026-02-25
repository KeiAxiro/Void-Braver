#include "LoginState.h"
#include "Core/StateManager.h"
#include "HubState.h" // Supaya bisa pindah ke HubState
#include <iostream>
#include <limits>

void LoginState::init() {
    pinInput = 0;
    isLoginSuccess = false;
    std::cout << "\n=== SELAMAT DATANG DI GAME ENGINE ===\n";
}

void LoginState::handleInput(StateManager& stateManager) {
    std::cout << "Masukkan PIN (ketik 123) untuk login, atau 0 untuk keluar: ";
    std::cin >> pinInput;

    // Cek jika pemain mengetik huruf/simbol (bukan angka)
    if (std::cin.fail()) {
        std::cin.clear(); // Bersihkan status error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang hurufnya
        pinInput = -1; // Set ke angka salah
    }
}

void LoginState::update(StateManager& stateManager) {
    if (pinInput == 123) {
        isLoginSuccess = true;
        pinInput = 0; // Reset input
        
        // Pindah ke layar HubState (HubState ditaruh di atas LoginState)
        stateManager.pushState(std::make_unique<HubState>());
    } 
    else if (pinInput == 0) {
        std::cout << "Menutup layar login...\n";
        stateManager.popState(); // Buang layar login, karena stack kosong, game akan tertutup
    }
}

void LoginState::render() {
    if (pinInput != 123 && pinInput != 0 && pinInput != -1) {
        std::cout << "[Sistem] PIN salah. Silakan coba lagi.\n";
    } else if (pinInput == -1) {
        std::cout << "[Sistem] Masukkan angka, bukan huruf!\n";
    }
}