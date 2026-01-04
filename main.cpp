#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_HEAD_HEIGHT 80
#define WINDOW_FOOT_HEIGHT 40
#define NUMBER_OF_CAPTCHASES 5

#include "include/raylib.h"
#include "common_structs.h"
#include "example.h"
#include <vector>
#include <iostream>
#include <queue>


using namespace std;

vector<captchaInfo> allCaptcha;
queue<size_t> queuedCaptchas;


struct CaptchaInstanceInfo {
    int captchaIndex;
    int x;
    int y;
    void * state;
    bool initialised;

    captchaInfo getInfo() const {
        return allCaptcha[captchaIndex];
    }

    void draw() const {
        //check if there is a draw funcion
        if (state != nullptr && getInfo().draw_function != nullptr) {
            getInfo().draw_function(state,x,y);//run the draw function
        }
    }
};

CaptchaInstanceInfo currentCaptcha = {-1,0,0,nullptr,false};
button playButton = {{SCREEN_WIDTH/2-75,SCREEN_HEIGHT/2-25,150,50},SKYBLUE,"Play"};
long frameCounter = 0;
int countDown = -1;
bool completed = false;
Vector2 movingCaptchaOffset = {-1000,-1000};

void initCaptcha(const size_t index) {
    currentCaptcha = {//create the captcha runtime structure
        (int)index,
        GetRandomValue(0,SCREEN_WIDTH - allCaptcha[index].width),
        GetRandomValue(WINDOW_HEAD_HEIGHT,SCREEN_HEIGHT - allCaptcha[index].height - WINDOW_FOOT_HEIGHT),
        nullptr,
        true
    };

    //ensure there is a creat function for this captcha
    if (currentCaptcha.getInfo().create_function == nullptr) {
        cerr << "ERROR: null create function for captcha "+currentCaptcha.getInfo().name << endl;
        return;//TODO crash here
    }
    //create the captcha
    currentCaptcha.state = currentCaptcha.getInfo().create_function();
}

void drawCaptcha(const CaptchaInstanceInfo &captcha) {

    string instructionText = "Captcha";
    if (captcha.getInfo().get_instructions_function != nullptr) {
        instructionText = captcha.getInfo().get_instructions_function(captcha.state);
    }

    //general backdrop
    DrawRectangle(captcha.x-2,captcha.y-2 -WINDOW_HEAD_HEIGHT,captcha.getInfo().width+4,captcha.getInfo().height + WINDOW_HEAD_HEIGHT + 4 + WINDOW_FOOT_HEIGHT,LIGHTGRAY);
    //blue info section at the top
    DrawRectangle(captcha.x,captcha.y-WINDOW_HEAD_HEIGHT,captcha.getInfo().width, WINDOW_HEAD_HEIGHT,Color{78, 147, 220,255});
    //info text
    DrawText(instructionText.c_str(),captcha.x + 10,captcha.y+10-WINDOW_HEAD_HEIGHT,20,WHITE);
    //content background
    DrawRectangle(captcha.x,captcha.y,captcha.getInfo().width,captcha.getInfo().height,WHITE);

    //draw the captcha
    captcha.draw();

    //verify button
    DrawRectangle(captcha.x + captcha.getInfo().width - 120, captcha.y + captcha.getInfo().height + 5,100,30,Color{78, 147, 220,255});
    DrawText("Verify",captcha.x + captcha.getInfo().width - 105, captcha.y + captcha.getInfo().height + 10,20,WHITE);
}

bool mouseOverCaptchaHeader(const CaptchaInstanceInfo &captcha) {
    const Vector2 mousePos = GetMousePosition();
    return mousePos.x > captcha.x && mousePos.x < captcha.x + captcha.getInfo().width && mousePos.y > captcha.y-WINDOW_HEAD_HEIGHT && mousePos.y < captcha.y;
}

