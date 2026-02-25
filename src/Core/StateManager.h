#pragma once
#include <stack>
#include <memory>
#include "GameState.h"

class StateManager {
private:
    std::stack<std::unique_ptr<GameState>> states;
    std::unique_ptr<GameState> newState;
    
    bool isRemoving;
    bool isAdding;
    bool isReplacing;

public:
    StateManager();
    
    void pushState(std::unique_ptr<GameState> state, bool replace = false);
    void popState();
    void processStateChanges();
    
    void update();
    void render();
    
    bool hasStates() const;
};