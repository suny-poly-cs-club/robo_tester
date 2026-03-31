#include "team2.h"
#include <iostream> //cout
#include "include/raylib.h"
#include <vector>
#include "shark.h"
#include "reaction.h"

using namespace std;

//prototypes

//consts

//shark

//stock market
constexpr int STOCK_HISTORY_LENGTH = 125;
struct stockMarketState {
    int funds{};
    int time_step{};
    int stock1[STOCK_HISTORY_LENGTH]{};
    int stock1Trend{};
    int stock2[STOCK_HISTORY_LENGTH]{};
    int stock2Trend{};
    int stock3[STOCK_HISTORY_LENGTH]{};
    int stock3Trend{};
    int doubleCounter{};
    int ownedStock1{};
    int ownedStock2{};
    int ownedStock3{};
    int netProfit{};
};

union ezAutoCasting {
    void * in;
    stockMarketState * stock;
};

void * stockMarketCreate() {
    void * state = malloc(sizeof(stockMarketState));
    ezAutoCasting ez{};
    ez.in = state;
    for (int i=0;i<STOCK_HISTORY_LENGTH;i++) {
        ez.stock->stock1[i] = 0;
        ez.stock->stock2[i] = 0;
        ez.stock->stock3[i] = 0;
    }
    ez.stock->time_step = 0;
    ez.stock->stock1Trend = 0;
    ez.stock->stock2Trend = 0;
    ez.stock->stock3Trend = 0;
    ez.stock->stock1[0] = GetRandomValue(50,250);
    ez.stock->stock2[0] = GetRandomValue(50,250);
    ez.stock->stock3[0] = GetRandomValue(50,250);
    ez.stock->doubleCounter = 0;
    ez.stock->ownedStock1=0;
    ez.stock->ownedStock2=0;
    ez.stock->ownedStock3=0;
    ez.stock->funds = 100;
    ez.stock->netProfit=0;
    return state;
}

void processStockValue(int timeStep, int* stock,int& trend,int volatility) {
    if (timeStep < STOCK_HISTORY_LENGTH) {
        int currentVal = GetRandomValue(50,250);
        if (timeStep!=0) {
            currentVal = stock[timeStep-1];
        }
        stock[timeStep] = GetRandomValue((int)std::max(1,currentVal-volatility+trend),(int)std::min(300,currentVal+volatility+trend));
        //compute the trend
        if (timeStep>=1) {
            trend = (stock[timeStep] - stock[timeStep-1])/3;
        }
        if (timeStep>=2) {
            trend += (stock[timeStep-1]-stock[timeStep-2])/3;
        }
    } else {
        //bubble each elemnt to the left
        for (int i=0;i<STOCK_HISTORY_LENGTH-1;i++) {
            stock[i] = stock[i+1];
        }
        int currentVal = stock[STOCK_HISTORY_LENGTH-1];
        stock[STOCK_HISTORY_LENGTH-1] = GetRandomValue((int)std::max(0,currentVal-volatility+trend),(int)std::min(300,currentVal+volatility+trend));
        // compute the trend
        trend = (stock[STOCK_HISTORY_LENGTH-1] - stock[STOCK_HISTORY_LENGTH-2])/3 + (stock[STOCK_HISTORY_LENGTH-2]- stock[STOCK_HISTORY_LENGTH-3])/3;
    }
}

void drawSimpleButton(int x, int y, int width,int height, std::string text, Color color) {
    DrawRectangle(x,y,width,height,color);
    const int textWidth = MeasureText(text.c_str(),30);//calculate how long the text on the button is
    const int textRight = x + width/2 - textWidth/2;//calculate the right x coord of the text
    DrawText(text.c_str(),textRight,y+(height-30)/2,30,BLACK);//render the text on the button
}

bool mouseOnSimpleButton(int x, int y, int width,int height, int mouseX, int mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        return true;
    }
    return false;
}

