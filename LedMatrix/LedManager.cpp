#include "LedManager.h"

uint32_t creeper[] = {0x002521, 0x132b14, 0x3f3f3f, 0x223012, 0x132b14, 0x132b14, 0x223012,
                        0x132b14, 0x000000, 0x000000, 0x132b14, 0x000000, 0x000000, 0x002521,
                        0x223012, 0x000000, 0x000000, 0x132b14, 0x000000, 0x000000, 0x132b14,
                        0x3f3f3f, 0x132b14, 0x223012, 0x000000, 0x132b14, 0x223012, 0x002521,
                        0x223012, 0x132b14, 0x000000, 0x000000, 0x000000, 0x132b14, 0x132b14,
                        0x132b14, 0x223012, 0x000000, 0x002521, 0x000000, 0x132b14, 0x002521,
                        0x132b14, 0x002521, 0x223012, 0x132b14, 0x3f3f3f, 0x223012, 0x132b14};
                        
uint32_t heart[] = {0x000000,0x3f0000,0x3f0000,0x000000,0x3f0000,0x3f0000,0x000000,
                      0x3f0000,0x3f0c12,0x3f0c12,0x3f0000,0x3e191d,0x3e191d,0x3f0000,
                      0x3f0000,0x3f0c12,0x3f0c12,0x3f0c12,0x3f0c12,0x3e191d,0x3f0000,
                      0x3f0000,0x3f0c12,0x3f0c12,0x3f0c12,0x3f0c12,0x3e191d,0x3f0000,
                      0x000000,0x3f0000,0x3f0c12,0x3f0c12,0x3e191d,0x3f0000,0x000000,
                      0x000000,0x000000,0x3f0000,0x3e191d,0x3f0000,0x000000,0x000000,
                      0x000000,0x000000,0x000000,0x3f0000,0x000000,0x000000,0x000000};

uint32_t pikachu[] = {0x000000,0x000000,0x3f3f3f,0x3f3f3f,0x3f3f3f,0x000000,0x000000,
                      0x3f3a0e,0x3f3a0e,0x3f3f3f,0x3f3f3f,0x3f3f3f,0x3f3a0e,0x3f3a0e,
                      0x3f3a0e,0x3f3a0e,0x3f3a0e,0x3f3a0e,0x3f3a0e,0x3f3a0e,0x3f3a0e,
                      0x3f3a0e,0x000000,0x000000,0x3f3a0e,0x000000,0x000000,0x3f3a0e,
                      0x3f3a0e,0x3f3f3f,0x000000,0x3f3a0e,0x3f3f3f,0x000000,0x3f3a0e,
                      0x2d040b,0x3f3a0e,0x3f3a0e,0x000000,0x3f3a0e,0x3f3a0e,0x2d040b,
                      0x2d040b,0x3f3a0e,0x000000,0x000000,0x000000,0x3f3a0e,0x2d040b};

LedManager::LedManager()
{
    FastLED.addLeds<PL9823, PIN_LEDS, RGB>(leds, NUM_PIXELS);
}

void LedManager::Setup(CNec &remote)
{
    IRLremote = remote;
    Clear();
}

void LedManager::Clear() {
  for( int i = 0; i<NUM_PIXELS; i++){
    leds[i] = 0x000000;
  }
}

void LedManager::SetMatrix(uint32_t colors[]) {
  for( int i = 0; i<NUM_PIXELS; i++){
    leds[i] = colors[i];
  }
}

void LedManager::Show()
{
    if (!IRLremote.receiving()) {
        FastLED.show();
        }
}

void LedManager::SetPixelColor(uint8_t pixel, uint32_t color)
{
    leds[pixel] = color;
}

void LedManager::SetPixelColor(uint8_t pixel, CRGB color)
{
    leds[pixel] = color;
}

void LedManager::SetPixelColor(uint8_t x, uint8_t y, uint32_t color)
{
    uint8_t pixel = 0;
    getPixel(x,y, pixel);
    SetPixelColor(pixel, color);
}

void LedManager::getCoordinate(uint8_t pixel, int& x, int& y)
{
    int xValue[] = {1, 2, 3, 4, 5, 6, 7,
                        7, 6, 5, 4, 3, 2, 1,
                        1, 2, 3, 4, 5, 6, 7,
                        7, 6, 5, 4, 3, 2, 1,
                        1, 2, 3, 4, 5, 6, 7,
                        7, 6, 5, 4, 3, 2, 1,
                        1, 2, 3, 4, 5, 6, 7};

    int yValue[] = {7, 7, 7, 7, 7, 7, 7,
                        6, 6, 6, 6, 6, 6, 6,
                        5, 5, 5, 5, 5, 5, 5,
                        4, 4, 4, 4, 4, 4, 4,
                        3, 3, 3, 3, 3, 3, 3,
                        2, 2, 2, 2, 2, 2, 2,
                        1, 1, 1, 1, 1, 1, 1};

    x = xValue[pixel];
    y = yValue[pixel];
}

void LedManager::getPixel(uint8_t x, uint8_t y, uint8_t& pixel)
{

    uint8_t pixelValue[7][7] = {  
        {43, 42, 29, 28, 15, 14, 1},   
        {44, 41, 30, 27, 16, 13, 2},   
        {45, 40, 31, 26, 17, 12, 3},
        {46, 39, 32, 25, 18, 11, 4},
        {47, 38, 33, 24, 19, 10, 5},
        {48, 37, 34, 23, 20, 9, 6}, 
        {49, 36, 35, 22, 21, 8, 7},  
    };

    pixel = pixelValue[x][y] - 1;
}

  void LedManager::Sinelon()
  {
    // a colored dot sweeping back and forth, with fading trails
    // fadeToBlackBy( leds, NUM_PIXELS, 20);
    // int pos = random(NUM_PIXELS -1);//beatsin16(13,0,NUM_PIXELS);

    fadeToBlackBy( leds, NUM_PIXELS, 1);
    
    EVERY_N_MILLISECONDS( 80 ) { 
    int pos = random16(NUM_PIXELS);  
    leds[pos] += CHSV( gHue, 255, 192);}

    
  }

    //
  // Mark's xy coordinate mapping code.  See the XYMatrix for more information on it.
  //
  uint16_t LedManager::XY( uint8_t x, uint8_t y)
  {
    uint16_t i;
    if( MATRIX_SERPENTINE_LAYOUT == false) {
      i = (y * MATRIX_WIDTH) + x;
    }
    if( MATRIX_SERPENTINE_LAYOUT == true) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (MATRIX_WIDTH - 1) - x;
        i = (y * MATRIX_WIDTH) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * MATRIX_WIDTH) + x;
      }
    }
    return i;
  }