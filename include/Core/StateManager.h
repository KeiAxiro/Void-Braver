#pragma once
#include <memory>
#include <stack>
#include "Core/State.h"
#include "Core/GameContext.h"

namespace Core {
    class StateManager {
    private:
        std::stack<std::unique_ptr<State>> states;
        bool isRunning;
        GameContext context;

    public:
        StateManager();

        void PushState(std::unique_ptr<State> state);
        void PopState();
        void ChangeState(std::unique_ptr<State> state);

        void HandleInput();
        void Update();
        void Render();

        bool Running() const;
        void Quit();

        GameContext& GetContext() { return context; }
    };
}