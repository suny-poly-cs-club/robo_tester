#include "team2.h"


//shark

//stock market
struct stockMarketState {
    int funds;
};

void * stockMarketCreate() {

    return malloc(sizeof(stockMarketState));
}

void stockMarketDraw(void * state, int x, int y) {
    auto * s = static_cast<stockMarketState*>(state);
    DrawRectangle(x,y,500,500,BLACK);
    std::string fundsText = "Funds: $";
    fundsText += std::to_string(s->funds);
    if (s->funds < 0) {
        DrawText(fundsText.c_str(),x+10,y+20,30,RED);
    } else {
        DrawText(fundsText.c_str(),x+10,y+20,30,GREEN);
    }
}

void stockMarketClicked(void * state, int button, int mouseX, int mouseY) {

}

bool stockMarkeySuccess(void * state) {
    return true;
}

std::string stockMarketGetInstructions(void * state) {
    return "Make $500 in the socket market";
}

//harm the huma



//teacton time



//this should be at the bottom
std::vector<captchaInfo> team2_get_captchas() {
    return {
        //shark

        //stock market
        {
            "stock_market",
            500, 500,
            &stockMarketCreate, // create function
            &stockMarketDraw, // draw function
            &stockMarketClicked, //mouse clicked function
            &stockMarkeySuccess, // success function
            &stockMarketGetInstructions,//instructions function
        }

        //harm the huma



        //teacton time


    };
}