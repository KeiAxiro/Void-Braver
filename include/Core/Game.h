#pragma once
#include "Core/StateManager.h"

namespace Core {
    class Game {
    private:
        StateManager stateManager;
        bool isRunning;

        void ProcessInput();
        void Update();
        void Render();

    public:
        Game();
        ~Game() = default;

        void Init();
        void Run();
    };
}