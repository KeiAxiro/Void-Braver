#include "Game.h"
#include <iostream>
#include "../states/MainMenuState.h" // Nanti kita buat file ini

Game::Game() : isRunning(false) {}
Game::~Game() {}

void Game::init() {
    isRunning = true;
    
    // Masukkan layar pertama ke tumpukan
    stateManager.pushState(std::make_unique<MainMenuState>());
    stateManager.processStateChanges(); 
}

void Game::run() {
    init();

    // Game Loop sinkronous khusus CLI
    while (isRunning && stateManager.hasStates()) {
        stateManager.render();   // 1. Gambar teks ke layar
        stateManager.update();   // 2. Minta input & jalankan logika
        stateManager.processStateChanges(); // 3. Ganti layar jika diminta
    }

    clean();
}

void Game::clean() {
    std::cout << "\nMenyimpan progres... Keluar dari game dengan aman.\n";
}