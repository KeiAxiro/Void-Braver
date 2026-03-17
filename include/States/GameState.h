// File: Core/GameState.h
#pragma once

class StateManager; // Forward declaration agar tidak error circular include

class GameState {
public:
    virtual ~GameState() = default;
    
// Di dalam GameState.h
virtual void init(StateManager& stateManager) = 0; // Tambahkan parameter ini
    
    // update() sekarang bertugas: meminta std::cin DAN memproses logikanya
    virtual void update(StateManager& stateManager) = 0; 
    
    // render() murni untuk std::cout (menampilkan UI ke terminal)
    virtual void render() = 0; 
};