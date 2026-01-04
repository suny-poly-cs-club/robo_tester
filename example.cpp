#include "example.h"
#include <iostream>

#include "include/raylib.h"
#include <vector>


struct check_box_captcha_state { //struct to hold state information for the checkbox captcha
    bool checked;
};

void * create_check_box_captcha() {
    //dynamically allocate space for our state structure
    void * memBlock = malloc(sizeof(check_box_captcha_state));
    //initialize its value
    ((check_box_captcha_state*)memBlock)->checked = false;
    //give the memory block to the main program
    return memBlock;
}

void check_box_draw(void * state, int x, int y) {
    //when auto is used the compiler will automatically determine the type
    const auto * checkState = (check_box_captcha_state*)state;
    //draw the back background for the checkbox
    DrawRectangle(x + 65, y+3,44,44,BLACK);
    //notice how the position is offset from x and y and not set absolutely
    DrawRectangle(x + 67, y+5,40,40,LIGHTGRAY);

    //draw the check if the box is checked
    if (checkState->checked) {
        //draw the lines of the check mark
        DrawLineEx({(float)x+87,(float)y+40},{(float)x+107,(float)y+5},5,RED);
        DrawLineEx({(float)x+87,(float)y+40},{(float)x+77,(float)y+25},5,RED);
    }
}

void check_box_mouse_clicked(void * state, int button, int mouseX, int mouseY) {
    //when auto is used the compiler will automatically determine the type
    auto * checkState = (check_box_captcha_state*)state;

    //check if the mouse is over the checkbox
    if (button == MOUSE_BUTTON_LEFT && mouseX > 67 && mouseX < 107 && mouseY > 5 && mouseY < 45) {
        //note that the inputs are relative to the captcha window
        checkState->checked = !checkState->checked;
    }
}

bool check_box_checked(void * state) {
    //when auto is used the compiler will automatically determine the type
    const auto * checkState = (check_box_captcha_state*)state;
    return checkState->checked;
}

std::string check_box_get_instructions(void * state) {
    //this is going to be constant so we do not need to use the state. it is still required for the function to be accepted tho
    return "check the box";
}

//global array to hold images
std::vector<Texture2D> images;
//global array to hold the data related to the images
std::vector<bool> image_data = {true,true,true,true,true,true,true,false,true,false,false,false,false,false,false,false};

struct image_captcha_state {//structure to hold state infomation for the image captcha
    bool lookingFor;
    bool selected[9];
    int images[9];
};

void * create_image_captcha() {
    //ensure that the images are only loaded once
    if (images.empty()) {
        for (int i=1;i<=16;i++) {
            //create the string with the file path
            std::string filename = "assets/example/";
            filename += std::to_string(i);
            filename +=".png";
            //load the image into system RAM
            Image image = LoadImage(filename.c_str());
            //resize the image to be the size we want
            ImageResize(&image,160,160);
            //load the image into GPU RAM and add it to the images array
            images.push_back(LoadTextureFromImage(image));
            //remove the image from system ram
            UnloadImage(image);
        }
    }
    //dynamically allocate space for our state structure
    void * memBlock = malloc(sizeof(image_captcha_state));
    const auto cap_state = (image_captcha_state*)memBlock;
    //initialize the selected states to false
    cap_state->selected[0] = false;
    cap_state->selected[1] = false;
    cap_state->selected[2] = false;
    cap_state->selected[3] = false;
    cap_state->selected[4] = false;
    cap_state->selected[5] = false;
    cap_state->selected[6] = false;
    cap_state->selected[7] = false;
    cap_state->selected[8] = false;
    //get a random looking for value
    cap_state->lookingFor = GetRandomValue(0,1);
    //get a random sequence of numbers to choose the images to display
    int * randomSequence = LoadRandomSequence(9,0,15);
    cap_state->images[0] = randomSequence[0];
    cap_state->images[1] = randomSequence[1];
    cap_state->images[2] = randomSequence[2];
    cap_state->images[3] = randomSequence[3];
    cap_state->images[4] = randomSequence[4];
    cap_state->images[5] = randomSequence[5];
    cap_state->images[6] = randomSequence[6];
    cap_state->images[7] = randomSequence[7];
    cap_state->images[8] = randomSequence[8];
    //deallocate the original sequence
    UnloadRandomSequence(randomSequence);

    return memBlock;
}

void image_captcha_draw(void * state, int x, int y) {
    const auto * image_state = (image_captcha_state*)state;
    //draw each image
    for (int i=0;i<9;i++) {
        DrawTexture(images[image_state->images[i]],x+(int)(166.667*(i%3))+3,y+(int)(166.667*(i/3))+3,WHITE); // NOLINT(*-integer-division)
        if (image_state->selected[i]) {//if this image has been selected
            //draw the border around the image
            DrawRectangleLinesEx(Rectangle{(float)x+166.667f*(float)(i%3)+3,(float)y+(166.667f*(float)(i/3))+3,160,160},5,Color{78, 147, 220,255}); // NOLINT(*-integer-division)
        }
    }
}

void image_captcha_mouse_clicked(void * state, int button, int mouseX, int mouseY) {
    auto * image_state = (image_captcha_state*)state;
    if (button == MOUSE_BUTTON_LEFT) {//if the left button was the one pressed
        int slot=0;
        //determine the row that was clicked on
        if (mouseY > 166) {//if not the first row
            if (mouseY > 333) {//if the last row
                slot = 2;
            } else {//if the middle row
                slot = 1;
            }
        }
        slot *= 3;
        //determine the specific image that was clicked on
        if (mouseX > 166) {//if not the first column
            if (mouseX > 333) {//if the last column
                slot += 2;
            } else {//if the middle column
                slot += 1;
            }
        }
        image_state->selected[slot] = !image_state->selected[slot];//toggle this image being selectcted
    }
}

bool image_captcha_check_success(void * state) {
    const auto * image_state = (image_captcha_state*)state;
    if (image_state->lookingFor) {//if looking for type 1
        for (int i=0;i<9;i++) {//check if the section match the not of image values
            if (image_state->selected[i] != image_data[image_state->images[i]]) {
                return false;
            }
        }
    } else {
        for (int i=0;i<9;i++) {//check if the selection is the same as the image values
            if (image_state->selected[i] == image_data[image_state->images[i]]) {
                return false;
            }
        }
    }
    return true;
}

std::string image_captcha_instructions(void * state) {
    const auto * image_state = (image_captcha_state*)state;
    //build the string that explains what wer are looking for
    std::string instructions = "Select all ";
    instructions += (image_state->lookingFor ? "MALE":"FEMALE");
    instructions += " personas";
    return instructions;
}


//this is the function that tells the program what options this file provides. this function is directly called by the main program
std::vector<captchaInfo> example_get_captchas() {
    //this creates an array of the info structs
    return {
    {
        "Check box",//name of this particular captcha
        175,//with of the captcha window
        50,//height of the captcha window
        &create_check_box_captcha,//state structure creation function pointer
        &check_box_draw,//draw function pointer
        &check_box_mouse_clicked,//mouse clicked function pointer
        &check_box_checked,//check for successful completion function pointer
        &check_box_get_instructions,//get the description text function pointer
        },
    {
        "images",
        500,
        500,
        &create_image_captcha,
        &image_captcha_draw,
        &image_captcha_mouse_clicked,
        &image_captcha_check_success,
        &image_captcha_instructions,
        }
    };
}
