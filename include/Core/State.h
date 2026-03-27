#pragma once

namespace Core {
    class StateManager;

    class State {
    public:
        virtual ~State() = default;

        virtual void Init() = 0;
        virtual void HandleInput(StateManager& stateManager) = 0;
        virtual void Update(StateManager& stateManager) = 0;
        virtual void Render() = 0;
    };
}