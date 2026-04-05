#include "states/dungeon_features.h"

#include "database.h"
#include "states/inventory_helpers.h"
#include "states/ui_helpers.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace state_helpers
{
    bool canCraftItem(const GameContext &ctx, const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].is_object() || !item["crafting"].contains("ingredients"))
            return false;

        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const string itemId = ingredient.value("item_id", string());
            const int quantity = ingredient.value("quantity", 0);
            if (inventoryCount(ctx.player, itemId) < quantity)
                return false;
        }

        return true;
    }

    int ownedIngredientCount(const GameContext &ctx, const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].contains("ingredients"))
            return 0;

        int owned = 0;
        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const string itemId = ingredient.value("item_id", string());
            owned += min(inventoryCount(ctx.player, itemId), ingredient.value("quantity", 0));
        }
        return owned;
    }

    int totalIngredientNeed(const json &item)
    {
        if (!item.contains("crafting") || !item["crafting"].contains("ingredients"))
            return 0;

        int total = 0;
        for (const auto &ingredient : item["crafting"]["ingredients"])
            total += ingredient.value("quantity", 0);
        return total;
    }

    vector<string> collectCraftingCategories(const GameContext &ctx)
    {
        vector<string> categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const string categoryName = category.get<string>();
            bool exists = false;

            for (const auto &item : ctx.gameData["items"])
            {
                if (item.value("category", string()) == categoryName &&
                    item.contains("crafting") &&
                    item["crafting"].contains("ingredients"))
                {
                    exists = true;
                    break;
                }
            }

            if (exists)
                categories.push_back(categoryName);
        }

        return categories;
    }

    vector<string> categoriesForShopMode(const GameContext &ctx, const string &mode)
    {
        vector<string> categories;
        if (!ctx.gameData.contains("item_category") || !ctx.gameData["item_category"].is_array())
            return categories;

        for (const auto &category : ctx.gameData["item_category"])
        {
            const string categoryName = category.get<string>();
            bool exists = false;

            for (const auto &item : ctx.gameData["items"])
            {
                if (item.value("category", string()) != categoryName)
                    continue;

                if (mode == "buy" && item.value("buy_price", 0) > 0)
                {
                    exists = true;
                    break;
                }

                if (mode == "sell" && item.value("sell_price", 0) > 0 &&
                    inventoryCount(ctx.player, item.value("id", string())) > 0)
                {
                    exists = true;
                    break;
                }
            }

            if (exists)
                categories.push_back(categoryName);
        }

        return categories;
    }

    void showRecipeDetail(const GameContext &ctx, const json &item)
    {
        cout << MENU_SEPARATOR << '\n';
        cout << item.value("name", string()) << '\n';
        cout << "Category: " << item.value("category", string()) << '\n';
        cout << "Tier: " << item.value("tier_level", 0) << '\n';
        cout << "Required Class: " << item.value("required_class_id", string("-")) << '\n';
        cout << "Stats -> ATK +" << item["stats"].value("atk_bonus", 0)
             << ", DEF +" << item["stats"].value("def_bonus", 0)
             << ", HP +" << item["stats"].value("hp_bonus", 0)
             << ", MP +" << item["stats"].value("mp_bonus", 0) << '\n';
        cout << "Ingredients:\n";

        for (const auto &ingredient : item["crafting"]["ingredients"])
        {
            const string ingredientId = ingredient.value("item_id", string());
            const int need = ingredient.value("quantity", 0);
            const int own = inventoryCount(ctx.player, ingredientId);
            cout << " - " << itemDisplayName(ctx, ingredientId) << " : " << own << "/" << need << '\n';
        }

        cout << MENU_SEPARATOR << '\n';
    }
} // namespace state_helpers
