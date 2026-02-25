#pragma once
#include "Core/GameState.h"
#include "Core/StateManager.h"
#include "Core/AccountManager.h"
#include "HubState.h"
#include <iostream>
#include <string>

class LoginState : public GameState {
private:
    AccountManager accManager;

public:
    void init() override {}

    void render() override {
        std::cout << "\n=== MENU LOGIN ===\n";
        std::cout << "Ketik 'batal' pada username untuk kembali.\n";
    }

    void update(StateManager& stateManager) override {
        std::string username, password;
        
        std::cout << "Username: ";
        std::cin >> username;
        if (username == "batal") {
            stateManager.popState(); return;
        }

        std::cout << "Password: ";
        std::cin >> password;

        if (accManager.loginAccount(username, password)) {
            std::cout << "\n[Success]: Login berhasil! Selamat datang, " << username << "!\n";
            // Nanti di sini kita ganti state ke Hub In-Game atau Character Selection!
            // stateManager.pushState(std::make_unique<CharacterSelectionState>(username), true);
            
            // Untuk sementara, kita pop agar kembali ke main menu
            stateManager.popState(); 
        } else {
            std::cout << "\n[Error]: Username atau Password salah!\n";
        }

        if (accManager.loginAccount(username, password)) {
            std::cout << "\n[Success]: Login berhasil! Selamat datang, " << username << "!\n";
            
            // Masuk ke menu pemilihan karakter (gunakan parameter replace=true 
            // agar layar Login dibuang dan diganti layar Select Character)
            stateManager.pushState(std::make_unique<HubState>(), false);
        } else {
            std::cout << "\n[Error]: Username atau Password salah!\n";
        }
    }
};