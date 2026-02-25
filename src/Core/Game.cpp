#include "Game.h"
#include "StateManager.h"
#include "States/LoginState.h" // Nanti kamu bisa buat file ini
#include <iostream>

Game::Game() : isRunning(false) {}
Game::~Game() {}

void Game::init() {
    isRunning = true;
    
    // Contoh: Masukkan state pertama kamu di sini.
    // stateManager.pushState(std::make_unique<LoginState>());
    std::cout << "Game Engine Berhasil Diinisialisasi!\n";
    stateManager.pushState(std::make_unique<LoginState>()); // Mulai dengan layar login
}

void Game::handleInput() {
    stateManager.handleInput();
}

void Game::update() {
    stateManager.update();

    // Jika tumpukan layar habis, matikan game
    if (!stateManager.hasStates()) {
        isRunning = false; 
    }
}

void Game::render() {
    stateManager.render();
}

void Game::clean() {
    std::cout << "\nMembersihkan memori... Game dimatikan dengan aman. Sampai jumpa!\n";
}