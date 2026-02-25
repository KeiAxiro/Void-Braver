#include "StateManager.h"

void StateManager::pushState(std::unique_ptr<GameState> newState) {
    states.push(std::move(newState));
    states.top()->init(); // Otomatis inisialisasi state baru
}

void StateManager::popState() {
    if (!states.empty()) {
        states.pop();
    }
}

void StateManager::handleInput() {
    if (!states.empty()) states.top()->handleInput(*this);
}

void StateManager::update() {
    if (!states.empty()) states.top()->update(*this);
}

void StateManager::render() {
    if (!states.empty()) states.top()->render();
}

bool StateManager::hasStates() const {
    return !states.empty();
}