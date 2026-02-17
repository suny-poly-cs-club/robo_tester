#include "team2.h"

#include "reaction.h"


//this should be at the bottom
std::vector<captchaInfo> team2_get_captchas() {
    return {
        {
            "Reaction Test",
            800,
            450,
            &reaction_create_fn,
            &reaction_draw_fn,
            &reaction_mouse_click_fn,
            &reaction_check_success_fn,
            &reaction_get_instructions_fn,
        }
    };
}