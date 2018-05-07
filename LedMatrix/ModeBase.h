#ifndef ModeBase_h
#define ModeBase_h
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "LedManager.h"

class ModeBase
{
    public:
        ModeBase(LedManager& ledMgr){
            led = ledMgr;
        }
        ModeBase()
        {}
        virtual void Init() {};
        virtual void Loop() {Serial.println(F("test"));}; 
        virtual void KeyInput(uint8_t key) {Serial.println(F("shit2"));};

    protected:
        LedManager led;
};

#endif