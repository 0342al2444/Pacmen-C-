#pragma once

#include <string>
#include <vector>
#include "raylib.h"

class TileMap {
public:
    bool LoadFromFile(const std::string& path);

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

    char GetTile(int x, int y) const;
    bool IsWall(int x, int y) const;
    bool ConsumePelletAt(int x, int y);
    void ResetTiles();

    bool HasPlayerSpawnA() const { return hasSpawnA_; }
    bool HasPlayerSpawnB() const { return hasSpawnB_; }
    Vector2 GetPlayerSpawnA() const { return spawnA_; }
    Vector2 GetPlayerSpawnB() const { return spawnB_; }
    const std::vector<Vector2>& GetGhostSpawns() const { return ghostSpawns_; }

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<std::string> tiles_;
    std::vector<std::string> originalTiles_;

    Vector2 spawnA_{ 0.0f, 0.0f };
    Vector2 spawnB_{ 0.0f, 0.0f };
    bool hasSpawnA_ = false;
    bool hasSpawnB_ = false;
    std::vector<Vector2> ghostSpawns_{};
};
