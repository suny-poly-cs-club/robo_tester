#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_WIDTH 175
#define WINDOW_HEIGHT 175

#include <cstddef>
#include <list>
#include <random>
#include <string>
#include <iostream>
#include "include/raylib.h"

struct WaldoState {
    RenderTexture2D texture;
    int waldo_x;
    int waldo_y;
    int waldo_r;
    int win_x;
    int win_y;
    bool won;
};

void * waldo_create_fn() {
    WaldoState* state = (WaldoState*)malloc(sizeof(WaldoState));

    // Create the texture.
    state->texture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    BeginTextureMode(state->texture);

    ClearBackground(BLACK);

    // Draw stuff.
    for (int i = 0; i < 1'000; i++) {
        const Color c = {
            static_cast<unsigned char>(GetRandomValue(0, 255)),
            static_cast<unsigned char>(GetRandomValue(0, 255)),
            static_cast<unsigned char>(GetRandomValue(0, 255)),
            static_cast<unsigned char>(GetRandomValue(200, 225))
        };
        if (GetRandomValue(0, 1) == 0) {
            const auto r = GetRandomValue(0, 100);
            DrawCircle(
                GetRandomValue(-r, SCREEN_WIDTH + r),
                GetRandomValue(-r, SCREEN_HEIGHT + r),
                r,
                c
            );
        } else {
            const auto w = GetRandomValue(0, 200);
            const auto h = GetRandomValue(0, 200);
            DrawRectangle(
                GetRandomValue(0, SCREEN_WIDTH - w),
                GetRandomValue(0, SCREEN_HEIGHT - h),
                w,
                h,
                c
            );
        }
    }

    // Draw waldo.
    state->waldo_x = GetRandomValue(0, SCREEN_WIDTH);
    state->waldo_y = GetRandomValue(0, SCREEN_HEIGHT);
    state->waldo_r = GetRandomValue(10, 20);
    DrawCircle(state->waldo_x, state->waldo_y, state->waldo_r, RED);
    DrawRectangleLines(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, YELLOW);

    EndTextureMode();

    state->win_x = 0;
    state->win_y = 0;

    return state;
}

void waldo_draw_fn(void * _state, int x, int y) {
    WaldoState* state = (WaldoState*)_state;
    state->win_x = x;
    state->win_y = y;
    DrawTexturePro(
        state->texture.texture,
        {(float)x, (float)(- y - WINDOW_HEIGHT), WINDOW_WIDTH, -WINDOW_HEIGHT},
        {(float)x, (float)y, WINDOW_WIDTH, WINDOW_HEIGHT},
        {0, 0},
        0.0f,
        WHITE
    );
}

void waldo_mouse_click_fn(void * _state, int button, int mouseX, int mouseY) {
    WaldoState* state = (WaldoState*)_state;
    auto dx = (mouseX + state->win_x) - state->waldo_x;
    auto dy = (mouseY + state->win_y) - state->waldo_y;
    auto r = state->waldo_r;
    state->won = state->won || ((dx * dx) + (dy * dy) <= r * r);
}

bool waldo_check_success_fn(void * _state) {
    const WaldoState* state = (WaldoState*)_state;
    return state->won;
}

std::string waldo_get_instructions_fn(void * state) {
    return "Shoot Waldo!";
}
