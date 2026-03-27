#include "Core/StateManager.h"

namespace Core {
    StateManager::StateManager() : isRunning(true) {}

    void StateManager::PushState(std::unique_ptr<State> state) {
        states.push(std::move(state));
        states.top()->Init();
    }

    void StateManager::PopState() {
        if (!states.empty()) {
            states.pop();
        }
    }

    void StateManager::ChangeState(std::unique_ptr<State> state) {
        if (!states.empty()) {
            states.pop();
        }
        PushState(std::move(state));
    }

    void StateManager::HandleInput() {
        if (!states.empty()) {
            states.top()->HandleInput(*this);
        }
    }

    void StateManager::Update() {
        if (!states.empty()) {
            states.top()->Update(*this);
        }
    }

    void StateManager::Render() {
        if (!states.empty()) {
            states.top()->Render();
        }
    }

    bool StateManager::Running() const {
        return isRunning && !states.empty();
    }

    void StateManager::Quit() {
        isRunning = false;
    }
}