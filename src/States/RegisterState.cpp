#include "RegisterState.h"
#include "HubState.h"
#include "Core/StateManager.h"
#include "../lib/json.hpp" // Library JSON
#include <iostream>
#include <fstream>

using json = nlohmann::json;

RegisterState::RegisterState() : attemptLogin(false), isLoggedIn(false) {}

void RegisterState::init() {}

void RegisterState::handleInput(StateManager& stateManager) {
    // Karena CLI, input langsung mem-blok layar (wajar untuk game CLI)
    std::cout << "Masukkan Username (ketik 'exit' untuk keluar): ";
    std::cin >> inputUsername;

    if (inputUsername == "exit") {
        stateManager.popState(); // Keluar dari game
        return;
    }

    std::cout << "Masukkan Password: ";
    std::cin >> inputPassword;

    attemptLogin = true; // Tandai bahwa user mencoba login
}

void RegisterState::update(StateManager& stateManager) {
    if (attemptLogin && !isLoggedIn) {
        attemptLogin = false; // Reset trigger

        // Buka file JSON
        std::ifstream file("../data/saves.json");
        if (!file.is_open()) {
            std::cout << "[Error] Gagal membuka data/saves.json! Pastikan file ada.\n";
            return;
        }

        json saveData;
        file >> saveData;
        file.close();

        bool found = false;

        // Cek satu-satu user di dalam JSON
        for (const auto& user : saveData["users"]) {
            if (user["username"] == inputUsername && user["password"] == inputPassword) {
                std::cout << "\n[Sukses] Login Berhasil!\n";
                
                // Ambil data karakternya
                json playerData = user["player_data"];
                
                // Pindah ke HubState dengan MEMBAWA DATA (replace = false agar bisa logout)

                stateManager.pushState(std::make_unique<HubState>(inputUsername, playerData));
                
                found = true;
                break;
            }
        }

        if (!found) {
            std::cout << "\n[Gagal] Username atau Password salah!\n";
        }
    }
}

void RegisterState::render() {
    // Render dipanggil setiap frame, tapi karena CLI kita batasi teksnya agar tidak spam
    // Kita biarkan handleInput yang mengambil alih tampilan form login.
    if (!attemptLogin) {
        std::cout << "\n=== VOID-BRAVER LOGIN ===\n";
    }
}