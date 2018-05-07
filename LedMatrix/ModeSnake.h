#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "IR_Keycodes.h"
#include "LedManager.h"
#include "Constants.h"
#include "ModeBase.h"

enum  controls { TYPE_A, TYPE_B };

class ModeSnake : public ModeBase                      
{
  public:     
    ModeSnake(LedManager& ledMgr);
    ModeSnake();

    virtual void Init();
    virtual void Loop();     
    virtual void KeyInput(uint8_t key);
    
 private:                      
    uint8_t body_x[NUM_PIXELS];
    uint8_t body_y[NUM_PIXELS];
    uint8_t dots;
    uint8_t apple_x;
    uint8_t apple_y;

    uint32_t bodyColor;
    uint32_t headColor;

    int direction[2];
    controls control;

    bool inGame;

    unsigned long last_millis;

    void move();
    void checkCollision();
    void locateApple();
    void checkApple();
    void updateGraphics();
};