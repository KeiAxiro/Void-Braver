#pragma once
#include "GameState.h"
#include "Core/Include/StateManager.h"
#include <vector>
#include <string>

class CharSelectionState : public GameState {
private:
    std::vector<std::string> characters; // Menyimpan daftar karakter dari database

public:
    void init(StateManager& stateManager) override;
    void render() override;
    void update(StateManager& stateManager) override;
};