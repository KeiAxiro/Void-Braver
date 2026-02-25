#include "Game.h"
#include "StateManager.h"
#include "States/LoginState.h"
#include <iostream>

/// Instance global StateManager untuk mengelola state/layar dalam game
StateManager stateManager;

/// @class Game
/// @brief Kelas utama yang mengatur alur permainan
/// 
/// Game bertanggung jawab untuk menginisialisasi, menangani input, 
/// update logika, render, dan membersihkan sumber daya game.
/// 
/// @details Menggunakan StateManager untuk mengelola stack state/layar.
/// State paling atas dalam stack akan diproses pertama kali.
/// 
/// @example
/// Game game;
/// game.init();
/// while(game.isRunning) {
///     game.handleInput();
///     game.update();
///     game.render();
/// }
/// game.clean();

Game::Game() { isRunning = false; }
Game::~Game() {}

/// @brief Inisialisasi game dan masukkan LoginState sebagai state awal
void Game::init() {
    isRunning = true;
    
    /// Saat game pertama kali dijalankan, masukkan LoginState ke dalam stack
    /// untuk menampilkan layar login sebagai state pertama
    stateManager.pushState(std::make_unique<LoginState>());
}

/// @brief Menangani input dari user
/// @details Delegasikan proses input ke StateManager, yang akan diteruskan
/// ke state paling atas dalam stack
void Game::handleInput() {
    stateManager.handleInput();
}

/// @brief Update logika game setiap frame
/// @details StateManager memproses update dari state paling atas.
/// Jika stack state kosong, game akan dihentikan
void Game::update() {
    stateManager.update();

    /// Jika tidak ada state dalam stack, hentikan game
    if (!stateManager.hasStates()) {
        isRunning = false; 
    }
}

/// @brief Render/gambar semua elemen visual game
/// @details StateManager memanggil render dari state paling atas
void Game::render() {
    stateManager.render();
}

/// @brief Membersihkan dan menutup game dengan aman
void Game::clean() {
    std::cout << "\nGame dimatikan dengan aman. Sampai jumpa!\n";
}