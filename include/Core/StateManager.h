#pragma once
#include <stack>
#include <memory>
#include <string>
#include "States/GameState.h"
#include "GameContext.h"

class StateManager
{
private:
    std::stack<std::unique_ptr<GameState>> states;
    std::unique_ptr<GameState> newState;

    bool isRemoving{false};
    bool isAdding{false};
    bool isReplacing{false};

    std::string systemMessage;
    GameContext context;

public:
    StateManager() = default;

    void pushState(std::unique_ptr<GameState> state, bool replace = false);
    void popState();
    void clearState();
    void processStateChanges();

    void update();
    void render();

    [[nodiscard]] bool hasStates() const;

    GameContext &getContext() { return context; }
    void setNotify(const std::string &msg) { systemMessage = msg; }
    [[nodiscard]] std::string getNotify() const { return systemMessage; }
    void clearNotify() { systemMessage.clear(); }
};