#include "Include/Game.h"
#include <iostream>
#include "States/Include/MainMenuState.h" // Atau .h jika sudah dipisah
#include "Utils/ConsoleUI.h" // 

// 1. HARUS ADA: Constructor
Game::Game() : isRunning(false) {}

// 3. HARUS ADA: Fungsi Init
void Game::init() {
    isRunning = true;
    // Berikan state awal
    stateManager.pushState(std::make_unique<MainMenuState>());
    stateManager.processStateChanges(); 
}

// 4. HARUS ADA: Fungsi Clean
void Game::clean() {
    std::cout << "\n[Sistem]: Menyimpan data dan keluar...\n";
}

void Game::run() {
    init(); 

    while (isRunning && stateManager.hasStates()) {
        // [OPSIONAL] Bersihkan terminal tiap frame jika kamu punya fungsi clearScreen
        ConsoleUI::clearScreen(); 

        // --- TAMBAHKAN BLOK KODE INI ---
        // Cek apakah ada notifikasi/pesan sistem dari State sebelumnya
        if (!stateManager.getNotify().empty()) {
            std::cout << "\n" << stateManager.getNotify() << "\n";
            stateManager.clearNotify(); // Wajib di-clear agar tidak muncul terus-menerus
        }
        // -------------------------------

        // Render UI dari State yang sedang aktif
        stateManager.render();
        
        // Minta input dan proses logika
        stateManager.update();
        
        // Ganti state jika ada perintah pindah menu
        stateManager.processStateChanges();
    }

    clean(); 
}