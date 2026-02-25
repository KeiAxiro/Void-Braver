#include "StateManager.h"

StateManager::StateManager() : isRemoving(false), isAdding(false), isReplacing(false) {}

void StateManager::pushState(std::unique_ptr<GameState> state, bool replace) {
    isAdding = true;
    isReplacing = replace;
    newState = std::move(state); // Simpan layar baru di ruang tunggu
}

void StateManager::popState() {
    isRemoving = true; // Angkat bendera bahwa layar ingin dihapus
}

void StateManager::processStateChanges() {
    // 1. Eksekusi penghapusan layar jika ada permintaan
    if (isRemoving && !states.empty()) {
        states.pop();
        isRemoving = false; // Turunkan bendera
    }

    // 2. Eksekusi penambahan layar jika ada permintaan
    if (isAdding) {
        // Jika mode 'replace' aktif, buang layar teratas dulu sebelum ditimpa
        if (isReplacing && !states.empty()) {
            states.pop();
        }
        states.push(std::move(newState));
        states.top()->init(); // Inisialisasi layar secara aman
        isAdding = false;     // Turunkan bendera
    }
}

void StateManager::handleInput() {
    if (!states.empty()) states.top()->handleInput(*this);
}

void StateManager::update() {
    if (!states.empty()) states.top()->update(*this);
}

void StateManager::render() {
    if (!states.empty()) states.top()->render();
}

bool StateManager::hasStates() const {
    return !states.empty();
}