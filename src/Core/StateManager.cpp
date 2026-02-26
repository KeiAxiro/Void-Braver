#include "Core/StateManager.h"

void StateManager::pushState(std::unique_ptr<GameState> state, bool replace)
{
    isAdding = true;
    isReplacing = replace;
    newState = std::move(state);
}

void StateManager::popState()
{
    isRemoving = true;
}

// void StateManager::clearState(){
//     while (!states.empty()) {
//         states.pop();
//     }
// }

void StateManager::processStateChanges()
{
    if (isRemoving && !states.empty())
    {
        states.pop();
        isRemoving = false;
    }

    if (isAdding && newState)
    {
        if (isReplacing && !states.empty())
        {
            states.pop();
        }

        states.push(std::move(newState));
        states.top()->init(*this); // Panggil init() saat state baru ditambahkan

        isAdding = false;
        isReplacing = false;
    }
}

void StateManager::update()
{
    if (!states.empty())
    {
        states.top()->update(*this);
    }
}

void StateManager::render()
{
    if (!states.empty())
    {
        states.top()->render();
    }
}

bool StateManager::hasStates() const
{
    return !states.empty();
}