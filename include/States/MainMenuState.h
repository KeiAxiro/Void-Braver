#pragma once
#include "Core/State.h"

namespace States {
    class MainMenuState : public Core::State {
    private:
        int selectedOption;
        const int MAX_OPTIONS = 3; // 0: New Game, 1: Load Game, 2: Exit

    public:
        MainMenuState();
        ~MainMenuState() override = default;

        void Init() override;
        void HandleInput(Core::StateManager& stateManager) override;
        void Update(Core::StateManager& stateManager) override;
        void Render() override;
    };
}