#pragma once
#include "GameState.h"

/**
 * @class HubState
 * @brief Layar menu utama (Hub) yang muncul setelah login sukses.
 */
class HubState : public GameState {
private:
    int menuChoice; ///< Menyimpan pilihan menu dari pemain

public:
    void init() override;
    void handleInput(StateManager& stateManager) override;
    void update(StateManager& stateManager) override;
    void render() override;
};