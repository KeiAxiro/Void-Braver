#pragma once

#include "globals.h"

// Titik masuk utama untuk setiap state dalam game.
// Stack state di main loop akan memanggil fungsi-fungsi ini.
void runMainMenu(GameContext &ctx);
void questMenu(GameContext &ctx);
void runHub(GameContext &ctx);
void runBattle(GameContext &ctx);
