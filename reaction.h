#pragma once

#include <cstddef>
#include <list>
#include <string>
#include "include/raylib.h"

Image foxyImg{};
Texture foxyGif{};
int nFrames = 0;

struct ReactionState {
    int curFrame;
    int nFramesLeft;

    ReactionState() : curFrame{0}, nFramesLeft{GetRandomValue(120, 360)} {

    }
};

void * reaction_create_fn() {
    // Load GIF
    if (nFrames == 0) {
        foxyImg = LoadImageAnim("assets/foxy.gif", &nFrames);
        foxyGif = LoadTextureFromImage(foxyImg);
    }

    auto state = new ReactionState();
    return state;
}

void reaction_draw_fn(void * _state, int x, int y) {
    ReactionState* state = (ReactionState*)_state;

    // Draw green
    if (state->nFramesLeft >= 0) {
        DrawRectangle(x, y, 800, 450, GREEN);
    } else {
        // Prepare next frame.
        state->curFrame = (state->curFrame + 1) % nFrames;
        UpdateTexture(foxyGif, (unsigned char*)foxyImg.data + (foxyImg.width * foxyImg.height * 4 * state->curFrame));

        const Rectangle src{
            0.0f,
            0.0f,
            static_cast<float>(foxyGif.width),
            static_cast<float>(foxyGif.height),
        };
        const Rectangle dst{
            static_cast<float>(x),
            static_cast<float>(y),
            800.0f,
            450.0f,
        };
        DrawTexturePro(foxyGif, src, dst, Vector2{}, 0.0f, WHITE);
    }

    state->nFramesLeft -= 1;
}

void reaction_mouse_click_fn(void * _state, int button, int mouseX, int mouseY) {
    
}

bool reaction_check_success_fn(void * _state) {
    ReactionState* state = (ReactionState*)_state;
    return state->nFramesLeft < 0;
}

std::string reaction_get_instructions_fn(void * state) {
    return "Click when the green is gone!";
}
