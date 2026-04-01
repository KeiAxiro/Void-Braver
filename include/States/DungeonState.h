#pragma once
#include "Core/State.h"
#include "Core/GameContext.h"
#include <string>
#include <vector>

namespace States {

    struct DungeonData {
        std::string name;
        std::string theme;
        std::string boss;
        std::string reward;
    };

    class DungeonState : public Core::State {
    private:
        Core::GameContext& context;
        int currentMenu; // 0: Select Dungeon, 1: Select Depth, 2: Exploring
        int selectedDungeonID;
        int selectedDepth;
        int selectedOption;
        int explorationProgress; // Variabel baru untuk melacak progres
        std::string logMessage;
        std::vector<DungeonData> dungeonList;

    public:
        DungeonState(Core::GameContext& ctx);
        ~DungeonState() override = default;

        void Init() override;
        void HandleInput(Core::StateManager& stateManager) override;
        void Update(Core::StateManager& stateManager) override;
        void Render() override;
    };
}