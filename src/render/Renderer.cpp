#include "render/Renderer.h"

#include "raylib.h"

Renderer::Renderer(int tilePixelSize)
    : tilePixelSize_(tilePixelSize) {
}

void Renderer::DrawMap(const TileMap& map) const {

    // Use strong contrast so walls are clearly visible against the background.
    // In the previous version, wallFill was a bright blue that could blend into
    // a similar blue background, making walls look "invisible".
    Color wallFill    { 0, 40, 140, 255 };        // darker blue
    Color wallOutline { 255, 255, 255, 255 };     // white outline
    Color pelletColor { 255, 210, 120, 255 };

    for (int y = 0; y < map.GetHeight(); ++y) {
        for (int x = 0; x < map.GetWidth(); ++x) {
            const char tile = map.GetTile(x, y);
            const int worldX = x * tilePixelSize_;
            const int worldY = y * tilePixelSize_;

            if (tile == '#') {
                Rectangle r{ (float)worldX, (float)worldY, (float)tilePixelSize_, (float)tilePixelSize_ };
                DrawRectangleRec(r, wallFill);
                DrawRectangleLinesEx(r, 2.0f, wallOutline);
            } else if (tile == '.') {
                DrawCircle(worldX + tilePixelSize_ / 2,
                           worldY + tilePixelSize_ / 2,
                           tilePixelSize_ * 0.15f,
                           pelletColor);
            }
        }
    }
}

void Renderer::DrawPlayer(const Player& player) const {
    DrawCircleV(player.position, player.radius, player.color);
}

void Renderer::DrawGhost(const Ghost& ghost) const {
    DrawCircleV(ghost.position, ghost.radius, ghost.color);
}

namespace {
    void DrawButton(const Rectangle& rect, const char* label, bool hovered) {
        const Color fill = hovered ? Color{ 90, 120, 220, 255 } : Color{ 70, 95, 190, 255 };
        const Color border = hovered ? RAYWHITE : Color{ 220, 220, 220, 255 };
        DrawRectangleRec(rect, fill);
        DrawRectangleLinesEx(rect, 2.0f, border);

        const int fontSize = 28;
        const int textWidth = MeasureText(label, fontSize);
        const int textX = static_cast<int>(rect.x + (rect.width - textWidth) * 0.5f);
        const int textY = static_cast<int>(rect.y + (rect.height - fontSize) * 0.5f);
        DrawText(label, textX, textY, fontSize, RAYWHITE);
    }
}

void Renderer::DrawUI(const TileMap& map,
                      const Player& playerA,
                      const Player& playerB,
                      int uiPanelWidth,
                      int uiPadding,
                      int rowHeight,
                      bool showStartButton,
                      const Rectangle& startButton,
                      bool startHovered,
                      bool showGameOver,
                      bool showWin) const {
    const int mapPixelWidth = map.GetWidth() * tilePixelSize_;
    Rectangle panel{ static_cast<float>(mapPixelWidth), 0.0f,
                     static_cast<float>(uiPanelWidth),
                     static_cast<float>(map.GetHeight() * tilePixelSize_) };

    DrawRectangleRec(panel, Color{ 20, 22, 32, 255 });
    DrawRectangleLinesEx(panel, 2.0f, Color{ 50, 55, 75, 255 });

    int textX = mapPixelWidth + uiPadding;
    int textY = uiPadding;
    const int lineHeight = rowHeight;

    DrawText("Scoreboard", textX, textY, 22, RAYWHITE);
    textY += lineHeight + 4;

    DrawText(TextFormat("P1 Score: %d", playerA.score), textX, textY, 20, RAYWHITE);
    textY += lineHeight;
    DrawText(TextFormat("P2 Score: %d", playerB.score), textX, textY, 20, RAYWHITE);
    textY += lineHeight + 4;
    DrawText(TextFormat("P1 Lives: %d", playerA.lives), textX, textY, 20, RAYWHITE);
    textY += lineHeight;
    DrawText(TextFormat("P2 Lives: %d", playerB.lives), textX, textY, 20, RAYWHITE);
    textY += lineHeight + uiPadding;

    DrawText("P1 WASD | P2 Arrows", textX, textY, 18, Color{ 180, 190, 210, 255 });
    textY += lineHeight;
    DrawText("Enter/Space: Start", textX, textY, 18, Color{ 180, 190, 210, 255 });
    textY += lineHeight;

    if (showWin) {
        DrawText("YOU WIN", textX, textY, 26, Color{ 120, 220, 150, 255 });
        textY += lineHeight + 4;
        DrawText("Press R for Menu", textX, textY, 18, Color{ 200, 200, 200, 255 });
        textY += lineHeight;
        DrawText("Press Esc to Quit", textX, textY, 18, Color{ 200, 200, 200, 255 });
        textY += lineHeight;
    }

    if (showGameOver) {
        DrawText("GAME OVER", textX, textY, 24, Color{ 255, 120, 90, 255 });
        textY += lineHeight;
        DrawText("Press R to return to Menu", textX, textY, 18, Color{ 200, 200, 200, 255 });
    }

    if (showStartButton) {
        DrawButton(startButton, "START", startHovered);
    }
}
