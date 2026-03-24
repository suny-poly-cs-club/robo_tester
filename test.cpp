#include "include/raylib.h"
#include <cstdio>
#include <cstdlib>

typedef struct {
    Vector2 start_point;
    Vector2 end_point;
    Color color;
} Track;

// static float EaseCubicInOut(float t, float b, float c, float d);

// https://www.desmos.com/calculator/2ityioy3yc
float EaseCubicInOut(float t, float b, float c, float d)
{
    float result = 0.0f;

    if ((t /= 0.5f*d) < 1) result = 0.5f*c*t*t*t + b;
    else
    {
        t -= 2;
        result = 0.5f*c*(t*t*t + 2.0f) + b;
    }

    return result;
}

int main() {
  float x1 = 0;
  float y1 = 0;
  float x2 = 1;
  float y2 = 1;

  for (float i = 0.0f, j=0; i < 1.0; i += 0.05, j++) {
    float thing = EaseCubicInOut(i - x1, y1, y2-y1, x2-x1);
    printf("%.0f, (%f, %f)\n", j, i, thing);
  }

    // const int screenWidth = 1000;
    // const int screenHeight = 450;
    //
    // SetConfigFlags(FLAG_MSAA_4X_HINT);
    // InitWindow(screenWidth, screenHeight, "Trolley Raylib Practice");
    //
    // int num_tracks = GetRandomValue(4, 9);
    // if (num_tracks % 2 != 0)
    //     num_tracks += 1;
    //
    // Track* tracks = (Track*)malloc(sizeof(Track) * num_tracks);
    //
    // for (int cnt = 0; cnt < num_tracks; cnt++) {
    //     tracks[cnt].start_point = { 500, 200 }; //{coord, coord} since they all have the same start point
    //     tracks[cnt].end_point = { 700, static_cast<float>(50 * cnt)};  //{coord, coord based on cnt (should all align in the end)}
    //     tracks[cnt].color = BLACK;
    // }
    //
    // SetTargetFPS(60);
    //
    // while (!WindowShouldClose()) {
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);
    //     for (int cnt = 0; cnt < num_tracks; cnt++) {
    //         DrawLineBezier(tracks[cnt].start_point, tracks[cnt].end_point, 4.0f, tracks[cnt].color);
    //     }
    //     EndDrawing();
    // }
    // free(tracks);
    // CloseWindow();
}
