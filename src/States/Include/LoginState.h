#pragma once
#include "GameState.h"
#include "Core/Include/StateManager.h"

class LoginState : public GameState {
public:
    void init(StateManager& stateManager) override;
    void render() override;
    void update(StateManager& stateManager) override;
};