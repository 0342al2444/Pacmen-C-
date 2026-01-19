#include "core/Game.h"

#include "systems/Input.h"
#include "raylib.h"

#include <algorithm>

Game::Game()
    : renderer_(tilePixelSize_) {
}

void Game::Run() {
    if (!Initialize()) {
        TraceLog(LOG_ERROR, "Failed to initialize game.");
        return;
    }

    while (!WindowShouldClose()) {
        const float deltaSeconds = GetFrameTime();
        Update(deltaSeconds);

        BeginDrawing();
        ClearBackground(Color{ 10, 10, 18, 255 }); // background once per frame
        Draw();
        EndDrawing();
    }

    CloseWindow();
}

bool Game::Initialize() {
    if (!map_.LoadFromFile(mapPath_)) {
        return false;
    }

    mapPixelWidth_ = map_.GetWidth() * tilePixelSize_;
    mapPixelHeight_ = map_.GetHeight() * tilePixelSize_;
    screenWidth_ = mapPixelWidth_ + uiPanelWidth_;
    screenHeight_ = mapPixelHeight_;

    InitWindow(screenWidth_, screenHeight_, "Pacmen");
    SetTargetFPS(60);

    playerA_.radius = tilePixelSize_ * 0.35f;
    playerA_.speed = tilePixelSize_ * 6.0f;
    playerA_.color = YELLOW;

    playerB_.radius = tilePixelSize_ * 0.35f;
    playerB_.speed = tilePixelSize_ * 6.0f;
    playerB_.color = GREEN;

    ResetSession();

    TraceLog(LOG_INFO, "tileSize=%d screen=%dx%d map=%dx%d",
         tilePixelSize_, screenWidth_, screenHeight_, map_.GetWidth(), map_.GetHeight());

    return true;
}

void Game::Update(float deltaSeconds) {
    if (IsKeyPressed(KEY_R)) {
        ResetToMenu();
        return;
    }

    if (state_ == GameState::Menu) {
        const Rectangle startRect = GetStartButtonRect();
        const Vector2 mouse = GetMousePosition();
        const bool hovered = IsPointInRect(mouse, startRect);
        const bool clicked = hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (clicked || Input::IsStartPressed()) {
            ResetSession();
            state_ = GameState::Playing;
        }
        return;
    }

    if (state_ == GameState::GameOver || state_ == GameState::Win) {
        return;
    }

    playerA_.invulnerableSeconds = std::max(0.0f, playerA_.invulnerableSeconds - deltaSeconds);
    playerB_.invulnerableSeconds = std::max(0.0f, playerB_.invulnerableSeconds - deltaSeconds);

    TryMovePlayer(playerA_, Input::GetPlayer1Direction(), deltaSeconds);
    TryMovePlayer(playerB_, Input::GetPlayer2Direction(), deltaSeconds);

    HandlePelletPickup(playerA_);
    HandlePelletPickup(playerB_);

    if (map_.GetRemainingPellets() == 0) {
        state_ = GameState::Win;
        return;
    }

    ghostSystem_.Update(ghosts_, map_, playerA_, playerB_, deltaSeconds, tilePixelSize_);

    if (playerA_.lives <= 0 && playerB_.lives <= 0) {
        state_ = GameState::GameOver;
    }
}

void Game::Draw() const {
    renderer_.DrawMap(map_);
    renderer_.DrawPlayer(playerA_);
    renderer_.DrawPlayer(playerB_);
    for (const Ghost& ghost : ghosts_) {
        renderer_.DrawGhost(ghost);
    }

    const Rectangle startRect = GetStartButtonRect();
    const Vector2 mouse = GetMousePosition();
    const bool hovered = IsPointInRect(mouse, startRect);
    const bool showStart = (state_ == GameState::Menu);
    const bool showGameOver = (state_ == GameState::GameOver);
    const bool showWin = (state_ == GameState::Win);
    renderer_.DrawUI(map_, playerA_, playerB_, uiPanelWidth_, uiPadding_, rowHeight_,
                     showStart, startRect, hovered, showGameOver, showWin);
}


void Game::TryMovePlayer(Player& player, Vector2 direction, float deltaSeconds) {
    if (direction.x == 0.0f && direction.y == 0.0f) {
        return;
    }

    Vector2 next = {
        player.position.x + direction.x * player.speed * deltaSeconds,
        player.position.y + direction.y * player.speed * deltaSeconds
    };

    int tileX = static_cast<int>(next.x / tilePixelSize_);
    int tileY = static_cast<int>(next.y / tilePixelSize_);

    if (!map_.IsWall(tileX, tileY)) {
        player.position = next;
    }

    player.position.x = std::clamp(player.position.x, player.radius, mapPixelWidth_ - player.radius);
    player.position.y = std::clamp(player.position.y, player.radius, mapPixelHeight_ - player.radius);
}

Vector2 Game::TileToWorldCenter(Vector2 tile) const {
    return {
        (tile.x + 0.5f) * tilePixelSize_,
        (tile.y + 0.5f) * tilePixelSize_
    };
}

