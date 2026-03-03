#include "team1.h"
#include "include/raylib.h"
#include <cstdlib>

typedef struct {
    Vector2 start_point;
    Vector2 end_point;
    Color color;
} Tracks;

struct trolley_captcha_state {
  int selected;
  int *people;
  int tracks;
  Tracks *trackLines;
};

const int CAPTCHA_HEIGHT = 500;
const int CAPTCHA_WIDTH = 600;

void *create_trolley_captcha() {
  trolley_captcha_state *state = (trolley_captcha_state *)malloc(sizeof(trolley_captcha_state));

  int tracks = GetRandomValue(4, 9);
  if (tracks % 2) {
    tracks += 1;
  }

  state->tracks = tracks;
  state->selected = 0;

  state->people = (int *)malloc(sizeof(int) * tracks);
  state->trackLines = (Tracks*)malloc(sizeof(Tracks) * tracks);
  for (int i = 0; i < tracks; i++) {
    state->people[i] = GetRandomValue(0, tracks * 2);
  }

  return state;
}

void draw_trolley_captcha(void *state, int x, int y) {
  const auto *captchaState = (trolley_captcha_state *)state;

  ClearBackground(RAYWHITE);

  // TODO: figure out how to put people on the lines
  // TODO: make the lines look better. make them relative to h/2+y

  for (int cnt = 0; cnt < captchaState->tracks; cnt++) {
    captchaState->trackLines[cnt].start_point = { (float)x, ((float)CAPTCHA_HEIGHT / 2)+y };
    captchaState->trackLines[cnt].end_point = { (float)x+450, static_cast<float>(50 * cnt + y)};
    captchaState->trackLines[cnt].color = BLACK;

    DrawLineBezier(
      captchaState->trackLines[cnt].end_point,
      captchaState->trackLines[cnt].start_point,
      4.0f,
      captchaState->trackLines[cnt].color
    );
  }
}

void trolley_mouse_click(void *state, int button, int mouseX, int mouseY) {}

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
      CAPTCHA_HEIGHT,
      CAPTCHA_HEIGHT,
      &create_trolley_captcha,
      &draw_trolley_captcha,
      &trolley_mouse_click,
      &trolley_box_checked,
      &trolley_get_instructions,
    }
  };
}
