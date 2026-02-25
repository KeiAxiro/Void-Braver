// File: Core/GameState.h
#pragma once

class StateManager; // Forward declaration agar tidak error circular include

class GameState {
public:
    virtual ~GameState() = default;
    
    virtual void init() = 0; 
    
    // update() sekarang bertugas: meminta std::cin DAN memproses logikanya
    virtual void update(StateManager& stateManager) = 0; 
    
    // render() murni untuk std::cout (menampilkan UI ke terminal)
    virtual void render() = 0; 
};