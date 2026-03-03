#include "include/raylib.h"
#include <cstdlib>

typedef struct {
    Vector2 start_point;
    Vector2 end_point;
    Color color;
} Tracks;

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Trolley Raylib Practice");

    int num_tracks = GetRandomValue(4, 9);
    if (num_tracks % 2 != 0)
        num_tracks += 1;

    Tracks* tracks = (Tracks*)malloc(sizeof(Tracks) * num_tracks);

    for (int cnt = 0; cnt < num_tracks; cnt++) {
        tracks[cnt].start_point = { 500, 200 }; //{coord, coord} since they all have the same start point
        tracks[cnt].end_point = { 700, static_cast<float>(50 * cnt)};  //{coord, coord based on cnt (should all align in the end)}
        tracks[cnt].color = BLACK;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int cnt = 0; cnt < num_tracks; cnt++) {
            DrawLineBezier(tracks[cnt].start_point, tracks[cnt].end_point, 4.0f, tracks[cnt].color);
        }
        EndDrawing();
    }
    free(tracks);
    CloseWindow();
}
