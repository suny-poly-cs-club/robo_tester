#include "team1.h"
#include "include/raylib.h"
#include <cstdlib>

typedef struct {
  Vector2 start_point;
  Vector2 end_point;
  Color color;
} Track;

struct trolley_captcha_state {
  int selected;
  int *people;
  int tracks;
};

const int CAPTCHA_HEIGHT = 500;
const int CAPTCHA_WIDTH = 600;

void *create_trolley_captcha() {
  trolley_captcha_state *state = (trolley_captcha_state *)malloc(sizeof(trolley_captcha_state));

  // tracks is always even
  int tracks = GetRandomValue(4, 9);
  if (tracks % 2) {
    tracks += 1;
  }

  state->tracks = tracks;
  state->selected = 0;

  state->people = (int *)malloc(sizeof(int) * tracks);

  int *peopleSeq = LoadRandomSequence(tracks, 0, tracks - 1);
  for (int i = 0; i < tracks; i++) {
    state->people[i] = peopleSeq[i];
  }

  UnloadRandomSequence(peopleSeq);

  return state;
}

void draw_trolley_captcha(void *state, int x, int y) {
  const auto *captchaState = (trolley_captcha_state *)state;

  ClearBackground(RAYWHITE);

  // TODO: figure out how to put people on the lines

  int tracks = captchaState->tracks;
  float paddingPx = CAPTCHA_WIDTH * 0.125f;

  float TRACK_SPACING = (float)(CAPTCHA_HEIGHT - 2.0f * paddingPx) / tracks;

  int midpoint = tracks / 2;

  Vector2 start = {paddingPx + x, ((float)CAPTCHA_HEIGHT / 2) + y};
  float endX = CAPTCHA_WIDTH - paddingPx + x;

  for (int cnt = 0; cnt < tracks; cnt++) {
    float endY = y + paddingPx + TRACK_SPACING * (cnt + 1 * (cnt >= midpoint ? 1 : 0)) + (cnt >= midpoint ? -2.0f : 2.0f);

    Vector2 end = {endX, endY};

    // float boxWidth = (tracks - 1) * 3.0f + (tracks - 2) * 5;
    float boxWidth = (tracks - 1) * 8;
    // float boxWidth = (tracks - 1) * 7 - 5;
    float boxHight = 25;

    Rectangle hitbox = {
      .x = endX - 15 - boxWidth + 5.5f,
      .y = endY - (boxHight / 2),
      .width = boxWidth,
      .height = boxHight,
    };

    DrawRectangleRec(hitbox, ORANGE);

    DrawLineBezier(
      start,
      end,
      4.0f,
      BLACK
    );

    for (int i = 0; i < captchaState->people[cnt]; i++) {
      Rectangle r = {
        .x = endX - 15 - i * 8.0f,
        .y = endY - 10,
        .width = 3.0f,
        .height = 20.0f,
      };

      DrawRectangleRec(
        r,
        RED
      );
    }
  }
}

void trolley_mouse_click(void *state, int button, int mouseX, int mouseY) {

}

bool trolley_box_checked(void *state) {
  return false;
}

std::string trolley_get_instructions(void *state) {
  return "thing";
}

//this should be at the bottom
std::vector<captchaInfo> team1_get_captchas() {
  return {
    {
      "Trolley Captcha",
      CAPTCHA_WIDTH,
      CAPTCHA_HEIGHT,
      &create_trolley_captcha,
      &draw_trolley_captcha,
      &trolley_mouse_click,
      &trolley_box_checked,
      &trolley_get_instructions,
    }
  };
}
