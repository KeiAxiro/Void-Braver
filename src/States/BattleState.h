#pragma once
#include "GameState.h"
#include "Entities/Player.h" // Wajib include ini agar BattleState tahu bentuk Player

class BattleState : public GameState {
private:
    Player myPlayer;  ///< Nah, di sinilah myPlayer akhirnya dideklarasikan!
    int menuChoice;   ///< Pilihan menu saat bertarung

public:
    void init() override;
    void handleInput(StateManager& stateManager) override;
    void update(StateManager& stateManager) override;
    void render() override;
};