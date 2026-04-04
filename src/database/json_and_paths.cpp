#include "database.h"

#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

namespace
{
    int asInt(const json &j, int fallback = 0)
    {
        if (j.is_number_integer())
            return j.get<int>();
        if (j.is_number_float())
            return static_cast<int>(j.get<double>());
        if (j.is_string())
        {
            try
            {
                return stoi(j.get<string>());
            }
            catch (...)
            {
                return fallback;
            }
        }
        if (j.is_boolean())
            return j.get<bool>() ? 1 : 0;
        return fallback;
    }

    string asString(const json &j, const string &fallback = "")
    {
        if (j.is_string())
            return j.get<string>();
        if (j.is_number_integer())
            return to_string(j.get<int>());
        if (j.is_number_float())
        {
            ostringstream oss;
            oss << j.get<double>();
            return oss.str();
        }
        if (j.is_boolean())
            return j.get<bool>() ? "true" : "false";
        return fallback;
    }

    bool fileExists(const string &path)
    {
        ifstream file(path);
        return file.good();
    }

    string resolveGameDataPath(const GameContext &ctx)
    {
        if (!ctx.gameDataPath.empty())
            return ctx.gameDataPath;
        return "data/game_data.json";
    }

    string resolveSavePath(const GameContext &ctx)
    {
        if (!ctx.savePath.empty())
            return ctx.savePath;
        return "data/save.json";
    }

    string sanitizeIdPart(const string &value)
    {
        string out;
        for (char ch : value)
        {
            unsigned char uch = static_cast<unsigned char>(ch);
            if (isalnum(uch))
                out.push_back(static_cast<char>(tolower(uch)));
            else if (isspace(uch) || ch == '-' || ch == '_')
                out.push_back('_');
        }

        if (out.empty())
            out = "hero";

        string compact;
        bool prevUnderscore = false;
        for (char ch : out)
        {
            if (ch == '_')
            {
                if (!prevUnderscore)
                    compact.push_back(ch);
                prevUnderscore = true;
                continue;
            }

            compact.push_back(ch);
            prevUnderscore = false;
        }

        if (!compact.empty() && compact.front() == '_')
            compact.erase(compact.begin());
        if (!compact.empty() && compact.back() == '_')
            compact.pop_back();
        return compact.empty() ? "hero" : compact;
    }

    int equippedItemBonus(const GameContext &ctx, const Player &player, const string &bonusKey)
    {
        int total = 0;
        for (const auto &entry : player.inventory)
        {
            if (!entry.equipped)
                continue;

            const auto *item = getItemById(ctx, entry.item_id);
            if (!item || !item->contains("stats") || !(*item)["stats"].is_object())
                continue;

            total += asInt((*item)["stats"].value(bonusKey, json(0)), 0);
        }
        return total;
    }

    void addStarterItem(Player &player, const string &itemId, int quantity, bool equipped, const string &slot)
    {
        if (itemId.empty() || quantity <= 0)
            return;

        InventoryEntry entry;
        entry.item_id = itemId;
        entry.quantity = quantity;
        entry.equipped = equipped;
        entry.slot = slot;
        player.inventory.push_back(entry);
    }

    void giveStarterKit(GameContext &ctx)
    {
        auto giveIfExists = [&](const string &itemId, int quantity, bool equipped = false)
        {
            const json *item = getItemById(ctx, itemId);
            if (item == nullptr)
                return;

            string slot;
            if (equipped)
                slot = slotFromCategory(item->value("category", string()));

            addStarterItem(ctx.player, itemId, quantity, equipped, slot);
        };

        giveIfExists("healing_potion", 3, false);
        giveIfExists("mana_elixir", 2, false);

        if (ctx.player.class_id == "knight")
        {
            giveIfExists("leather_coif", 1, true);
            giveIfExists("padded_tunic", 1, true);
            giveIfExists("rusty_greatsword", 1, true);
            giveIfExists("traveler_s_boots", 1, true);
        }
        else if (ctx.player.class_id == "sorcerer")
        {
            giveIfExists("apprentice_wand", 1, true);
            giveIfExists("novice_robe", 1, true);
        }
        else if (ctx.player.class_id == "wanderer")
        {
            giveIfExists("short_bow", 1, true);
            giveIfExists("leather_cap", 1, true);
            giveIfExists("rough_tunic", 1, true);
        }
    }
} // namespace
