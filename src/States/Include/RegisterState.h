#pragma once
#include "GameState.h"
#include "Core/Include/StateManager.h"

class RegisterState : public GameState {
public:
    void init(StateManager& stateManager) override;
    void render() override;
    void update(StateManager& stateManager) override;
};