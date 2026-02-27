#pragma once
#include "Core/StateManager.h" // Atau sesuaikan dengan file yang berisi base class GameState

class RegisterState : public GameState
{
public:
    // Tidak perlu constructor khusus (GameContext* ctx)
    // Cukup gunakan fungsi bawaan dari GameState milikmu
    void init(StateManager &stateManager) override;
    void render() override;
    void update(StateManager &stateManager) override;
};