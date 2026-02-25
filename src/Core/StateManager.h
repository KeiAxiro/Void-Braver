#pragma once
#include <stack>
#include <memory>
#include "States/GameState.h"

/**
 * @class StateManager
 * @brief Manages a stack-based game state system for handling multiple game screens/states.
 * 
 * StateManager implements a state management pattern using a stack of unique pointers to GameState objects.
 * It allows pushing new states onto the stack and popping states off, with automatic memory management
 * through std::unique_ptr. The manager processes input, updates, and renders the topmost active state.
 * 
 * @details
 * States are managed in LIFO (Last In, First Out) order, allowing for nested game screens
 * (e.g., menus over gameplay, pause screens, dialogs, etc.). Each state on the stack consumes
 * game loop cycles independently.
 * 
 * @author 
 * @version 1.0
 * @date 
 */

/**
 * @brief Pushes a new game state onto the state stack.
 * 
 * Adds a new GameState to the top of the stack. The new state becomes the active state
 * and will receive input, update, and render calls during the game loop.
 * 
 * @param newState A std::unique_ptr<GameState> representing the new state to push.
 * @note The StateManager takes ownership of the state object.
 * @see popState()
 */
void pushState(std::unique_ptr<GameState> newState);

/**
 * @brief Removes and destroys the topmost game state from the stack.
 * 
 * Pops the active state from the top of the stack. The state object is automatically
 * destroyed due to unique_ptr semantics. Control returns to the previous state on the stack.
 * 
 * @warning Calling popState() on an empty stack has undefined behavior.
 * @see hasStates()
 * @see pushState()
 */
void popState();

/**
 * @brief Processes input events for the topmost active state.
 * 
 * Delegates input handling to the currently active state on top of the stack.
 * 
 * @see update()
 * @see render()
 */
void handleInput();

/**
 * @brief Updates the logic of the topmost active state.
 * 
 * Delegates the update cycle to the currently active state on top of the stack.
 * 
 * @see handleInput()
 * @see render()
 */
void update();

/**
 * @brief Renders the topmost active state to the screen.
 * 
 * Delegates the rendering process to the currently active state on top of the stack.
 * 
 * @see handleInput()
 * @see update()
 */
void render();

/**
 * @brief Checks if there are any active states on the stack.
 * 
 * @return @c true if at least one state exists on the stack, @c false otherwise.
 * @see pushState()
 * @see popState()
 */

class StateManager {
private:
    // Ini adalah tumpukan memori pintarnya (std::unique_ptr)
    std::stack<std::unique_ptr<GameState>> states;

public:
    // Menambah layar baru ke atas tumpukan
    void pushState(std::unique_ptr<GameState> newState);
    
    // Membuang layar teratas (kembali ke layar sebelumnya)
    void popState();

    // Meneruskan siklus game ke layar teratas
    void handleInput();
    void update();
    void render();

    // Cek apakah masih ada layar yang menyala
    bool hasStates() const;
};