#pragma once
#include "GameState.h"
#include <string>
#include <iostream>


class MainMenuState : public GameState {
private:
    

public:
    // Constructor menerima data dari LoginState
    MainMenuState();

    void init() override;
    void handleInput(StateManager& stateManager) override;
    void update(StateManager& stateManager) override;
    void render() override;
};