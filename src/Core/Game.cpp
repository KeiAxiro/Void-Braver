#include "Game.h"
#include "StateManager.h"
#include "../States/LoginState.h"
#include <iostream>

// Bikin variabel StateManager di dalam file ini
StateManager stateManager;

Game::Game() { isRunning = false; }
Game::~Game() {}

void Game::init() {
    isRunning = true;
    
    // Saat game pertama kali nyala, masukkan Layar Login sebagai piring pertama!
    stateManager.pushState(std::make_unique<LoginState>());
}

void Game::handleInput() {
    stateManager.handleInput(); // Biarkan layar teratas yang mengurus input
}

void Game::update() {
    stateManager.update(); // Biarkan layar teratas yang berhitung

    // Jika tumpukan layarnya habis (kosong), berarti waktunya tutup game!
    if (!stateManager.hasStates()) {
        isRunning = false; 
    }
}

void Game::render() {
    stateManager.render(); // Biarkan layar teratas yang menggambar dirinya
}

void Game::clean() {
    std::cout << "\nGame dimatikan dengan aman. Sampai jumpa!\n";
}