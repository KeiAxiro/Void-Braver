#pragma once
#include "Core/State.h"

namespace States {
    class CharSelectionState : public Core::State {
    private:
        int selectedClass;

    public:
        CharSelectionState();
        ~CharSelectionState() override = default;

        void Init() override;
        void HandleInput(Core::StateManager& stateManager) override;
        void Update(Core::StateManager& stateManager) override;
        void Render() override;
    };
}