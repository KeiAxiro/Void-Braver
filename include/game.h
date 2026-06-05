#pragma once

#include "globals.h"

// Titik masuk aplikasi setelah fungsi main.
int startGame();

// Loop utama game. Dipisah supaya main.cpp tetap pendek dan mudah dibaca.
void runGame(GameContext &ctx);
