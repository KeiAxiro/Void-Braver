#pragma once
#include "../core/GameState.h"
#include "../core/StateManager.h"
#include "../core/AccountManager.h"
// #include "CharacterCreationState.h" // Nanti kita buat
// #include "InGameState.h"            // Nanti kita buat
#include <iostream>
#include <string>
#include <vector>

class CharacterSelectionState : public GameState {
private:
    std::string loggedInUser;
    AccountManager accManager;
    std::vector<std::string> characters;

public:
    // Constructor menerima username agar tahu siapa yang sedang login
    CharacterSelectionState(const std::string& username) : loggedInUser(username) {}

    void init() override {
        // Ambil daftar karakter dari JSON saat layar ini dimuat
        characters = accManager.getCharacterList(loggedInUser);
    }

    void render() override {
        std::cout << "\n=== DAFTAR KARAKTER (" << loggedInUser << ") ===\n";
        
        if (characters.empty()) {
            std::cout << "[!] Kamu belum memiliki karakter.\n";
        } else {
            for (size_t i = 0; i < characters.size(); ++i) {
                std::cout << i + 1 << ". " << characters[i] << "\n";
            }
        }
        
        std::cout << "------------------------------------\n";
        std::cout << "N. Buat Karakter Baru\n";
        std::cout << "X. Logout (Kembali ke Main Menu)\n";
        std::cout << "Pilihan: ";
    }

    void update(StateManager& stateManager) override {
        std::string input;
        std::cin >> input;

        if (input == "N" || input == "n") {
            std::cout << "\n[Sistem]: Masuk ke menu pembuatan karakter...\n";
            // stateManager.pushState(std::make_unique<CharacterCreationState>(loggedInUser));
        } 
        else if (input == "X" || input == "x") {
            std::cout << "\n[Sistem]: Logout berhasil.\n";
            stateManager.popState(); // Kembali ke MainMenuState
        } 
        else {
            // Cek apakah input adalah angka dan valid sesuai daftar karakter
            try {
                int choice = std::stoi(input);
                if (choice > 0 && choice <= characters.size()) {
                    std::cout << "\n[Sistem]: Memasuki dunia dengan karakter " << characters[choice - 1] << "...\n";
                    
                    // Nanti di sini kita load full data karakter dan masuk ke game:
                    // stateManager.pushState(std::make_unique<InGameState>(loggedInUser, characters[choice - 1]));
                } else {
                    std::cout << "\n[Sistem]: Pilihan karakter tidak valid!\n";
                }
            } catch (...) {
                std::cout << "\n[Sistem]: Input tidak valid!\n";
            }
        }
    }
};