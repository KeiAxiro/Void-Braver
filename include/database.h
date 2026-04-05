#pragma once

#include <string>
#include <vector>

#include "globals.h"

bool loadGameData(GameContext &ctx);
bool loadSave(GameContext &ctx);
bool loadCharacter(GameContext &ctx, int index);
bool saveGame(const GameContext &ctx);
void createNewGame(GameContext &ctx, const std::string &playerName, const std::string &classId);

const json *findInArrayById(const json &arr, const std::string &id);
const json *getClassById(const GameContext &ctx, const std::string &classId);
const json *getItemById(const GameContext &ctx, const std::string &itemId);
const json *getSkillById(const GameContext &ctx, const std::string &skillId);
const json *getDungeonById(const GameContext &ctx, const std::string &dungeonId);
const json *getEnemyById(const GameContext &ctx, const std::string &enemyId);
const json *findDungeonDepth(const json &dungeon, int depth);

int expRequiredForNextLevel(const GameContext &ctx, int nextLevel);
int itemAtkBonus(const GameContext &ctx, const std::string &itemId);
int itemDefBonus(const GameContext &ctx, const std::string &itemId);
int itemHpBonus(const GameContext &ctx, const std::string &itemId);
int itemMpBonus(const GameContext &ctx, const std::string &itemId);
std::string itemCategory(const GameContext &ctx, const std::string &itemId);
std::string classPrimaryStat(const GameContext &ctx, const std::string &classId);
std::string itemRequiredClass(const GameContext &ctx, const std::string &itemId);
std::string slotFromCategory(const std::string &category);
bool isEquipmentCategory(const std::string &category);
int baseMaxHp(const Player &player);
int baseMaxMp(const Player &player);
int effectiveMaxHp(const GameContext &ctx, const Player &player);
int effectiveMaxMp(const GameContext &ctx, const Player &player);
void normalizePlayerResources(GameContext &ctx);

// int asInt(const json &j, int fallback = 0);
// std::string asString(const json &j, const std::string &fallback = "");
// // int equippedItemBonus(const GameContext &ctx, const Player &player, const string &bonusKey);
// void addStarterItem(Player &player, const std::string &itemId, int quantity, bool equipped, const std::string &slot);
// void giveStarterKit(GameContext &ctx);
// std::vector<std::string> collectCraftingCategories(const GameContext &ctx);
// std::vector<std::string> categoriesForShopMode(const GameContext &ctx, const std::string &mode);
// void showRecipeDetail(const GameContext &ctx, const json &item);

// void addStarterItem(Player &player, const string &itemId, int quantity, bool equipped, const string &slot);

// std::string resolveGameDataPath(const GameContext &ctx);
// std::string resolveSavePath(const GameContext &ctx);
// bool fileExists(const string &path);

// Player playerFromJson(const json &source);
// Player playerFromLegacyJson(const json &root);
// json playerToJson(const Player &player);

// void upsertCharacter(vector<Player> &characters, const Player &player);

// string sanitizeIdPart(const string &value);