void draw() {
    frameCounter++;
    ClearBackground(GRAY); //clear the screen and set the background color to gray

    if (countDown > 0) {//decrease the counter if it is more then 0
        countDown--;
    }

    //loading animation dots
    //dot 1 if more then 10 frames
    if (frameCounter % 60 > 10) {
        DrawCircle(SCREEN_WIDTH/2 - 50,SCREEN_HEIGHT/2,20,LIGHTGRAY);
    }
    //dot 2 if more then 30 frames
    if (frameCounter % 60 > 30) {
        DrawCircle(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,20,LIGHTGRAY);
    }
    //dot 3 if more then 45 frames
    if (frameCounter % 60 > 45) {
        DrawCircle(SCREEN_WIDTH/2 + 50,SCREEN_HEIGHT/2,20,LIGHTGRAY);
    }

    if (completed) {
        DrawText("Error, you appear to be a robot",100,100,60,RED);
    }

    //check for if a captcha is currently being shown
    if (currentCaptcha.initialised) {
        //check if the captcha is being moved
        if (movingCaptchaOffset.x > -1000) {
            const Vector2 mousePos = GetMousePosition();
            const Vector2 newCapPos = {movingCaptchaOffset.x+mousePos.x, movingCaptchaOffset.y+mousePos.y};
            currentCaptcha.x = (int)newCapPos.x;
            currentCaptcha.y = (int)newCapPos.y;
        }

        //draw the captcha
        drawCaptcha(currentCaptcha);
    } else {
        //if no captcha open then draw the other things

        //if there are captcha in the queue then initialize the next one
        if (!queuedCaptchas.empty()) {
            initCaptcha(queuedCaptchas.front());
            queuedCaptchas.pop();
        } else {
            //if the countdown has not happened yet
            if (countDown  == -1) {
                //draw the play button
                drawButton(playButton);
            }
        }
    }
    //when the countdown is about to be over
    if (countDown == 1) {
        //generate the captcha that will be shown
        int* captchaSequence = LoadRandomSequence(NUMBER_OF_CAPTCHASES,0,1000);//get a list of random numbers
        for (int i=0;i<NUMBER_OF_CAPTCHASES;i++) {//go through the list and add each number to the queue
            queuedCaptchas.push(captchaSequence[i] % allCaptcha.size());
        }
        UnloadRandomSequence(captchaSequence);
    }
}


void leftMouseClicked() {
    const Vector2 mousePos = GetMousePosition();
    if (currentCaptcha.initialised) {//if there is a captcha
        //check if the mouse is over the verify button
        if ((int)mousePos.x >= currentCaptcha.x + currentCaptcha.getInfo().width-120 &&
            (int)mousePos.x <= currentCaptcha.x + currentCaptcha.getInfo().width-20 &&
            (int)mousePos.y >= currentCaptcha.y + currentCaptcha.getInfo().height + 5 &&
            (int)mousePos.y <= currentCaptcha.y + currentCaptcha.getInfo().height + 35) {
            cout << "verrifying" << endl;
            if (currentCaptcha.getInfo().check_success_function != nullptr) {
                const bool success = currentCaptcha.getInfo().check_success_function(currentCaptcha.state);
                if (success) {
                    cout << "SUCCESS!!!" << endl;
                    free(currentCaptcha.state);//deallocate the memory associated with the state info of the current captcha
                    currentCaptcha = {-1,0,0,nullptr,false};//uninitialize the captcha
                    if (queuedCaptchas.empty()) {
                        completed = true;
                    }
                    return;
                }
            }
        }
    }

}

bool loadCaptchas(const vector<captchaInfo>& v) {
    for (const captchaInfo & captcha : v) {
        //check for a valid name
        if (captcha.name == "list") {
            cerr << "Illegal captcha name: list"<<endl;
            return false;
        }
        for (captchaInfo & existingCaptcha: allCaptcha) {
            if (captcha.name == existingCaptcha.name) {
                cerr << "Duplicate captcha name: " << captcha.name << endl;
                return false;
            }
        }
        if (captcha.width < 175) {
            cerr << "The width of captcha " << captcha.name << " is less then the minimum with of 175"<<endl;
            return false;
        }
        allCaptcha.emplace_back(captcha);
    }
    return true;
}

