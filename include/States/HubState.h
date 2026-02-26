#pragma once
#include "GameState.h"
#include "Core/StateManager.h"

class HubState : public GameState
{
public:
    void init(StateManager &stateManager) override;
    void render() override;
    void update(StateManager &stateManager) override;
};