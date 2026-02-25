#pragma once

class Game {
private:
    bool isRunning; // Penanda apakah game masih menyala atau sudah ditutup

public:
    Game();  // Konstruktor (dipanggil pertama kali saat game dibuat)
    ~Game(); // Destruktor (dipanggil saat game dihancurkan)

    void init();        // Menyiapkan game (memuat data, dsb)
    void handleInput(); // Membaca ketikan keyboard pemain
    void update();      // Menghitung logika (HP berkurang, dsb)
    void render();      // Menampilkan teks ke layar terminal
    void clean();       // Membersihkan memori sebelum keluar

    bool running() { return isRunning; } // Mengecek status game
};