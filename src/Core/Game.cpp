#include "Game.h"
#include "StateManager.h"
#include "../States/MainMenuState.h" // Perhatikan path menggunakan ../
#include <iostream>

Game::Game() : isRunning(false) {}
Game::~Game() {}

void Game::init() {
    isRunning = true;
    std::cout << "Game Engine Berhasil Diinisialisasi!\n";
    
    // 1. Masukkan layar pertama ke "ruang tunggu"
    stateManager.pushState(std::make_unique<MainMenuState>());
    
    // 2. EKSEKUSI RUANG TUNGGU SEKARANG JUGA!
    // Wajib dipanggil agar MainMenuState langsung masuk ke tumpukan 
    // sebelum Game Loop pertama kali berputar.
    stateManager.processStateChanges(); 
}

void Game::handleInput() {
    stateManager.handleInput();
}

void Game::update() {
    // 1. Cek apakah tumpukan layar habis
    if (!stateManager.hasStates()) {
        isRunning = false; 
        return;
    }

    // 2. Jalankan logika layar yang sedang aktif saat ini
    // (Misal: MainMenuState mengecek input dari pemain)
    stateManager.update();

    // 3. EKSEKUSI RUANG TUNGGU DI SINI (SETELAH UPDATE SELESAI)
    // Jika update tadi meminta ganti layar, layar akan langsung terganti
    // sebelum masuk ke tahap render() atau balik meminta input di frame berikutnya.
    stateManager.processStateChanges(); 
}

void Game::render() {
    if (isRunning && stateManager.hasStates()) {
        stateManager.render();
    }
}

void Game::clean() {
    std::cout << "\nMembersihkan memori... Game dimatikan dengan aman. Sampai jumpa!\n";
}