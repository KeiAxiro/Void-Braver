#pragma once
#include "StateManager.h"

class Game {
private:
    bool isRunning;
    StateManager stateManager;

public:
    Game();
    ~Game();

    void init();
    void run();   // Loop utama digabung di sini
    void clean();
};