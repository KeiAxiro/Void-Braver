#pragma once
#include "Core/State.h"
#include "Core/GameContext.h"
#include <string>

namespace States {

    class DungeonState : public Core::State {
    private:
        Core::GameContext& context;
        int currentMenu; // 0: Select Depth, 1: Exploring
        int selectedDepth;
        int selectedOption;
        std::string logMessage;

    public:
        DungeonState(Core::GameContext& ctx);
        ~DungeonState() override = default;

        void Init() override;
        void HandleInput(Core::StateManager& stateManager) override;
        void Update(Core::StateManager& stateManager) override;
        void Render() override;
    };
}