void Game::InitializeGhosts() {
    ghosts_.clear();
    ghosts_.reserve(6);

    std::vector<Vector2> spawnTiles = map_.GetGhostSpawns();
    if (spawnTiles.size() > 6) {
        spawnTiles.resize(6);
    }

    if (spawnTiles.size() < 6) {
        const int needed = 6 - static_cast<int>(spawnTiles.size());
        std::vector<Vector2> fallback = FindFallbackGhostSpawns(needed, spawnTiles);
        spawnTiles.insert(spawnTiles.end(), fallback.begin(), fallback.end());
    }

    const Color ghostColors[6] = {
        Color{ 230, 70, 60, 255 },
        Color{ 255, 140, 0, 255 },
        Color{ 255, 105, 180, 255 },
        Color{ 80, 160, 255, 255 },
        Color{ 170, 90, 255, 255 },
        Color{ 60, 220, 120, 255 }
    };

    for (size_t i = 0; i < spawnTiles.size() && i < 6; ++i) {
        Ghost ghost{};
        ghost.radius = tilePixelSize_ * 0.33f;
        ghost.speed = playerA_.speed * 0.25f;
        ghost.color = ghostColors[i];
        ghost.position = TileToWorldCenter(spawnTiles[i]);
        ghost.currentDirection = { 0.0f, 0.0f };
        ghosts_.push_back(ghost);
    }
}

std::vector<Vector2> Game::FindFallbackGhostSpawns(int needed, const std::vector<Vector2>& usedTiles) const {
    std::vector<Vector2> results;
    results.reserve(needed);

    const int centerX = map_.GetWidth() / 2;
    const int centerY = map_.GetHeight() / 2;
    const int maxRadius = std::max(map_.GetWidth(), map_.GetHeight());

    auto tileUsed = [&usedTiles](int x, int y) {
        for (const Vector2& tile : usedTiles) {
            if (static_cast<int>(tile.x) == x && static_cast<int>(tile.y) == y) {
                return true;
            }
        }
        return false;
    };

    for (int radius = 0; radius <= maxRadius && static_cast<int>(results.size()) < needed; ++radius) {
        for (int y = centerY - radius; y <= centerY + radius && static_cast<int>(results.size()) < needed; ++y) {
            for (int x = centerX - radius; x <= centerX + radius && static_cast<int>(results.size()) < needed; ++x) {
                if (x < 0 || y < 0 || x >= map_.GetWidth() || y >= map_.GetHeight()) {
                    continue;
                }
                if (map_.IsWall(x, y)) {
                    continue;
                }
                if (tileUsed(x, y)) {
                    continue;
                }
                if (map_.HasPlayerSpawnA() &&
                    static_cast<int>(map_.GetPlayerSpawnA().x) == x &&
                    static_cast<int>(map_.GetPlayerSpawnA().y) == y) {
                    continue;
                }
                if (map_.HasPlayerSpawnB() &&
                    static_cast<int>(map_.GetPlayerSpawnB().x) == x &&
                    static_cast<int>(map_.GetPlayerSpawnB().y) == y) {
                    continue;
                }

                results.push_back(Vector2{ static_cast<float>(x), static_cast<float>(y) });
            }
        }
    }

    return results;
}

void Game::ResetSession() {
    map_.ResetTiles();

    if (map_.HasPlayerSpawnA()) {
        playerA_.position = TileToWorldCenter(map_.GetPlayerSpawnA());
    } else {
        playerA_.position = { tilePixelSize_ * 1.5f, tilePixelSize_ * 1.5f };
    }
    playerA_.spawnPosition = playerA_.position;
    playerA_.lives = 3;
    playerA_.score = 0;
    playerA_.invulnerableSeconds = 0.0f;

    if (map_.HasPlayerSpawnB()) {
        playerB_.position = TileToWorldCenter(map_.GetPlayerSpawnB());
    } else {
        playerB_.position = { mapPixelWidth_ - tilePixelSize_ * 1.5f, mapPixelHeight_ - tilePixelSize_ * 1.5f };
    }
    playerB_.spawnPosition = playerB_.position;
    playerB_.lives = 3;
    playerB_.score = 0;
    playerB_.invulnerableSeconds = 0.0f;

    InitializeGhosts();
}

void Game::ResetToMenu() {
    ResetSession();
    state_ = GameState::Menu;
}

void Game::HandlePelletPickup(Player& player) {
    const int tileX = static_cast<int>(player.position.x / tilePixelSize_);
    const int tileY = static_cast<int>(player.position.y / tilePixelSize_);
    if (map_.ConsumePelletAt(tileX, tileY)) {
        player.score += 10;
    }
}

Rectangle Game::GetStartButtonRect() const {
    const float panelLeft = static_cast<float>(mapPixelWidth_);
    const float padding = static_cast<float>(uiPadding_);
    const float buttonWidth = static_cast<float>(uiPanelWidth_) - padding * 2.0f;
    const float buttonHeight = static_cast<float>(rowHeight_ * 2);
    const float scoreboardHeight = static_cast<float>(rowHeight_ * 6);
    const float instructionHeight = static_cast<float>(rowHeight_ * 2);
    const float buttonX = panelLeft + padding;
    const float buttonY = padding + scoreboardHeight + instructionHeight + padding;
    return Rectangle{ buttonX, buttonY, buttonWidth, buttonHeight };
}

bool Game::IsPointInRect(Vector2 point, Rectangle rect) const {
    return point.x >= rect.x && point.x <= rect.x + rect.width
        && point.y >= rect.y && point.y <= rect.y + rect.height;
}
