#include "team2.h"
#include <iostream> //cout
#include "include/raylib.h"
#include <vector>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

//prototypes

//consts

//shark

//stock market


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
    DrawTexture(road,x,y,WHITE);
    DrawTexture(car,x,y,WHITE);
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
    PlaySound(TEXT("DrivingInMyCar.mp3"), NULL, SND_SYNC);
}


//teacton time



//this should be at the bottom
std::vector<captchaInfo> team2_get_captchas() {
    return {
        //shark

        //stock market


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
        }



        //teacton time


    };
}