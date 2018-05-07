#ifndef LedManager_h
#define LedManager_h


#include <FastLED.h>
#include "Constants.h"
#include "IRLremote.h"

class LedManager                      
{
  public:
    LedManager();              
    
    void Setup(CNec &remote);
    void Clear();   
    void SetMatrix(uint32_t colors[]);
    void SetPixelColor(uint8_t pixel, uint32_t color);
    void SetPixelColor(uint8_t x, uint8_t y, uint32_t color);
    void SetPixelColor(uint8_t pixel, CRGB color);
    void Show();

    void Sinelon();
    uint8_t gHue = 0;
    uint16_t XY( uint8_t x, uint8_t y);
 private:                      
    CRGB leds[NUM_PIXELS];
    void getCoordinate(uint8_t pixel, int& x, int& y);
    void getPixel(uint8_t x, uint8_t y, uint8_t& pixel);

    CNec IRLremote;
    
};

#endif
