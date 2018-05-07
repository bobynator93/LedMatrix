#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "IR_Keycodes.h"
#include "LedManager.h"
#include "Constants.h"
#include "ModeBase.h"
#include <FastLED.h>

enum  AnimationTypes { RandomNoise, Noise, Sinelon};

class ModeAnimations : public ModeBase                      
{
  public:     
    ModeAnimations(LedManager& ledMgr);
    ModeAnimations();

    virtual void Init();
    virtual void Loop();     
    virtual void KeyInput(uint8_t key);
    
 private:                      
    unsigned long last_millis;

    uint16_t x;
    uint16_t y;
    uint16_t z;

    uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
    uint8_t colorLoop = 1;

    void ChangePaletteAndSettingsPeriodically();
    void fillNoise();
    void mapNoiseToLEDsUsingPalette();
    void SetupRandomPalette();
    void SetupBlackAndWhiteStripedPalette();
    void SetupPurpleAndGreenPalette();
    uint16_t XY( uint8_t x, uint8_t y);
    

    bool randomMode = true;
    AnimationTypes animationType = RandomNoise;

    uint16_t speed = 1; // speed is set dynamically once we've started up
    uint16_t scale = 30; // scale is set dynamically once we've started up
};