#pragma once
#include "GameState.h"
#include <string>

/**
 * @class RegisterState
 * @brief Layar pertama saat game dibuka. Meminta pemain memasukkan PIN.
 */
class RegisterState : public GameState {
private:
    std::string inputUsername;
    std::string inputPassword;
    bool attemptLogin;
    bool isLoggedIn;

public:
    RegisterState();
    /// @brief Menyiapkan data awal saat layar login dibuka
    void init() override;

    /// @brief Membaca ketikan angka dari pemain
    void handleInput(StateManager& stateManager) override;

    /// @brief Mengecek apakah PIN benar dan memindahkan layar jika sukses
    void update(StateManager& stateManager) override;

    /// @brief Menampilkan teks UI ke layar terminal
    void render() override;
};