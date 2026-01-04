#ifndef ROBOT_TESTER_COMMON_STRUCTS_H
#define ROBOT_TESTER_COMMON_STRUCTS_H
#include <string>
#include "include/raylib.h"

struct captchaInfo{
    std::string name;
    int width;
    int height;
    /**Function to create a new context object for this captcha
    @return a pointer to the struct containing all the state information for this captcha
    */
    void * (* create_function)();
    /**Draw the captcha on the screen
    @param state a pointer to the struct containing all the state info for this captcha
    @param x The on screen x coordinate for the left of the captcha window
    @param y The on screen y coordinate for the top of the captcha window
    */
    void (* draw_function)(void * state, int x, int y);
    /**Draw the captcha on the screen
    @param state a pointer to the struct containing all the state info for this captcha
    @param button the mouse button pressed
    @param mouseX the relative in window x position of the mouse
    @param mouseY the relative in window y position of the mouse
    */
    void (* mouse_clicked_function)(void * state, int button, int mouseX, int mouseY);
    /**Draw the captcha on the screen
    @param state a pointer to the struct containing all the state info for this captcha
    @return true if the captcha has been completed successfully
    */
    bool (* check_success_function)(void * state);

    /**Get the current instructions for this captcha
    @return A string containing the instructions to be displayed above the captcha window
    */
    std::string (* get_instructions_function)(void * state);

};


struct button {//simple structure to represent a button
    Rectangle size;//the size and position of the button
    Color color;//the color of the button
    std::string text;//text on the button
};

/**Draw a button to the screen
 * @param buttonIn The button to draw
 */
void drawButton(const button &buttonIn);
/**Check if the mouse is over a button
 * @param buttonIn The button to check with
 * @return true if the mouse is over the provided button
 */
bool checkMouseOverButton(const button &buttonIn);
/**Check if the mouse is over a button
 * @param buttonIn The button to check with
 * @param mousePos The position of the mouse
 * @return true if the provided mouse position is over the button
 */
bool checkMouseOverButton(const button &buttonIn, const Vector2 &mousePos);

#endif //ROBOT_TESTER_COMMON_STRUCTS_H