int main(const int argc, char** argv) {
    //setup memory for the state if each mouse button
    bool mouseStates[] = {false,false,false,false,false,false,false};

    void * rp = malloc(8);//get a random dynamic pointer
    //compute a random seed using a large number of unknown and possibly random values
    const long long randomSeed = (long long)&cout ^ (long long)&printf ^ (long long)&draw ^
        (long long)&ClearBackground ^ (long long)&mouseStates ^ (long long)rp ^ *(long long*)rp
    * time(nullptr);
    free(rp);//deallocate that dynamic pointer
    SetRandomSeed((unsigned int)randomSeed);//set a good random seed

    //ingest the commandline args into a string vector
    vector<string> args;
    for(int i=0;i<argc;i++){
        args.emplace_back(argv[i]);
    }
    //get all captcha definitions

    if (!loadCaptchas(example_get_captchas())) {
        return EXIT_FAILURE;
    }


    //process args
    //arg 0 is the executable
    for(size_t k=1;k<args.size();k++){
        //check if the user is asking to list all captcha
        if(args[k] == "list"){
            for(captchaInfo & captcha : allCaptcha){
                cout << captcha.name << endl;
            }
            return EXIT_SUCCESS;
        }
        //check if the user wants to test a specific captcha
        bool found = false;
        for (size_t i=0;i < allCaptcha.size();i++) {
            if (args[k] == allCaptcha[i].name) {
                queuedCaptchas.push(i);//add the found captcha to the queue of captchas to display
                found = true;
                countDown = 0;//set the countdown to 0 so the captcha displays instantly
                break;
            }
        }
        if (!found) {
            cerr << "Unknown captcha: " << args[k] << endl;
            return EXIT_FAILURE;
        }

    }

    // window Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Robot Tester");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second


    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();//set up the frame buffer to start drawing
        draw();//draw our stuff
        EndDrawing();//tell the frame buffer it can stop drawing and show the image
        //----------------------------------------------------------------------------------

        //check mouse events
        for (int i=0;i<7;i++) {//for all the mouse buttons
            const bool buttonState = IsMouseButtonDown(i);//get the current state of the button
            if (buttonState && ! mouseStates[i]) { // if the button was just presses and was not pressed last frame
                //process mouse presses
                if (currentCaptcha.initialised) {
                    const Vector2 mousePosition = GetMousePosition();
                    if (currentCaptcha.getInfo().mouse_clicked_function != nullptr) {//ensure the mouse clicked function has been defined
                        //check if the mouse is over the captcha window
                        const Vector2 capMousePos = {mousePosition.x - (float)currentCaptcha.x, mousePosition.y - (float)currentCaptcha.y};
                        if (capMousePos.x > 0 && (int)capMousePos.x < currentCaptcha.getInfo().width &&
                            capMousePos.y > 0 && (int)capMousePos.y < currentCaptcha.getInfo().height) {
                            //run the mouse clicked function for this captcha
                            currentCaptcha.getInfo().mouse_clicked_function(currentCaptcha.state,i,(int)capMousePos.x,(int)capMousePos.y);
                        }
                    }
                    if (i == MOUSE_BUTTON_LEFT) {
                        if (mouseOverCaptchaHeader(currentCaptcha)) {//check if the mouse clicked on the title box of the captcha
                            //if so initialize the captcha movement values
                            movingCaptchaOffset = {currentCaptcha.x - mousePosition.x,currentCaptcha.y - mousePosition.y};
                        }
                    }
                } else if (i == MOUSE_BUTTON_LEFT) {//if there is no captcha
                    if (countDown  == -1) {//and the countdown has not been started
                        if (checkMouseOverButton(playButton)) {//if the play button has been clicked
                            countDown = 120;//set the countdown to 2 seconds
                        }
                    }

                }
                if (i==MOUSE_BUTTON_LEFT) {//if left button
                    leftMouseClicked();
                }
            }
            mouseStates[i] = buttonState;//update the remembered state of this mouse button
            if (i==MOUSE_BUTTON_LEFT && !buttonState && movingCaptchaOffset.x > -1000) {
                movingCaptchaOffset = {-1000,-1000};
            }
        }

    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}

/**Draw a button to the screen
 * @param buttonIn The button to draw
 */
void drawButton(const button &buttonIn) {
    DrawRectangleRec(buttonIn.size, buttonIn.color);//draw the rectangle background
    const int textWidth = MeasureText(buttonIn.text.c_str(),30);//calculate how long the text on the button is
    const int textRight = buttonIn.size.x + buttonIn.size.width/2 - textWidth/2;//calculate the right x coord of the text
    DrawText(buttonIn.text.c_str(),textRight,(int)buttonIn.size.y+(int)(buttonIn.size.height-30)/2,30,BLACK);//render the text on the button
}

/**Check if the mouse is over a button
 * @param buttonIn The button to check with
 * @return true if the mouse is over the provided button
 */
bool checkMouseOverButton(const button &buttonIn) {
    const Vector2 mouse = GetMousePosition();//get the mouse position
    return checkMouseOverButton(buttonIn, mouse);//check if the mouse is over the button
}

/**Check if the mouse is over a button
 * @param buttonIn The button to check with
 * @param mousePos The position of the mouse
 * @return true if the provided mouse position is over the button
 */
bool checkMouseOverButton(const button &buttonIn, const Vector2 &mousePos) {
    //check if the mouse position falls inside the button
    if (mousePos.x >= buttonIn.size.x && mousePos.x <= buttonIn.size.x + buttonIn.size.width && mousePos.y >= buttonIn.size.y && mousePos.y <= buttonIn.size.y + buttonIn.size.height) {
        return true;
    }
    return false;
}