#include "states/ui_helpers.h"

#include "database.h"
#include "states/inventory_helpers.h"
#include "states/player_stats_helpers.h"
#include "utils/console_ui.h"

#include <iostream>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void printHubHeader(const GameContext &ctx)
    {
        cout << colorText(MENU_SEPARATOR, Color::Blue) << '\n';
        cout << colorText("Character", Color::Yellow, true) << ": " << ctx.player.name
             << " | Class: " << ctx.player.class_id
             << " | Level: " << ctx.player.level << '\n';
        cout << colorText("Gold", Color::Yellow, true) << ": " << ctx.player.gold
             << " | EXP: " << ctx.player.current_exp << " / "
             << expRequiredForNextLevel(ctx, ctx.player.level + 1) << '\n';
        cout << colorText("HP", Color::Green, true) << ": " << ctx.player.hp << "/" << ctx.player.max_hp
             << " | " << colorText("MP", Color::Cyan, true) << ": " << ctx.player.mp << "/" << ctx.player.max_mp << '\n';
        cout << "STR: " << ctx.player.stats.str << " | INT: " << ctx.player.stats.intl
             << " | AGI: " << ctx.player.stats.agi << " | VIT: " << ctx.player.stats.vit
             << " | Stat Points: " << ctx.player.stat_points << '\n';
        cout << "ATK: " << calculatePlayerAttack(ctx)
             << " | DEF: " << calculatePlayerDefense(ctx)
             << " | Speed: " << calculatePlayerSpeed(ctx) << '\n';
        cout << colorText(MENU_SEPARATOR, Color::Blue) << '\n';
    }

    void printStateHeader(const GameContext &ctx, const string &title)
    {
        printHubHeader(ctx);
        printTitleBox(title);
    }

    void showItemDetails(const GameContext &ctx, const json &item, int ownedCount, int equippedCount)
    {
        const string itemId = item.value("id", string("-"));
        const string specialEffect = item.value("special_effect", string("null"));

        cout << colorText(item.value("name", itemId), Color::Yellow, true) << '\n';
        cout << colorText("ID", Color::Cyan, true) << "          : " << itemId << '\n';
        cout << colorText("Category", Color::Cyan, true) << "    : " << item.value("category", string("-")) << '\n';
        cout << colorText("Description", Color::Cyan, true) << " : " << item.value("description", string("-")) << '\n';
        cout << colorText("Tier", Color::Cyan, true) << "        : " << item.value("tier_level", 0) << '\n';
        cout << colorText("Required", Color::Cyan, true) << "    : " << item.value("required_class_id", string("-")) << '\n';
        cout << colorText("Buy / Sell", Color::Cyan, true) << "  : " << item.value("buy_price", 0)
             << " / " << item.value("sell_price", 0) << '\n';

        if (ownedCount >= 0)
            cout << colorText("Owned", Color::Cyan, true) << "       : " << ownedCount << '\n';
        if (equippedCount >= 0)
            cout << colorText("Equipped", Color::Cyan, true) << "    : " << equippedCount << '\n';

        cout << colorText("Stats", Color::Cyan, true) << "       : "
             << "ATK +" << item["stats"].value("atk_bonus", 0)
             << " | DEF +" << item["stats"].value("def_bonus", 0)
             << " | HP +" << item["stats"].value("hp_bonus", 0)
             << " | MP +" << item["stats"].value("mp_bonus", 0) << '\n';

        if (!specialEffect.empty() && specialEffect != "null")
            cout << colorText("Effect", Color::Cyan, true) << "      : " << specialEffect << '\n';

        if (item.contains("crafting") && item["crafting"].contains("ingredients"))
        {
            cout << colorText("Crafting", Color::Cyan, true) << "    :\n";
            for (const auto &ingredient : item["crafting"]["ingredients"])
            {
                const string ingredientId = ingredient.value("item_id", string("-"));
                const int need = ingredient.value("quantity", 0);
                const int own = inventoryCount(ctx.player, ingredientId);
                cout << "  - " << itemDisplayName(ctx, ingredientId) << " : " << own << "/" << need << '\n';
            }
        }

        printLine('=');
    }

    string unlockedTierName(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        if (!cls || !cls->contains("tiers") || !(*cls)["tiers"].is_array())
            return "-";

        string highest = cls->value("name", string("-"));
        for (const auto &tier : (*cls)["tiers"])
        {
            if (ctx.player.level >= tier.value("level", 999))
                highest = tier.value("name", highest);
        }
        return highest;
    }

    void printClassDetails(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        if (!cls)
            return;

        cout << "Current Tier: " << unlockedTierName(ctx) << '\n';
        if (cls->contains("base_damage_formula"))
            cout << "Base Formula: " << (*cls)["base_damage_formula"].get<string>() << '\n';

        if (cls->contains("tiers") && (*cls)["tiers"].is_array())
        {
            cout << "Tier Progress:\n";
            for (const auto &tier : (*cls)["tiers"])
            {
                const bool unlocked = ctx.player.level >= tier.value("level", 999);
                cout << " - Lv " << tier.value("level", 0) << " -> " << tier.value("name", string())
                     << (unlocked ? " [Unlocked]" : "") << '\n';
            }
        }
    }

    string itemDisplayName(const GameContext &ctx, const string &itemId)
    {
        const auto *item = getItemById(ctx, itemId);
        if (!item)
            return itemId;
        return item->value("name", itemId);
    }

    void showCharacterOverview(const GameContext &ctx)
    {
        printStateHeader(ctx, "CHARACTER MENU");
        printClassDetails(ctx);
        cout << "Equipment Bonus -> ATK: " << itemBonusFromEquipped(ctx, ctx.player, "atk")
             << ", DEF: " << itemBonusFromEquipped(ctx, ctx.player, "def")
             << ", HP: " << itemBonusFromEquipped(ctx, ctx.player, "hp")
             << ", MP: " << itemBonusFromEquipped(ctx, ctx.player, "mp") << '\n';
        cout << "Stat Growth Preview:\n";
        cout << " - STR +1 => sekitar +0 sampai +1 ATK dan +1 Max HP\n";
        cout << " - INT +1 => menaikkan damage skill dan sekitar +3 Max MP\n";
        cout << " - AGI +1 => sekitar +2 Speed dan +0.2% Crit\n";
        cout << " - VIT +1 => sekitar +0 sampai +1 DEF dan +4 Max HP\n";
        cout << MENU_SEPARATOR << '\n';
    }
} // namespace state_helpers
