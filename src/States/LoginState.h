#pragma once
#include "GameState.h"

/**
 * @class LoginState
 * @brief Layar pertama saat game dibuka. Meminta pemain memasukkan PIN.
 */
class LoginState : public GameState {
private:
    int pinInput;         ///< Menyimpan input angka dari pemain
    bool isLoginSuccess;  ///< Menandai apakah login berhasil

public:
    /// @brief Menyiapkan data awal saat layar login dibuka
    void init() override;

    /// @brief Membaca ketikan angka dari pemain
    void handleInput(StateManager& stateManager) override;

    /// @brief Mengecek apakah PIN benar dan memindahkan layar jika sukses
    void update(StateManager& stateManager) override;

    /// @brief Menampilkan teks UI ke layar terminal
    void render() override;
};