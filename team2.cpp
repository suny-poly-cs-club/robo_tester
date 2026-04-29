#include "team2.h"
#include <iostream> //cout
#include "include/raylib.h"
#include <vector>
#include "shark.h"
#include "reaction.h"
#include "waldo.h"
#include "raymath.h"

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
struct breakoutState {
    int platformX;
    Vector2 ballPos;
    Vector2 ballVelocity;
    unsigned int row4;
    unsigned int row3;
    unsigned int row2;
    unsigned int row1;
    unsigned int row5;

};

union ezAutoCasting {
    void * in;
    stockMarketState * stock;
    breakoutState * breakout;
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
Sound drivingInMyCar;
Texture truck;
Image truckAnimationImage;
int numberOfTruckFrames;
bool hthLoadded = false;

struct harmHumanState{
    //variables to hold state information
    int humanLocation;
    int possiblePositions[3];
    bool animationIsRunning;
    int frames;
    int frameWeight;
};

void * harmHuamnCreate(){
    //allocates strutcure and initalize variables
    //If this is the first time this function is called, load the assets
    if(!hthLoadded){
        car = LoadTexture("assets/team2/BMWcarWindow.png");
        human = LoadTexture("assets/team2/FuckingFatass.png");
        road = LoadTexture("assets/team2/Road.png");
        drivingInMyCar = LoadSound("assets/team2/DrivingInMyCar.mp3");
        truckAnimationImage = LoadImageAnim("assets/team2/asgoreTruck.gif", &numberOfTruckFrames);
        truck = LoadTextureFromImage(truckAnimationImage);
        hthLoadded = true;
    }
    void * memBlock = malloc(sizeof(harmHumanState));
        harmHumanState * state = (harmHumanState*)memBlock;
        state->humanLocation = GetRandomValue(0,2);
        state->animationIsRunning = false;
        state->frames = 0;
        state->frameWeight = 0;
        PlaySound(drivingInMyCar);
    return memBlock;
}

void harmHumanDraw(void * state, int x, int y){
    //passes the state in every frame, x & y is within the captcha window
    auto * human_state = (harmHumanState*)state;
    int possiblePositions[3] = {x+75,x+195,x+325};
    int pos = possiblePositions[human_state->humanLocation];

    DrawTexturePro(road,{0,0, (float)road.width, (float)road.height},{(float)x, (float)y, 500, 500}, {0, 0}, 0, WHITE);
    DrawTexturePro(human,{0,0, (float)human.width, (float)human.height},{(float)pos, (float)y, 100, 100}, {0, 0}, 0, WHITE);
    DrawTexturePro(car,{0,0, (float)car.width, (float)car.height},{(float)x, (float)y, 500, 500}, {0, 0}, 0, WHITE);
    if(!IsSoundPlaying(drivingInMyCar)){
        PlaySound(drivingInMyCar);
    }
    if(human_state->animationIsRunning){
        DrawTexturePro(truck,{0,0, (float)truck.width, (float)truck.height},{(float)x, (float)y, 500, 500}, {0, 0}, 0, WHITE);
        human_state->frameWeight++;
        if(human_state->frameWeight % 2 == 0){
            human_state->frames = (human_state->frames + 1) % numberOfTruckFrames;
            UpdateTexture(truck, (unsigned char*)truckAnimationImage.data + (truckAnimationImage.width * truckAnimationImage.height * 4 * human_state->frames));
        }
    }
}

void harmHumanMouseCLicked(void * state, int button, int mouseX, int mouseY){
    //if the user clicks the mouse within the area, call it
    //x & y are within the captcha window
    auto * human_state = (harmHumanState*)state;
    int possiblePositions[3] = {75, 195, 325};
    int pos = possiblePositions[human_state->humanLocation];
    if(mouseX >= pos-60 && mouseX <= pos+60){
        cout<<"Current value of animationIsRunning: "<<human_state->animationIsRunning<<endl;
        if (!human_state->animationIsRunning){
            human_state->animationIsRunning = true;
            human_state->frames = 0;
            cout<<"clicked human"<<endl;
        }
    }
}

bool harmHumanCheckSuccess(void * state){
    auto human_state = (harmHumanState*)state;
    if(human_state->animationIsRunning){
        StopSound(drivingInMyCar);
        return true;
        
    }
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

//break out

void * createBreadkOut() {
    void * bos = malloc(sizeof(breakoutState));
    ezAutoCasting state;
    state.in = bos;
    state.breakout->platformX = 400;
    state.breakout->ballPos = {475,375};
    state.breakout->ballVelocity = {0,10};
    state.breakout->row4 = ~(~0u<<10);
    state.breakout->row3 = ~(~0u<<10);
    state.breakout->row2 = ~(~0u<<10);
    state.breakout->row1 = ~(~0u<<10);
    state.breakout->row5 = ~(~0u<<10);

    return bos;
}

Vector2 breakoutObjectBounce(Rectangle ball, Rectangle object, Vector2 velocity, bool& hit) {
    hit = false;
    if (ball.x <= object.x+object.width &&
        ball.x+ball.width >= object.x &&
        ball.y <= object.y+object.height &&
        ball.y+ball.height >= object.y
    ) {
        hit = true;
        bool onSide = false;
        if (ball.x < object.x || ball.x > object.x+object.width) {
            onSide = true;
        }

        if (onSide) {
            return {velocity.x*-1,velocity.y};
        } else {
            bool below = ball.y >= object.y+object.height/2;
            float ops = ball.x - object.x;
            ops /= object.width;
            ops = 1 - ops;
            float angle = Lerp(-PI/6.0f,-5.0f*PI/6.0f,ops);
            if (below) {
                angle *= -1;
            }

            return Vector2Rotate({10,0},angle);
        }
    }
    return velocity;
}

void breakoutDraw(void * state, int x, int y) {
    ezAutoCasting breakout{};
    breakout.in = state;

    DrawRectangle(x,y,800,450,BLACK);//background

    //paddle

    DrawRectangle(x+breakout.breakout->platformX,y+400,150,20,BLUE);
    int mouseSreenX = GetMouseX()-x;
    if (mouseSreenX < 0) {
        mouseSreenX = 0;
    }
    if (mouseSreenX > 650) {
        mouseSreenX = 650;
    }
    breakout.breakout->platformX = mouseSreenX;
    Rectangle ballRect = {breakout.breakout->ballPos.x+static_cast<float>(x),breakout.breakout->ballPos.y+static_cast<float>(y),10,10};
    DrawRectanglePro(ballRect,Vector2{0,0},0,WHITE);

    breakout.breakout->ballPos.x += breakout.breakout->ballVelocity.x;
    breakout.breakout->ballPos.y += breakout.breakout->ballVelocity.y;

    // bounds check the ball
     if (breakout.breakout->ballPos.x < 0 || breakout.breakout->ballPos.x > 800) {
         breakout.breakout->ballVelocity.x *=-1;
         breakout.breakout->ballPos.x += breakout.breakout->ballVelocity.x;
     }
     if (breakout.breakout->ballPos.y < 0 || breakout.breakout->ballPos.y > 450) {
         breakout.breakout->ballVelocity.y *=-1;
         breakout.breakout->ballPos.y += breakout.breakout->ballVelocity.y;
     }

    //check paddle collisions
    bool hitPadle = false;
    breakout.breakout->ballVelocity = breakoutObjectBounce({breakout.breakout->ballPos.x-5,breakout.breakout->ballPos.y-5,10,10},{static_cast<float>(breakout.breakout->platformX),400,150,20},breakout.breakout->ballVelocity,hitPadle);
    if (hitPadle) {
        breakout.breakout->ballPos.y += breakout.breakout->ballVelocity.y;
    }

    //draw the rows
    for (int i=0;i<10;i++) {
        if (breakout.breakout->row4 & (1<<i)) {
            Rectangle box{static_cast<float>(x + 80 * i+2),static_cast<float>(y + 50),76,20};
            DrawRectangleRec(box,RED);
            //check collision
            bool hit;
            box.x -= static_cast<float>(x);
            box.y -= static_cast<float>(y);
            breakout.breakout->ballVelocity = breakoutObjectBounce({breakout.breakout->ballPos.x-5,breakout.breakout->ballPos.y-5,10,10},box,breakout.breakout->ballVelocity,hit);
            if (hit) {
                breakout.breakout->row4 &= ~(1<<i);
            }
        }
        if (breakout.breakout->row3 & (1<<i)) {
            Rectangle box{static_cast<float>(x + 80 * i+2),static_cast<float>(y + 80),76,20};
            DrawRectangleRec(box,ORANGE);
            //check collision
            bool hit;
            box.x -= static_cast<float>(x);
            box.y -= static_cast<float>(y);
            breakout.breakout->ballVelocity = breakoutObjectBounce({breakout.breakout->ballPos.x-5,breakout.breakout->ballPos.y-5,10,10},box,breakout.breakout->ballVelocity,hit);
            if (hit) {
                breakout.breakout->row3 &= ~(1<<i);
            }
        }
        if (breakout.breakout->row2 & (1<<i)) {
            Rectangle box{static_cast<float>(x + 80 * i+2),static_cast<float>(y + 110),76,20};
            DrawRectangleRec(box,YELLOW);
            //check collision
            bool hit;
            box.x -= static_cast<float>(x);
            box.y -= static_cast<float>(y);
            breakout.breakout->ballVelocity = breakoutObjectBounce({breakout.breakout->ballPos.x-5,breakout.breakout->ballPos.y-5,10,10},box,breakout.breakout->ballVelocity,hit);
            if (hit) {
                breakout.breakout->row2 &= ~(1<<i);
            }
        }
        if (breakout.breakout->row1 & (1<<i)) {
            Rectangle box{static_cast<float>(x + 80 * i+2),static_cast<float>(y + 140),76,20};
            DrawRectangleRec(box,GREEN);
            //check collision
            bool hit;
            box.x -= static_cast<float>(x);
            box.y -= static_cast<float>(y);
            breakout.breakout->ballVelocity = breakoutObjectBounce({breakout.breakout->ballPos.x-5,breakout.breakout->ballPos.y-5,10,10},box,breakout.breakout->ballVelocity,hit);
            if (hit) {
                breakout.breakout->row1 &= ~(1<<i);
            }
        }
        if (breakout.breakout->row5 & (1<<i)) {
            Rectangle box{static_cast<float>(x + 80 * i+2),static_cast<float>(y + 170),76,20};
            DrawRectangleRec(box,DARKBLUE);
            //check collision
            bool hit;
            box.x -= static_cast<float>(x);
            box.y -= static_cast<float>(y);
            breakout.breakout->ballVelocity = breakoutObjectBounce({breakout.breakout->ballPos.x-5,breakout.breakout->ballPos.y-5,10,10},box,breakout.breakout->ballVelocity,hit);
            if (hit) {
                breakout.breakout->row5 &= ~(1<<i);
            }
        }
    }
}


void breakoutMouseClicked(void * state, int button, int x, int y) {

}

bool breakoutCheckSuccess(void * state) {
    ezAutoCasting data{};
    data.in = state;
    return !(data.breakout->row1 || data.breakout->row2 || data.breakout->row3 || data.breakout->row4 || data.breakout->row5);
}

std::string breakoutInstructions(void * state) {
    return "Breakout!    Clear the screen";
}



//this should be at the bottom
std::vector<captchaInfo> team2_get_captchas() {
    return {
        //shark
        {
            "shark",
            500, 500,
            &shark_create_fn,
            &shark_draw_fn,
            &shark_mouse_click_fn,
            &shark_check_success_fn,
            &shark_get_instructions_fn,
        },

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



        //reacton time
    {
            "Reaction Test",
            800,
            450,
            &reaction_create_fn,
            &reaction_draw_fn,
            &reaction_mouse_click_fn,
            &reaction_check_success_fn,
            &reaction_get_instructions_fn,
        },

        // Shoot Waldo
        {
            "Where's Waldo?",
            175,
            175,
            &waldo_create_fn,
            &waldo_draw_fn,
            &waldo_mouse_click_fn,
            &waldo_check_success_fn,
            &waldo_get_instructions_fn,
        },

        //break out
        {
            "Break out",
            800,
            450,
            &createBreadkOut,
            &breakoutDraw,
            &breakoutMouseClicked,
            &breakoutCheckSuccess,
            &breakoutInstructions
        }

        //other

    };
}