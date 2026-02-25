#pragma once

// Kita beri tahu dulu kalau ada class bernama StateManager (agar tidak error)
class StateManager;

class GameState {
public:
    // Destruktor virtual agar saat layar ditutup, memorinya bersih
    virtual ~GameState() = default;

    // 3 Fungsi wajib dari dokumen riset:
    virtual void handleInput(StateManager& stateManager) = 0;
    virtual void update(StateManager& stateManager) = 0;
    virtual void render() = 0;
};