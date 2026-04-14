#include "team1.h"
#include "include/raylib.h"
#include <cstdlib>

// TODO: delete people when they get hit
// TODO: figure out which line mouse clicks

typedef struct {
  Vector2 start_point;
  Vector2 end_point;
  Color color;
} Track;

struct trolley_captcha_state {
  int selected;
  int *people;
  int tracks;
  int animationframe;
};

const int CAPTCHA_HEIGHT = 500;
const int CAPTCHA_WIDTH = 600;

float EaseCubicInOut(float t, float b, float c, float d) {
  if ((t /= 0.5f*d) < 1) {
    return 0.5f*c*t*t*t + b;
  }

  t -= 2;
  return 0.5f*c*(t*t*t + 2.0f) + b;
}

void *create_trolley_captcha() {
  trolley_captcha_state *state = (trolley_captcha_state *)malloc(sizeof(trolley_captcha_state));

  // tracks is always even
  int tracks = GetRandomValue(4, 9);
  if (tracks % 2) {
    tracks += 1;
  }

  state->tracks = tracks;
  state->selected = 2;

  state->people = (int *)malloc(sizeof(int) * tracks);
  state->animationframe = 0;

  int *peopleSeq = LoadRandomSequence(tracks, 0, tracks - 1);
  for (int i = 0; i < tracks; i++) {
    state->people[i] = peopleSeq[i];
  }

  UnloadRandomSequence(peopleSeq);

  return state;
}

void draw_trolley_captcha(void *state, int x, int y) {
  auto *captchaState = (trolley_captcha_state *)state;

  ClearBackground(RAYWHITE);

  int tracks = captchaState->tracks;
  float paddingPx = CAPTCHA_WIDTH * 0.125f;

  float TRACK_SPACING = (float)(CAPTCHA_HEIGHT - 2.0f * paddingPx) / tracks;

  int midpoint = tracks / 2;

  Vector2 start = {paddingPx + x, ((float)CAPTCHA_HEIGHT / 2) + y};
  float endX = CAPTCHA_WIDTH - paddingPx + x;

  Vector2 endanimation;

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

    // DrawRectangleRec(hitbox, ORANGE);

    if (cnt == captchaState->selected) {
      endanimation.x = end.x;
      endanimation.y = end.y;
    }

    DrawLineBezier(
      start,
      end,
      4.0f,
      cnt == captchaState->selected ? BLUE : BLACK
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

  if (captchaState->animationframe <= 120) {
    float animationprogress = ((float)captchaState->animationframe/120);

    float x1 = start.x;
    float y1 = start.y;
    float x2 = endanimation.x;
    float y2 = endanimation.y;

    float animationx = (endanimation.x - start.x) * animationprogress + start.x;
    float animationy = EaseCubicInOut(animationprogress, y1, y2-y1, 1);

    DrawRectangle(animationx-8, animationy-8, 16, 16, RED);

    printf("%f (%f %f) y1=%f y2=%f\n", animationprogress, animationx, animationy, y1, y2);

    captchaState->animationframe++;
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
