#pragma once

#include <string>
#include <vector>

#include "battle_types.h"

namespace state_helpers
{
    EnemyInstance spawnEnemyForDepth(const GameContext &ctx, const json &dungeon, int depth);
    bool isDepthLevelAllowed(const json &depthRow, int level);
    void printDungeonLore(const json &depthRow);
    bool selectDungeonDepth(GameContext &ctx, bool enterBattleState);
    void chooseDungeon(GameContext &ctx);

    bool canCraftItem(const GameContext &ctx, const json &item);
    int ownedIngredientCount(const GameContext &ctx, const json &item);
    int totalIngredientNeed(const json &item);
    std::vector<std::string> collectCraftingCategories(const GameContext &ctx);
    std::vector<std::string> categoriesForShopMode(const GameContext &ctx, const std::string &mode);
    void showRecipeDetail(const GameContext &ctx, const json &item);
} // namespace state_helpers
