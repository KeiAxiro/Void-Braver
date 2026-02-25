#include "HubState.h"
#include "../Core/StateManager.h"

HubState::HubState(const std::string& username, const json& charData) 
    : accountName(username), playerCharData(charData) {}

void HubState::init() {
    // Tidak ada inisialisasi khusus saat ini
}

void HubState::handleInput(StateManager& stateManager) {
    std::cout << "\nPilihan: [1] Jelajah  [2] Status  [3] Logout\n";
    std::cout << "Masukkan pilihan: ";
    
    std::string input;
    std::cin >> input;

    if (input == "3") {
        std::cout << "Logout berhasil...\n";
        stateManager.popState(); // Menghancurkan HubState, kembali ke Login (jika Login ada di bawahnya)
    } 
    else if (input == "2") {
        std::cout << "\n--- STATUS KARAKTER ---\n";
        std::cout << "Nama  : " << playerCharData["char_name"].get<std::string>() << "\n";
        std::cout << "Level : " << playerCharData["level"].get<int>() << "\n";
        std::cout << "HP    : " << playerCharData["hp"].get<int>() << "\n";
        std::cout << "Gold  : " << playerCharData["gold"].get<int>() << " G\n";
        std::cout << "-----------------------\n";
    }
}

void HubState::update(StateManager& stateManager) {
    // Logika game utama (waktu berjalan, regenerasi HP, dll)
}

void HubState::render() {
    std::cout << "\n====================================\n";
    std::cout << "        MARKAS UTAMA (HUB)\n";
    std::cout << "====================================\n";
    std::cout << "Selamat datang, Akun: [" << accountName << "]\n";
    std::cout << "Karakter Aktif: " << playerCharData["char_name"].get<std::string>() << "\n";
}