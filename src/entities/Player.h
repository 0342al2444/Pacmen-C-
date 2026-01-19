#pragma once

#include "raylib.h"

struct Player {
    Vector2 position{};
    Vector2 spawnPosition{};
    float radius = 8.0f;
    float speed = 120.0f;
    Color color = YELLOW;
    int lives = 3;
    int score = 0;
    float invulnerableSeconds = 0.0f;
};
