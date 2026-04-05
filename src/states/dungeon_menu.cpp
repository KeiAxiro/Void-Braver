#include "states/dungeon_features.h"

#include "database.h"
#include "states/input_helpers.h"
#include "states/progress_helpers.h"
#include "states/ui_helpers.h"
#include "utils/console_ui.h"

#include <iostream>
#include <string>

using namespace std;
using namespace consoleui;

namespace state_helpers
{
    void printDungeonLore(const json &depthRow)
    {
        if (!depthRow.contains("tale") || !depthRow["tale"].is_object())
            return;

        const auto &tale = depthRow["tale"];
        cout << colorText("Chapter", Color::Magenta, true) << ": " << tale.value("chapter_name", std::string("-")) << '\n';
        cout << colorText("Title", Color::Cyan, true) << ": " << tale.value("depth_title", std::string("-")) << '\n';
        cout << colorText("Lore", Color::Yellow, true) << ": " << tale.value("lore_content", std::string("-")) << '\n';
    }

    bool selectDungeonDepth(GameContext &ctx, bool enterBattleState)
    {
        while (true)
        {
            clearScreen();
            printStateHeader(ctx, "ENTER DUNGEON");

            int index = 1;
            for (const auto &dungeon : ctx.gameData["dungeons"])
            {
                const bool locked = isFinalDungeonLocked(ctx, dungeon);
                const int unlockedDepth = getDungeonUnlockedDepth(ctx.player, dungeon);
                const int maxDepth = dungeonMaxDepth(dungeon);

                cout << colorText(to_string(index++) + ". " + dungeon.value("name", std::string()), Color::Yellow, true) << '\n';
                cout << "   Theme : " << dungeon.value("theme", std::string("-")) << '\n';
                cout << "   Boss  : " << dungeon.value("boss_name", std::string("-"))
                     << " | Orb: " << dungeon.value("reward_orb", std::string("-")) << '\n';
                cout << "   Depth : " << unlockedDepth << "/" << maxDepth;
                if (isDungeonCompleted(ctx.player, dungeon))
                    cout << " [Completed]";
                if (locked)
                    cout << " [Locked]";
                cout << "\n\n";
            }

            cout << "Pilih dungeon dengan nomor, atau c untuk kembali: ";
            const string input = toLower(readLine());
            if (input == "c")
                return false;

            int selected = 0;
            if (!tryParseInt(input, selected) || selected < 1 || selected > static_cast<int>(ctx.gameData["dungeons"].size()))
            {
                cout << "Pilihan dungeon tidak valid.\n";
                continue;
            }

            const json &dungeon = ctx.gameData["dungeons"][static_cast<size_t>(selected - 1)];
            if (isFinalDungeonLocked(ctx, dungeon))
            {
                cout << "Tahta Kehampaan masih terkunci. Selesaikan dungeon lain dulu.\n";
                waitForEnter();
                continue;
            }

            const int unlockedDepth = getDungeonUnlockedDepth(ctx.player, dungeon);
            const int maxDepth = dungeonMaxDepth(dungeon);

            clearScreen();
            printStateHeader(ctx, dungeon.value("name", std::string()));
            cout << "Pilih depth 1 sampai " << unlockedDepth << ".\n\n";

            for (int depth = 1; depth <= unlockedDepth && depth <= maxDepth; ++depth)
            {
                const json *depthRow = findDungeonDepth(dungeon, depth);
                if (!depthRow)
                    continue;

                const int minLevel = (*depthRow)["level_range"].value("min", 1);
                const int maxLevelRow = (*depthRow)["level_range"].value("max", 999);

                cout << colorText(to_string(depth) + ". Depth " + to_string(depth), Color::Cyan, true)
                     << " | Recommended Lv " << minLevel << "-" << maxLevelRow;
                if (depth == maxDepth)
                    cout << " [Boss]";
                cout << '\n';

                printDungeonLore(*depthRow);
                cout << '\n';
            }

            cout << "Depth pilihan, atau c untuk cancel: ";
            const string depthInput = toLower(readLine());
            if (depthInput == "c")
                continue;

            int depthChoice = 0;
            if (!tryParseInt(depthInput, depthChoice) || depthChoice < 1 || depthChoice > unlockedDepth || depthChoice > maxDepth)
            {
                cout << "Depth tidak valid.\n";
                continue;
            }

            const json *depthRow = findDungeonDepth(dungeon, depthChoice);
            if (!depthRow)
            {
                cout << "Data depth tidak ditemukan.\n";
                waitForEnter();
                continue;
            }

            if (!isDepthLevelAllowed(*depthRow, ctx.player.level))
            {
                cout << "Level character belum sesuai dengan range depth ini.\n";
                waitForEnter();
                continue;
            }

            ctx.player.progress.current_dungeon = dungeon.value("id", std::string());
            ctx.player.progress.current_depth = depthChoice;
            ctx.player.progress.max_depth_unlocked = unlockedDepth;

            if (enterBattleState)
                ctx.stateStack.push_back(GameState::Battle);

            return true;
        }
    }

    void chooseDungeon(GameContext &ctx)
    {
        selectDungeonDepth(ctx, true);
    }
} // namespace state_helpers
