#pragma once

#include <vector>

#include "entities/Ghost.h"
#include "entities/Player.h"
#include "game/TileMap.h"

class GhostSystem {
public:
    void Update(std::vector<Ghost>& ghosts,
                const TileMap& map,
                Player& playerA,
                Player& playerB,
                float deltaSeconds,
                int tilePixelSize) const;
};
