#include "raylib.h"

int main() {
    const int screenWidth = 960;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "Pacmen");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Pacmen: raylib + CMake + vcpkg is working", 40, 40, 20, RAYWHITE);
        DrawText("Next: tilemap + 2 players + grid movement", 40, 70, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}