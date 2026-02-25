#include "MainMenuState.h"
#include "../Core/StateManager.h"
#include "LoginState.h"
#include "RegisterState.h"

MainMenuState::MainMenuState() {}

void MainMenuState::init() {
    // Tidak ada inisialisasi khusus saat ini
}

void MainMenuState::handleInput(StateManager& stateManager) {
    std::cout << "\nPilihan: [1] Login  [2] Register  [3] Exit\n";
    std::cout << "Masukkan pilihan: ";
    
    std::string input;
    std::cin >> input;

    if (input == "3") {
        std::cout << "Exit berhasil...\n";
        stateManager.popState(); // Menghancurkan MainMenuState, kembali ke Login (jika Login ada di bawahnya)
    } 
    else if (input == "2") {
        stateManager.pushState(std::make_unique<RegisterState>(), false); // Tambah layar Register tanpa replace
    }
    else if (input == "1") {
        stateManager.pushState(std::make_unique<LoginState>(), false); // Tambah layar Login tanpa replace
    }
}

void MainMenuState::update(StateManager& stateManager) {
    // Logika game utama (waktu berjalan, regenerasi HP, dll)
}

void MainMenuState::render() {
    std::cout << "\n====================================\n";
    std::cout << "              Main Menu\n";
    std::cout << "====================================\n";
    
}