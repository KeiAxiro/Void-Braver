#include "states/character_features.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/inventory_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void allocateStats(GameContext &ctx)
    {
        while (true)
        {
            clearScreen();
            refreshPlayerResources(ctx);
            printStateHeader(ctx, "ALLOCATE STAT POINTS");
            cout << colorText("Available Points", Color::Yellow, true) << ": " << ctx.player.stat_points << "\n\n";
            cout << colorText("1. STR", Color::Red) << " (" << ctx.player.stats.str << ")\n";
            cout << colorText("2. INT", Color::Cyan) << " (" << ctx.player.stats.intl << ")\n";
            cout << colorText("3. AGI", Color::Green) << " (" << ctx.player.stats.agi << ")\n";
            cout << colorText("4. VIT", Color::Yellow) << " (" << ctx.player.stats.vit << ")\n";
            cout << colorText("5. Back", Color::Magenta) << '\n';
            cout << "Choose stat: ";

            const string choice = readLine();
            if (choice == "5")
                return;

            if (ctx.player.stat_points <= 0)
            {
                cout << "Tidak ada stat point tersisa.\n";
                waitForEnter();
                return;
            }

            int statIndex = 0;
            if (!tryParseInt(choice, statIndex) || statIndex < 1 || statIndex > 4)
            {
                cout << "Pilihan tidak valid.\n";
                waitForEnter();
                continue;
            }

            cout << "Masukkan jumlah point yang ingin dialokasikan: ";
            const string amountInput = readLine();
            int amount = 0;
            if (!tryParseInt(amountInput, amount) || amount <= 0)
            {
                cout << "Jumlah tidak valid.\n";
                waitForEnter();
                continue;
            }

            amount = min(amount, ctx.player.stat_points);
            if (statIndex == 1)
                ctx.player.stats.str += amount;
            else if (statIndex == 2)
                ctx.player.stats.intl += amount;
            else if (statIndex == 3)
                ctx.player.stats.agi += amount;
            else if (statIndex == 4)
                ctx.player.stats.vit += amount;

            ctx.player.stat_points -= amount;
            refreshPlayerResources(ctx);
            cout << "Stat berhasil dialokasikan. HP/MP maksimum ikut diperbarui.\n";
            waitForEnter();
        }
    }

    void skillMenu(const GameContext &ctx)
    {
        const auto *cls = getClassById(ctx, ctx.player.class_id);
        clearScreen();
        printStateHeader(ctx, "SKILL LIST");
        if (!cls || !cls->contains("skills") || !(*cls)["skills"].is_array())
        {
            cout << "Skill tidak tersedia.\n";
            waitForEnter();
            return;
        }

        int index = 1;
        for (const auto &skill : (*cls)["skills"])
        {
            cout << colorText(to_string(index++) + ". " + skill.value("name", string()), Color::Yellow, true) << '\n';
            cout << "   Mana: " << skill.value("mana_cost", 0)
                 << " | Cooldown: " << skill.value("cooldown_turns", 0)
                 << " | CD Saat Ini: " << getCooldown(ctx.player, skill.value("id", string())) << '\n';
            if (skill.contains("damage_formula"))
                cout << "   Formula: " << skill.value("damage_formula", string("-")) << '\n';
            cout << "   " << skill.value("mechanic_description", string("-")) << "\n\n";
        }

        printLine('=');
        waitForEnter();
    }
} // namespace state_helpers
