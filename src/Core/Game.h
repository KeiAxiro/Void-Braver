#pragma once
#include "StateManager.h"

/**
 * @class Game
 * @brief Kelas utama yang mengatur siklus hidup (Game Loop) dari awal hingga akhir.
 */
class Game {
private:
    bool isRunning;           ///< Penanda apakah game masih menyala
    StateManager stateManager; ///< Manager layar (Sekarang menjadi bagian dari Game, bukan global)

public:
    Game();
    ~Game();

    void init();        ///< Menyiapkan mesin dan memuat state pertama
    void handleInput(); ///< Membaca input pemain per frame
    void update();      ///< Menghitung logika game per frame
    void render();      ///< Menggambar hasil ke layar per frame
    void clean();       ///< Membersihkan sisa data sebelum keluar

    /// @brief Mengecek status mesin game
    bool running() const { return stateManager.hasStates(); } // Game tetap berjalan selama masih ada state di dalam tumpukan
};