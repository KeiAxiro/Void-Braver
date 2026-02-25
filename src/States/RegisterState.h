#pragma once
#include "Core/GameState.h"
#include "Core/StateManager.h"
#include "Core/AccountManager.h"
#include <iostream>
#include <string>

class RegisterState : public GameState {
private:
    AccountManager accManager;

public:
    void init() override {}

    void render() override {
        std::cout << "\n=== MENU REGISTER ===\n";
        std::cout << "Ketik 'batal' pada username untuk kembali.\n";
    }

    void update(StateManager& stateManager) override {
        std::string username, password;
        
        std::cout << "Masukkan Username: ";
        std::cin >> username;
        if (username == "batal") {
            stateManager.popState(); return;
        }

        std::cout << "Masukkan Password: ";
        std::cin >> password;

        if (accManager.registerAccount(username, password)) {
            std::cout << "\n[Success]: Registrasi berhasil! Silakan Login.\n";
            stateManager.popState(); // Kembali ke Main Menu
        } else {
            std::cout << "\n[Error]: Username sudah dipakai! Coba yang lain.\n";
        }
    }
};