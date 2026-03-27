#pragma once
#include "Core/State.h"
#include "Core/GameContext.h"
#include "Entities/Enemy.h"
#include <string>

namespace States {

    class BattleState : public Core::State {
    private:
        Core::GameContext& context;
        Entities::Enemy currentEnemy;
        
        int dungeonDepth;
        int battlePhase; 
        int currentMenu; 
        int selectedOption;
        
        bool playerShielding;
        bool enemyShielding;

        std::string battleLog;

        void spawnEnemy();
        void processEnemyTurn();
        void calculateRewards();

    public:
        BattleState(Core::GameContext& ctx, int depth);
        ~BattleState() override = default;

        void Init() override;
        void HandleInput(Core::StateManager& stateManager) override;
        void Update(Core::StateManager& stateManager) override;
        void Render() override;
    };
}