#include "systems/GhostSystem.h"

#include <algorithm>
#include <cfloat>

namespace {
    float DistanceSquared(Vector2 a, Vector2 b) {
        const float dx = a.x - b.x;
        const float dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    Vector2 ChooseTarget(const Player& playerA, const Player& playerB, const Ghost& ghost) {
        const float distA = DistanceSquared(ghost.position, playerA.position);
        const float distB = DistanceSquared(ghost.position, playerB.position);
        return (distA <= distB) ? playerA.position : playerB.position;
    }

    void TryCapturePlayer(const Ghost& ghost, Player& player) {
        if (player.invulnerableSeconds > 0.0f) {
            return;
        }

        const float captureDistance = ghost.radius + player.radius;
        if (DistanceSquared(ghost.position, player.position) <= captureDistance * captureDistance) {
            player.position = player.spawnPosition;
            player.invulnerableSeconds = 1.0f;
            if (player.lives > 0) {
                --player.lives;
            }
        }
    }
}

void GhostSystem::Update(std::vector<Ghost>& ghosts,
                         const TileMap& map,
                         Player& playerA,
                         Player& playerB,
                         float deltaSeconds,
                         int tilePixelSize) const {
    const float mapWidth = map.GetWidth() * tilePixelSize;
    const float mapHeight = map.GetHeight() * tilePixelSize;

    const Vector2 directions[4] = {
        { -1.0f, 0.0f },
        { 0.0f, -1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f }
    };

    for (Ghost& ghost : ghosts) {
        const Vector2 target = ChooseTarget(playerA, playerB, ghost);

        float bestDistance = FLT_MAX;
        bool foundDirection = false;
        Vector2 bestDirection{ 0.0f, 0.0f };
        Vector2 bestNext = ghost.position;

        for (const Vector2& dir : directions) {
            Vector2 next = {
                ghost.position.x + dir.x * ghost.speed * deltaSeconds,
                ghost.position.y + dir.y * ghost.speed * deltaSeconds
            };

            const int tileX = static_cast<int>(next.x / tilePixelSize);
            const int tileY = static_cast<int>(next.y / tilePixelSize);
            if (map.IsWall(tileX, tileY)) {
                continue;
            }

            const float distance = DistanceSquared(next, target);
            if (distance < bestDistance) {
                bestDistance = distance;
                bestDirection = dir;
                bestNext = next;
                foundDirection = true;
            }
        }

        if (foundDirection) {
            ghost.currentDirection = bestDirection;
            ghost.position = bestNext;
        }

        ghost.position.x = std::clamp(ghost.position.x, ghost.radius, mapWidth - ghost.radius);
        ghost.position.y = std::clamp(ghost.position.y, ghost.radius, mapHeight - ghost.radius);

        TryCapturePlayer(ghost, playerA);
        TryCapturePlayer(ghost, playerB);
    }
}
