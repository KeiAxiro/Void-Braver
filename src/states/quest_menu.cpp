#include "states.h"
#include "database.h"
#include "states/input_helpers.h"
#include "states/player_stats_helpers.h"
#include "states/ui_helpers.h"
#include "states/math_helpers.h"
#include "utils/console_ui.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace consoleui;
using namespace state_helpers;

namespace {
    void generateQuestsIfNeeded(GameContext &ctx) {
        if (!ctx.player.quests.board_quests.empty()) return;

        if (!ctx.gameData.contains("enemies") || !ctx.gameData["enemies"].is_array()) return;

        vector<const json*> pool;
        for (const auto &enemy : ctx.gameData["enemies"]) {
            if (enemy.value("is_boss", false) == false) {
                pool.push_back(&enemy);
            }
        }

        if (pool.empty()) return;

        for (int i = 0; i < 5; ++i) {
            int idx = randInt(0, pool.size() - 1);
            const json* enemy = pool[idx];

            Quest q;
            q.id = "quest_" + to_string(randInt(1000, 9999));
            q.target_enemy_id = enemy->value("id", string(""));
            q.target_enemy_name = enemy->value("name", string("Enemy"));
            q.target_amount = randInt(3, 8);
            q.current_amount = 0;
            
            // Hadiah disesuaikan dengan level musuh
            int minLvl = (*enemy)["level_range"].value("min", 1);
            q.reward_exp = q.target_amount * minLvl * 15;
            q.reward_gold = q.target_amount * minLvl * 10;
            
            ctx.player.quests.board_quests.push_back(q);
        }
    }
}

void questMenu(GameContext &ctx) {
    while (true) {
        clearScreen();
        printStateHeader(ctx, "PAPAN MISI (QUEST BOARD)");

        generateQuestsIfNeeded(ctx);

        if (ctx.player.quests.has_active_quest) {
            Quest &aq = ctx.player.quests.active_quest;
            cout << colorText("Misi Aktif:", Color::Yellow, true) << "\n";
            cout << "Target : Kalahkan " << aq.target_amount << " " << aq.target_enemy_name << "\n";
            cout << "Progres: " << aq.current_amount << " / " << aq.target_amount << "\n";
            cout << "Hadiah : " << aq.reward_exp << " EXP, " << aq.reward_gold << " Gold\n\n";

            printLine('-');
            cout << colorText("1. Batalkan Misi", Color::Red) << "\n";
            if (aq.current_amount >= aq.target_amount) {
                cout << colorText("2. Klaim Hadiah", Color::Green) << "\n";
            }
            cout << "c. Kembali ke Hub\n";
            cout << "Pilih: ";

            string choice = toLower(readLine());
            if (choice == "c") return;
            else if (choice == "1") {
                ctx.player.quests.board_quests.push_back(aq);
                ctx.player.quests.has_active_quest = false;
                saveGame(ctx);
                cout << "Misi dibatalkan.\n";
                waitForEnter();
            }
            else if (choice == "2" && aq.current_amount >= aq.target_amount) {
                ctx.player.current_exp += aq.reward_exp;
                ctx.player.gold += aq.reward_gold;
                cout << colorText("Hadiah diklaim!", Color::Green, true) << "\n";
                cout << "Mendapat " << aq.reward_exp << " EXP dan " << aq.reward_gold << " Gold.\n";
                
                levelUpIfNeeded(ctx);
                
                // Misi selesai, reset papan misi dengan 5 misi baru
                ctx.player.quests.has_active_quest = false;
                ctx.player.quests.board_quests.clear();
                
                saveGame(ctx);
                waitForEnter();
            } else {
                cout << "Pilihan tidak valid atau misi belum selesai.\n";
                waitForEnter();
            }
        }
        else {
            if (ctx.player.quests.board_quests.empty()) {
                cout << "Belum ada misi tersedia saat ini.\n";
                waitForEnter();
                return;
            }

            cout << "Pilih salah satu misi untuk dikerjakan (Hanya bisa 1 misi aktif):\n\n";
            int idx = 1;
            for (const auto &q : ctx.player.quests.board_quests) {
                cout << colorText(to_string(idx) + ". Kalahkan " + to_string(q.target_amount) + " " + q.target_enemy_name, Color::Cyan) << "\n";
                cout << "   Hadiah: " << q.reward_exp << " EXP, " << q.reward_gold << " Gold\n";
                idx++;
            }
            cout << "\nc. Kembali ke Hub\n";
            cout << "Pilih misi (1-5): ";

            string choice = toLower(readLine());
            if (choice == "c") return;

            int selected = 0;
            if (tryParseInt(choice, selected) && selected >= 1 && selected <= ctx.player.quests.board_quests.size()) {
                ctx.player.quests.active_quest = ctx.player.quests.board_quests[selected - 1];
                ctx.player.quests.has_active_quest = true;
                ctx.player.quests.board_quests.erase(ctx.player.quests.board_quests.begin() + (selected - 1));
                saveGame(ctx);
                cout << "Misi diambil! Semoga berhasil.\n";
                waitForEnter();
            } else {
                cout << "Pilihan tidak valid.\n";
                waitForEnter();
            }
        }
    }
}
