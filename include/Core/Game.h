#pragma once
#include "StateManager.h"

class Game {
private:
    bool isRunning{false};
    StateManager stateManager;

public:
    Game();
    ~Game() = default;

    void init();
    void run(); 
    void clean();
};