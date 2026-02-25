#pragma once
#include "GameState.h"
#include "Entities/Player.h" // Masukkan struct Player

class InventoryState : public GameState {
private:
    Player* playerRef; // Pointer untuk memegang akses ke data pemain yang asli

public:
    // Constructor yang memaksa siapa pun yang memanggil tas ini, wajib memberikan data pemain
    InventoryState(Player* player); 

    void init() override;
    void handleInput(StateManager& stateManager) override;
    void update(StateManager& stateManager) override;
    void render() override;
};