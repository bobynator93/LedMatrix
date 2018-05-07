#include "ModeSnake.h"

ModeSnake::ModeSnake(LedManager& ledMgr)
: ModeBase(ledMgr)
{
    control = TYPE_B;
    bodyColor = 0x0000ff;
    headColor = 0xff0000;
}

ModeSnake::ModeSnake()
: ModeBase()
{}

void ModeSnake::Init()
{
    direction[0] = 1;
    direction[1] = 0;

    dots = 1; // 0 index
    body_x[0] = 3;
    body_y[0] = 3;
    body_x[1] = 2;
    body_y[1] = 3;

    inGame = true;
    locateApple();
}

void ModeSnake::Loop()
{
    unsigned long current_millis = millis();
    if((current_millis - last_millis) > 500 && inGame){
        last_millis = current_millis;

        move();
        checkCollision();
        checkApple();
        
        updateGraphics();
    }
}

void ModeSnake::KeyInput(uint8_t key)
{
    // Change coontrols
    if (key == IR_KEYCODE_1){
        control = TYPE_A;
        bodyColor = 0xFFA500;
        headColor = 0xff0000;}
    
    if (key == IR_KEYCODE_2){
        control = TYPE_B;
        bodyColor = 0x0000ff;
        headColor = 0xff0000;}

    // TYPE_A controlling
    if (control == TYPE_A){
        if (key == IR_KEYCODE_UP){
            direction[0] = 0; direction[1] = 1;}
        if (key == IR_KEYCODE_RIGHT){
            direction[0] = 1; direction[1] = 0;}
        if (key == IR_KEYCODE_DOWN){
            direction[0] = 0; direction[1] = -1;}
        if (key == IR_KEYCODE_LEFT){
            direction[0] = -1; direction[1] = 0;}
    }

    // TYPE_B controlling
    if (control == TYPE_B){
        if (key == IR_KEYCODE_RIGHT){

            if(direction[0] == 0){
                direction[0] = direction[1];
                direction[1] = 0;
            }else if(direction[1] == 0){
                direction[1] = -direction[0];
                direction[0] = 0;}
            
        }else if (key == IR_KEYCODE_LEFT){
            
            if(direction[0] == 0){
                direction[0] = -direction[1];
                direction[1] = 0;
            }else if(direction[1] == 0){
                direction[1] = direction[0];
                direction[0] = 0;}
        }
    }
}

void ModeSnake::updateGraphics()
{
    // clean all
    led.Clear();

    // draw body
    for (uint8_t z = dots ; z > 0; z--) {
        led.SetPixelColor(body_x[z], body_y[z], bodyColor);
    }
    led.SetPixelColor(body_x[0], body_y[0], headColor);

    // draw apple
    led.SetPixelColor(apple_x, apple_y, 0x00ff00);

    led.Show();
}

void ModeSnake::move()
{
    // move dot position to previous one
    for (uint8_t z = dots; z > 0; z--) {
        body_x[z] = body_x[(z - 1)];
        body_y[z] = body_y[(z - 1)];
    }

    // determine new head position
    int newx = body_x[0] + direction[0];
    int newy = body_y[0] + direction[1];
    
    // run throu walls
    if (newx > (MATRIX_WIDTH - 1))
        newx = 0;
    if (newy > (MATRIX_HEIGHT - 1))
        newy = 0;
    if (newx < 0)
        newx = MATRIX_WIDTH - 1;
    if (newy < 0)
        newy = MATRIX_HEIGHT - 1;

    // set new head position
    body_x[0] = newx;
    body_y[0] = newy;
}

void ModeSnake::checkCollision()
{
    for (int z = dots; z > 0; z--) {

        if ((body_x[0] == body_x[z]) && (body_y[0] == body_y[z])) {
            inGame = false;
            Init();
            break;
        }
    }
}

void ModeSnake::locateApple() 
{
    apple_x = random(MATRIX_WIDTH - 1);
    apple_y = random(MATRIX_HEIGHT - 1);
    for (int z = dots; z >= 0; z--) {
        if ((body_x[z] == apple_x) && (body_y[z] == apple_y)) {
            locateApple();
            break;
        }
    }   
}

void ModeSnake::checkApple() 
{
    if ((body_x[0] == apple_x) && (body_y[0] == apple_y)) 
    {
        dots++;
        body_x[dots] = body_x[dots-1];
        body_y[dots] = body_y[dots-1];
        locateApple();
    }
}