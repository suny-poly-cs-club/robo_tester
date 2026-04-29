#pragma once

#include <cstddef>
#include <list>
#include <string>
#include "include/raylib.h"

struct SharkState {
    std::list<Rectangle> fish;
};

void * shark_create_fn() {
    auto pState = new SharkState();

    for (int i = 0; i < 10; i++) {
        Rectangle body{
            static_cast<float>(GetRandomValue(0, 480)),
            static_cast<float>(GetRandomValue(0, 480)),
            20.0f,
            20.0f,
        };
        pState->fish.push_back(body);
    }

    return pState;
}

void shark_draw_fn(void * _state, int x, int y) {
    SharkState* state = (SharkState*)_state;

    for (auto fish: state->fish) {
        fish.x += x;
        fish.y += y;
        DrawRectangleRec(fish, ORANGE);
    }
}

void shark_mouse_click_fn(void * _state, int button, int mouseX, int mouseY) {
    SharkState* state = (SharkState*)_state;
    Vector2 mousePos{ static_cast<float>(mouseX), static_cast<float>(mouseY) };

    state->fish.remove_if([&](auto& fish){
        return fish.x < mouseX &&
            fish.x + fish.width > mouseX &&
            fish.y < mouseY &&
            fish.y + fish.height > mouseY;
    });
}

bool shark_check_success_fn(void * _state) {
    SharkState* state = (SharkState*)_state;

    return state->fish.empty();
}

std::string shark_get_instructions_fn(void * state) {
    return "Click on all the fish!";
}
