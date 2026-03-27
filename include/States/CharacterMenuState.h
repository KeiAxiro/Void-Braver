#pragma once
#include "Core/State.h"
#include "Core/GameContext.h"

namespace States {
    class CharacterMenuState : public Core::State {
    private:
        Core::GameContext& context;
        int currentView; 
        int selectedOption;

    public:
        CharacterMenuState(Core::GameContext& ctx);
        ~CharacterMenuState() override = default;

        void Init() override;
        void HandleInput(Core::StateManager& stateManager) override;
        void Update(Core::StateManager& stateManager) override;
        void Render() override;
    };
}