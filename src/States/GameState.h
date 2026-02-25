#pragma once

// Kita beri tahu dulu kalau ada class bernama StateManager (agar tidak error)
class StateManager;

/**
 * @class GameState
 * @brief Abstract base class for managing different game states in the application.
 *
 * GameState defines the interface for all game states using the State pattern.
 * Each derived state must implement three core methods for input handling, logic updates,
 * and rendering. The virtual destructor ensures proper cleanup of derived class resources.
 *
 * @note This is an abstract class and cannot be instantiated directly.
 * @see StateManager
 */
class GameState {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived state objects.
     *
     * Ensures that when a GameState object is deleted through a base class pointer,
     * the destructor of the derived class is properly invoked, preventing memory leaks.
     */
    virtual ~GameState() = default;

    /**
     * @brief Handles user input for the current game state.
     *
     * This method processes all input events (keyboard, mouse, etc.) relevant to
     * the current state and may request state transitions through the StateManager.
     *
     * @param stateManager Reference to the StateManager for performing state transitions.
     */
    virtual void handleInput(StateManager& stateManager) = 0;

    /**
     * @brief Updates the game logic for the current state.
     *
     * This method is called once per frame to update game entities, physics,
     * animations, and other state-specific logic.
     *
     * @param stateManager Reference to the StateManager for performing state transitions.
     */
    virtual void update(StateManager& stateManager) = 0;

    /**
     * @brief Renders the current game state to the screen.
     *
     * This method handles all drawing operations specific to this state,
     * including UI elements, game objects, and backgrounds.
     */
    virtual void render() = 0;
};