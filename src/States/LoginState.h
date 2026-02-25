#pragma once
#include "GameState.h" // 1. Bawa buku menu induknya

// 2. Beri tahu C++ bahwa LoginState adalah turunan resmi dari GameState
class LoginState : public GameState {
public:
    // Tambahkan kata 'override' untuk memastikan kita menimpa fungsi aslinya dengan benar
    void handleInput(StateManager& stateManager) override;
    void update(StateManager& stateManager) override;
    void render() override;
};