int getStockPrice(stockMarketState * state, int stock) {
    //the time index or something
    int tind = std::min(state->time_step,STOCK_HISTORY_LENGTH-1);
    switch (stock) {
    case 1:
        return state->stock1[tind];
    case 2:
        return state->stock2[tind];
    case 3:
        return state->stock3[tind];
    default:
        return 0;
    }
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
    std::string profitText = "Net: $";
    profitText+=std::to_string(s->netProfit);
    int netLength = MeasureText(profitText.c_str(),30);
    if (s->netProfit < 0) {
        DrawText(profitText.c_str(),x+490-netLength,y+20,30,RED);
    } else {
        DrawText(profitText.c_str(),x+490-netLength,y+20,30,GREEN);
    }

    int lineLength = 500/STOCK_HISTORY_LENGTH;
    for (int i=0;i<STOCK_HISTORY_LENGTH-1;i++) {
        DrawLine(x+i*lineLength,y+400-s->stock1[i],x+i*lineLength+lineLength,y+400-s->stock1[i+1],RED);
        DrawLine(x+i*lineLength,y+400-s->stock2[i],x+i*lineLength+lineLength,y+400-s->stock2[i+1],ORANGE);
        DrawLine(x+i*lineLength,y+400-s->stock3[i],x+i*lineLength+lineLength,y+400-s->stock3[i+1],BLUE);
    }

    DrawRectangle(x,y+95,500,2,GRAY);
    DrawRectangle(x,y+400,500,2,GRAY);

    s->doubleCounter++;
    if (s->doubleCounter%8==0) {
        s-> time_step ++;
        processStockValue(s->time_step,s->stock1,s->stock1Trend,4);
        processStockValue(s->time_step,s->stock2,s->stock2Trend,6);
        processStockValue(s->time_step,s->stock3,s->stock3Trend,8);
    }

    //draw the buttons
    drawSimpleButton(6+x,455+y,70,40,"Buy",s->funds > getStockPrice(s,1)? RED:GRAY);
    drawSimpleButton(6+83+x,455+y,70,40,"Sell",s->ownedStock1 > 0?RED:GRAY);
    drawSimpleButton(6+83*2+x,455+y,70,40,"Buy",s->funds > getStockPrice(s,2)? ORANGE:GRAY);
    drawSimpleButton(6+83*3+x,455+y,70,40,"Sell",s->ownedStock2 > 0?ORANGE:GRAY);
    drawSimpleButton(6+83*4+x,455+y,70,40,"Buy",s->funds > getStockPrice(s,3)? BLUE:GRAY);
    drawSimpleButton(6+83*5+x,455+y,70,40,"Sell",s->ownedStock3 > 0?BLUE:GRAY);

    std::string stock1Level = std::to_string(s->ownedStock1);
    std::string stock2Level = std::to_string(s->ownedStock2);
    std::string stock3Level = std::to_string(s->ownedStock3);

    DrawText(stock1Level.c_str(),x+10,y+420,30,RED);
    DrawText(stock2Level.c_str(),x+180,y+420,30,ORANGE);
    DrawText(stock3Level.c_str(),x+350,y+420,30,BLUE);

}

