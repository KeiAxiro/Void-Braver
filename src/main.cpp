#include "Core/Game.h"
#include <thread>  // Untuk sleep
#include <chrono>  // Untuk mengatur waktu

int main() {
    // 1. Buat mesin gamenya
    Game game;

    // 2. Nyalakan dan siapkan mesinnya
    game.init();

    // 3. JANTUNG GAME (Game Loop): Terus berputar selama game berjalan
    while (game.running()) {
        game.handleInput(); // Baca perintah pemain
        game.update();      // Hitung efek perintahnya
        game.render();      // Tampilkan hasilnya
        
        // Catatan: Di game nyata, kamu butuh fungsi "Sleep" atau "Delta Time" 
        // di sini agar CPU tidak terpakai 100%.
        // Istirahatkan CPU selama ~16 milidetik (60 Frame per detik)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // 4. Bersihkan sisa data saat game keluar
    game.clean();

    return 0;
}