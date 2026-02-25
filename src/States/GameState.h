#pragma once

class StateManager; // Forward declaration

/**
 * @class GameState
 * @brief Interface dasar (Blueprint) untuk semua layar/state di dalam game.
 * * Setiap layar (Menu, Gameplay, Pause) harus mewarisi kelas ini dan
 * mengimplementasikan fungsinya sendiri.
 */
class GameState {
public:
    virtual ~GameState() = default;

    /// @brief Dipanggil sekali saat state pertama kali dibuat
    virtual void init() = 0;

    /// @brief Membaca input dari pemain
    /// @param stateManager Referensi ke manager agar state bisa mengganti layar (misal: masuk ke layar gameplay)
    virtual void handleInput(StateManager& stateManager) = 0;

    /// @brief Memperbarui logika game di layar ini
    virtual void update(StateManager& stateManager) = 0;

    /// @brief Menampilkan visual ke layar terminal
    virtual void render() = 0;
};