#include "ModeAnimations.h"

CRGBPalette16 currentPalette( PartyColors_p );

ModeAnimations::ModeAnimations(LedManager& ledMgr)
: ModeBase(ledMgr)
{

}

ModeAnimations::ModeAnimations()
: ModeBase()
{}

void ModeAnimations::Init()
{
    x = random16();
    y = random16();
    z = random16();
}

void ModeAnimations::Loop()
{
    unsigned long current_millis = millis();
    if((current_millis - last_millis) > 10){
        last_millis = current_millis;

        switch(animationType){
          case RandomNoise:
            ChangePaletteAndSettingsPeriodically();

            // generate noise data
            fillNoise();

            // convert the noise data to colors in the LED array
            // using the current palette
            mapNoiseToLEDsUsingPalette();
            break;
          case Noise:
            // generate noise data
            fillNoise();

            // convert the noise data to colors in the LED array
            // using the current palette
            mapNoiseToLEDsUsingPalette();
            break;
          case Sinelon:
            led.Sinelon();
            break;
        }
        // // Periodically choose a new palette, speed, and scale
        // if(randomMode == true)
        //   ChangePaletteAndSettingsPeriodically();

        // // generate noise data
        // fillNoise();
        
        // // convert the noise data to colors in the LED array
        // // using the current palette
        // mapNoiseToLEDsUsingPalette();

        led.Show();
    }
    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { led.gHue++; } // slowly cycle the "base color" through the rainbow
}

void ModeAnimations::KeyInput(uint8_t key)
{
  switch (key)  
  {  
      case IR_KEYCODE_0:  
        animationType = RandomNoise;
        break;  
      case IR_KEYCODE_1:  
        animationType = Noise;
        currentPalette = RainbowColors_p;         speed = 20; scale = 30; colorLoop = 1;
        break;
      case IR_KEYCODE_2:  
        animationType = Noise;
        SetupPurpleAndGreenPalette();             speed = 10; scale = 50; colorLoop = 1;
        break;  
      case IR_KEYCODE_3:  
        animationType = Noise;
        SetupBlackAndWhiteStripedPalette();       speed = 10; scale = 30; colorLoop = 1;
        break;  
      case IR_KEYCODE_4:  
        animationType = Noise;
        currentPalette = ForestColors_p;          speed =  8; scale =120; colorLoop = 0;
        break;  
      case IR_KEYCODE_5:  
        animationType = Noise;
        currentPalette = CloudColors_p;           speed =  8; scale = 30; colorLoop = 0;
        break;  
      case IR_KEYCODE_6:  
        animationType = Noise;
        currentPalette = LavaColors_p;            speed =  8; scale = 50; colorLoop = 0;
        break;  
      case IR_KEYCODE_7:  
        animationType = Sinelon;
        break; 
      case IR_KEYCODE_8:  
        animationType = Noise;
        currentPalette = PartyColors_p;           speed = 20; scale = 30; colorLoop = 1;
        break;  
      case IR_KEYCODE_9:  
        animationType = Noise;
        SetupRandomPalette();                     speed = 15; scale = 50; colorLoop = 1;
        break;  
  } 
}

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void ModeAnimations::fillNoise() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      
      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      
      noise[i][j] = data;
    }
  }
  
  //z += speed;
  
  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;
}

void ModeAnimations::mapNoiseToLEDsUsingPalette(){
  static uint8_t ihue=0;
  
  for(int i = 0; i < MATRIX_WIDTH; i++) {
    for(int j = 0; j < MATRIX_HEIGHT; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) { 
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the 
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      //leds[XY(i,j)] = color;
      led.SetPixelColor(XY(i,j), color);

    }
  }
  
  ihue+=1;
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.

// 1 = 5 sec per palette
// 2 = 10 sec per palette
// etc
#define HOLD_PALETTES_X_TIMES_AS_LONG 2

void ModeAnimations::ChangePaletteAndSettingsPeriodically()
{
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         speed = 20; scale = 30; colorLoop = 1; }
    if( secondHand ==  5)  { SetupPurpleAndGreenPalette();             speed = 10; scale = 50; colorLoop = 1; }
    if( secondHand == 10)  { SetupBlackAndWhiteStripedPalette();       speed = 10; scale = 30; colorLoop = 1; }
    if( secondHand == 15)  { currentPalette = ForestColors_p;          speed =  8; scale =120; colorLoop = 0; }
    if( secondHand == 20)  { currentPalette = CloudColors_p;           speed =  8; scale = 30; colorLoop = 0; }
    if( secondHand == 25)  { currentPalette = LavaColors_p;            speed =  8; scale = 50; colorLoop = 0; }
    if( secondHand == 30)  { currentPalette = HeatColors_p ;           speed = 20; scale = 90; colorLoop = 0; }
    if( secondHand == 35)  { currentPalette = PartyColors_p;           speed = 20; scale = 30; colorLoop = 1; }
    if( secondHand == 40)  { SetupRandomPalette();                     speed = 5; scale = 20; colorLoop = 1; }
    if( secondHand == 45)  { SetupRandomPalette();                     speed = 15; scale = 50; colorLoop = 1; }
    if( secondHand == 50)  { SetupRandomPalette();                     speed = 30; scale = 90; colorLoop = 1; }
    if( secondHand == 55)  { currentPalette = RainbowStripeColors_p;   speed = 30; scale = 20; colorLoop = 1; }
  }
}


  // This function generates a random palette that's a gradient
  // between four different colors.  The first is a dim hue, the second is 
  // a bright hue, the third is a bright pastel, and the last is 
  // another bright hue.  This gives some visual bright/dark variation
  // which is more interesting than just a gradient of different hues.
  void ModeAnimations::SetupRandomPalette()
  {
    currentPalette = CRGBPalette16( 
                        CHSV( random8(), 255, 32), 
                        CHSV( random8(), 255, 255), 
                        CHSV( random8(), 128, 255), 
                        CHSV( random8(), 255, 255)); 
  }

  // This function sets up a palette of black and white stripes,
  // using code.  Since the palette is effectively an array of
  // sixteen CRGB colors, the various fill_* functions can be used
  // to set them up.
  void ModeAnimations::SetupBlackAndWhiteStripedPalette()
  {
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

  }

  // This function sets up a palette of purple and green stripes.
  void ModeAnimations::SetupPurpleAndGreenPalette()
  {
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16( 
      green,  green,  black,  black,
      purple, purple, black,  black,
      green,  green,  black,  black,
      purple, purple, black,  black );
  }


  //
  // Mark's xy coordinate mapping code.  See the XYMatrix for more information on it.
  //
  uint16_t ModeAnimations::XY( uint8_t x, uint8_t y)
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



