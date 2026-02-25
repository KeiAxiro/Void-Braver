#include "InventoryState.h"
#include "Core/StateManager.h"
#include <iostream>
#include <limits>

// Simpan alamat memori pemain ke dalam playerRef
InventoryState::InventoryState(Player* player) : playerRef(player) {}

void InventoryState::init() {
    std::cout << "\n=== MEMBUKA TAS ===\n";
}

void InventoryState::handleInput(StateManager& stateManager) {
    int pilihan;
    std::cout << "1. Minum Potion (Sisa: " << playerRef->potionCount << ")\n";
    std::cout << "0. Tutup Tas (Kembali ke Battle)\n";
    std::cout << "Pilih: ";
    std::cin >> pilihan;

    if (pilihan == 1) {
        if (playerRef->potionCount > 0) {
            playerRef->potionCount -= 1;
            playerRef->hp += 20; // Tambah darah
            std::cout << "\n[Aksi] Glug glug glug! HP bertambah 20. Total HP: " << playerRef->hp << "\n";
        } else {
            std::cout << "\n[Aksi] Potion habis!\n";
        }
    } 
    else if (pilihan == 0) {
        stateManager.popState(); // Buang layar tas, kembali ke layar pertarungan
    }
}

// update() dan render() biarkan kosong untuk sementara
void InventoryState::update(StateManager& stateManager) {}
void InventoryState::render() {}