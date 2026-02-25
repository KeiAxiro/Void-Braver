#pragma once
#include "../core/GameState.h"
#include "../core/StateManager.h"
#include "LoginState.h"    // TAMBAHKAN INI
#include "RegisterState.h" // TAMBAHKAN INI
#include <iostream>
#include <string>

class MainMenuState : public GameState {
public:
    void init() override {}

    void render() override {
        std::cout << "\n====================================\n";
        std::cout << "      CLI RPG GAME - MAIN MENU      \n";
        std::cout << "====================================\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "3. Exit\n";
        std::cout << "Pilihan: ";
    }

    void update(StateManager& stateManager) override {
        std::string input;
        std::cin >> input;

        if (input == "1") {
            // Push layar Login
            stateManager.pushState(std::make_unique<LoginState>());
        } 
        else if (input == "2") {
            // Push layar Register
            stateManager.pushState(std::make_unique<RegisterState>());
        } 
        else if (input == "3") {
            std::cout << "\n[Sistem]: Keluar dari game...\n";
            stateManager.popState(); 
        } 
        else {
            std::cout << "\n[Sistem]: Input tidak valid!\n";
        }
    }
};