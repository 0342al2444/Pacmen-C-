#include "systems/Input.h"

#include <cmath>

namespace Input {
    static Vector2 Normalize(Vector2 value) {
        float length = std::sqrt(value.x * value.x + value.y * value.y);
        if (length <= 0.0f) {
            return { 0.0f, 0.0f };
        }

        return { value.x / length, value.y / length };
    }

    Vector2 GetPlayer1Direction() {
        Vector2 dir = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_W)) {
            dir.y -= 1.0f;
        }
        if (IsKeyDown(KEY_S)) {
            dir.y += 1.0f;
        }
        if (IsKeyDown(KEY_A)) {
            dir.x -= 1.0f;
        }
        if (IsKeyDown(KEY_D)) {
            dir.x += 1.0f;
        }
        return Normalize(dir);
    }

    Vector2 GetPlayer2Direction() {
        Vector2 dir = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_UP)) {
            dir.y -= 1.0f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            dir.y += 1.0f;
        }
        if (IsKeyDown(KEY_LEFT)) {
            dir.x -= 1.0f;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            dir.x += 1.0f;
        }
        return Normalize(dir);
    }

    bool IsStartPressed() {
        return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
    }
}
