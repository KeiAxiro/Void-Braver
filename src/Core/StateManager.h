#pragma once
#include <stack>
#include <memory>
#include "../States/GameState.h"

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