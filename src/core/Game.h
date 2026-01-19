#pragma once

#include <string>
#include <vector>

#include "game/TileMap.h"
#include "render/Renderer.h"
#include "systems/GhostSystem.h"
#include "entities/Ghost.h"
#include "entities/Player.h"

class Game {
public:
    Game();
    void Run();

private:
    enum class GameState {
        Menu,
        Playing,
        GameOver,
        Win
    };

    bool Initialize();
    void Update(float deltaSeconds);
    void Draw() const;
    void TryMovePlayer(Player& player, Vector2 direction, float deltaSeconds);
    Vector2 TileToWorldCenter(Vector2 tile) const;
    void InitializeGhosts();
    std::vector<Vector2> FindFallbackGhostSpawns(int needed, const std::vector<Vector2>& usedTiles) const;
    void ResetSession();
    void ResetToMenu();
    void HandlePelletPickup(Player& player);
    Rectangle GetStartButtonRect() const;
    bool IsPointInRect(Vector2 point, Rectangle rect) const;

    TileMap map_;

    const int tileSize_ = 24;
    const int uiPanelWidth_ = 260;
    const int uiPadding_ = 12;
    const int rowHeight_ = 24;
    const int tilePixelSize_ = tileSize_;

    Renderer renderer_;
    Player playerA_{};
    Player playerB_{};
    std::vector<Ghost> ghosts_{};
    GhostSystem ghostSystem_{};

    int screenWidth_ = 0;
    int screenHeight_ = 0;
    int mapPixelWidth_ = 0;
    int mapPixelHeight_ = 0;
    GameState state_ = GameState::Menu;
    std::string mapPath_ = "assets/maps/level1.txt";
};
