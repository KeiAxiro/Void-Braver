#pragma once
#include "Entities/Player.h"

namespace Core {
    struct GameContext {
        int playerClassId;
        Entities::Player player;

        GameContext() : playerClassId(-1) {}
    };
}