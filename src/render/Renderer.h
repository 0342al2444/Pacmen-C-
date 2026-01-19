#pragma once

#include "game/TileMap.h"
#include "entities/Ghost.h"
#include "entities/Player.h"

class Renderer {
public:
    explicit Renderer(int tilePixelSize);

    void DrawMap(const TileMap& map) const;
    void DrawPlayer(const Player& player) const;
    void DrawGhost(const Ghost& ghost) const;
    void DrawUI(const TileMap& map,
                const Player& playerA,
                const Player& playerB,
                int uiPanelWidth,
                int uiPadding,
                int rowHeight,
                bool showStartButton,
                const Rectangle& startButton,
                bool startHovered,
                bool showGameOver) const;
    int GetTilePixelSize() const { return tilePixelSize_; }

private:
    int tilePixelSize_ = 24;
};
