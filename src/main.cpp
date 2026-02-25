#include "Core/Game.h"

int main() {
    // 1. Buat mesin gamenya
    Game game;

    // 2. Nyalakan dan siapkan mesinnya
    game.init();

    // 3. JANTUNG GAME: Selama game menyala, terus putar siklus ini
    while (game.running()) {
        game.handleInput(); // Baca perintah pemain
        game.update();      // Hitung efek perintahnya
        game.render();      // Tampilkan hasilnya
    }

    // 4. Jika siklus berhenti (pemain mengetik 'keluar'), bersihkan sisa data
    game.clean();

    return 0;
}