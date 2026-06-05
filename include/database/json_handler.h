#pragma once

#include <string>

#include "globals.h"

bool loadGameData(GameContext &ctx);
bool loadSave(GameContext &ctx);
bool loadCharacter(GameContext &ctx, int index);
bool saveGame(const GameContext &ctx);
bool deleteCharacter(GameContext &ctx, int index);
void createNewGame(GameContext &ctx, const std::string &playerName, const std::string &classId);