void stockMarketClicked(void * state, int button, int mouseX, int mouseY) {
    ezAutoCasting ez{};
    ez.in = state;
    if(mouseOnSimpleButton(6,455,70,40,     mouseX,mouseY) && ez.stock->funds > getStockPrice(ez.stock,1)){
        ez.stock->ownedStock1++;
        ez.stock->funds -= getStockPrice(ez.stock,1);
        ez.stock->netProfit -= getStockPrice(ez.stock,1);
    }
    if(mouseOnSimpleButton(6+83,455,70,40,  mouseX,mouseY) && ez.stock->ownedStock1 > 0){
        ez.stock->ownedStock1--;
        ez.stock->funds += getStockPrice(ez.stock,1);
        ez.stock->netProfit += getStockPrice(ez.stock,1);
    }
    if(mouseOnSimpleButton(6+83*2,455,70,40,mouseX,mouseY) && ez.stock->funds > getStockPrice(ez.stock,2)){
        ez.stock->ownedStock2++;
        ez.stock->funds -= getStockPrice(ez.stock,2);
        ez.stock->netProfit -= getStockPrice(ez.stock,2);
    }
    if(mouseOnSimpleButton(6+83*3,455,70,40,mouseX,mouseY) && ez.stock->ownedStock2 > 0){
        ez.stock->ownedStock2--;
        ez.stock->funds += getStockPrice(ez.stock,2);
        ez.stock->netProfit += getStockPrice(ez.stock,2);
    }
    if(mouseOnSimpleButton(6+83*4,455,70,40,mouseX,mouseY) && ez.stock->funds > getStockPrice(ez.stock,3)){
        ez.stock->ownedStock3++;
        ez.stock->funds -= getStockPrice(ez.stock,3);
        ez.stock->netProfit -= getStockPrice(ez.stock,3);
    }
    if(mouseOnSimpleButton(6+83*5,455,70,40,mouseX,mouseY) && ez.stock->ownedStock3 > 0){
        ez.stock->ownedStock3--;
        ez.stock->funds += getStockPrice(ez.stock,3);
        ez.stock->netProfit += getStockPrice(ez.stock,3);
    }
}

bool stockMarkeySuccess(void * state) {
    ezAutoCasting ez{};
    ez.in = state;
    return ez.stock->netProfit > 500;//temp code!
}

std::string stockMarketGetInstructions(void * state) {
    return "Make $500 in the socket market";
}

//harm the human
//variables
Texture car;
Texture human;
Texture road;
bool hthLoadded = false;

struct harmHumanState{
    //variables to hold state information
    int humanLocation;
    int possiblePositions[3];
};

void * harmHuamnCreate(){
    //allocates strutcure and initalize variables
    //If this is the first time this function is called, load the assets
    if(!hthLoadded){
        car = LoadTexture("assets/team2/BMWcarWindow.png");
        human = LoadTexture("assets/team2/FuckingFatass.png");
        road = LoadTexture("assets/team2/Road.png");
        hthLoadded = true;
    }
    void * memBlock = malloc(sizeof(harmHumanState));
        harmHumanState * state = (harmHumanState*)memBlock;
        state->humanLocation = GetRandomValue(0,2);
    return memBlock;
}

void harmHumanDraw(void * state, int x, int y){
    //passes the state in every frame, x & y is within the captcha window
    const auto * human_state = (harmHumanState*)state;
    int possiblePositions[3] = {x+50,x+200,x+350};
    int pos = possiblePositions[human_state->humanLocation];

    DrawTexturePro(road,{0,0, (float)road.width, (float)road.height},{(float)x, (float)y, 500, 500}, {0, 0}, 0, WHITE);
    DrawTexturePro(human,{0,0, (float)human.width, (float)human.height},{(float)pos, (float)y, 100, 100}, {0, 0}, 0, WHITE);
    DrawTexturePro(car,{0,0, (float)car.width, (float)car.height},{(float)x, (float)y, 500, 500}, {0, 0}, 0, WHITE);
}

void harmHumanMouseCLicked(void * state, int button, int mouseX, int mouseY){
    //if the user clicks the mouse within the area, call it
    //x & y are within the captcha window
}

bool harmHumanCheckSuccess(void * state){
    //returns true if captcha is in a completed state
    return false;
}

std::string harmHumanGetInstructions(void * state){
    //returns the instructions of the captcha
    return "Choose the road with the human";
}

void play_music(){
    // PlaySound(TEXT("DrivingInMyCar.mp3"), NULL, SND_SYNC);
}


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
        },

        //harm the human
        {
            "Harm the human",
            500,
            500,
            &harmHuamnCreate,
            &harmHumanDraw,
            &harmHumanMouseCLicked,
            &harmHumanCheckSuccess,
            &harmHumanGetInstructions
        },



        //teacton time
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