#include "game/TileMap.h"

#include <fstream>
#include <string>
#include <vector>

bool TileMap::LoadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<std::string> lines;
    std::string line;
    int maxWidth = 0;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if ((int)line.size() > maxWidth) maxWidth = (int)line.size();
        lines.push_back(line);
    }

    if (lines.empty() || maxWidth == 0) return false;

    width_ = maxWidth;
    height_ = (int)lines.size();
    tiles_.assign(height_, std::string(width_, ' '));

    hasSpawnA_ = false;
    hasSpawnB_ = false;
    ghostSpawns_.clear();

    for (int y = 0; y < height_; ++y) {
        const std::string& src = lines[y];
        for (int x = 0; x < (int)src.size(); ++x) {
            char c = src[x];

            if (c == 'P') {
                spawnA_ = Vector2{ (float)x, (float)y };
                hasSpawnA_ = true;
                c = ' ';
            } else if (c == 'Q') {
                spawnB_ = Vector2{ (float)x, (float)y };
                hasSpawnB_ = true;
                c = ' ';
            } else if (c == 'G') {
                ghostSpawns_.push_back(Vector2{ (float)x, (float)y });
                c = ' ';
            }

            tiles_[y][x] = c;
        }
    }

    originalTiles_ = tiles_;

    return true;
}

char TileMap::GetTile(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) return '#';
    return tiles_[y][x];
}

bool TileMap::IsWall(int x, int y) const {
    return GetTile(x, y) == '#';
}

bool TileMap::ConsumePelletAt(int x, int y) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return false;
    }

    if (tiles_[y][x] == '.') {
        tiles_[y][x] = ' ';
        return true;
    }

    return false;
}

void TileMap::ResetTiles() {
    tiles_ = originalTiles_;
}
