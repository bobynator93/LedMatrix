#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "IR_Keycodes.h"
#include "LedManager.h"
#include "Constants.h"
#include "ModeBase.h"

class ModeSoundLevel : public ModeBase                      
{
  public:     
    ModeSoundLevel(LedManager& ledMgr);
    ModeSoundLevel();

    virtual void Init();
    virtual void Loop();     
    virtual void KeyInput(uint8_t key);
    
 private:                      

    unsigned long last_millis;
    unsigned int sample;
    int sampleWindow = 50;
    unsigned int peakToPeak = 0;   
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
    uint32_t matrix[NUM_PIXELS];
    int redZone = 4;
    unsigned int max = 1023;
    unsigned int min = 0;
};