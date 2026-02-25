#pragma once
#include <stack>
#include <memory>
#include "States/GameState.h"

/**
 * @class StateManager
 * @brief Mengelola sistem state/layar game berbasis tumpukan (Stack LIFO).
 * * Menggunakan std::unique_ptr agar memori otomatis dibersihkan saat layar ditutup.
 * Layar yang paling atas (Top) adalah layar yang sedang aktif dimainkan.
 */
class StateManager {
private:
    /// Tumpukan memori pintar untuk menyimpan layar game
    std::stack<std::unique_ptr<GameState>> states;

public:
    /**
     * @brief Menambah layar baru ke puncak tumpukan.
     * @param newState Pointer unik dari GameState yang baru.
     */
    void pushState(std::unique_ptr<GameState> newState);
    
    /**
     * @brief Menghapus layar teratas (kembali ke layar sebelumnya).
     */
    void popState();

    /// @brief Meneruskan input ke layar yang sedang aktif (paling atas)
    void handleInput();

    /// @brief Meneruskan kalkulasi logika ke layar yang sedang aktif
    void update();

    /// @brief Meneruskan perintah render ke layar yang sedang aktif
    void render();

    /**
     * @brief Mengecek apakah masih ada layar di dalam tumpukan.
     * @return True jika ada state, False jika kosong (waktunya game tutup).
     */
    bool hasStates() const;
};