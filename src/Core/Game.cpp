#include "Core/Game.h"
#include "States/MainMenuState.h"

namespace Core {
    Game::Game() : isRunning(false) {}

    void Game::Init() {
        isRunning = true;
        
        stateManager.PushState(std::make_unique<States::MainMenuState>());
    }

    void Game::Run() {
        while (isRunning && stateManager.Running()) {
            ProcessInput();
            Update();
            if (isRunning && stateManager.Running()) {
                Render();
            }
        }
    }

    void Game::ProcessInput() {
        stateManager.HandleInput();
    }

    void Game::Update() {
        stateManager.Update();
    }

    void Game::Render() {
        stateManager.Render();
    }
}