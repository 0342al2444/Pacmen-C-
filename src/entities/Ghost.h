#pragma once

#include "raylib.h"

struct Ghost {
    Vector2 position{};
    float radius = 8.0f;
    float speed = 90.0f;
    Color color = RED;
    Vector2 currentDirection{ 0.0f, 0.0f };
};
