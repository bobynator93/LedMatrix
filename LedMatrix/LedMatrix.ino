#include "IRLremote.h"
#include "IR_Keycodes.h"
#include "LedManager.h"
#include "ModeSnake.h"
#include "ModeAnimations.h"
#include "ModeSoundLevel.h"
#include "Constants.h"



void *operator new( size_t size, void *ptr ){return ptr;}

static uint8_t objStorageSnake[ sizeof(ModeSnake) ]; // RAM for the object
static uint8_t objStorageAnimations[ sizeof(ModeAnimations) ];
static uint8_t objStorageSoundLevel[ sizeof(ModeSoundLevel) ];

//enum  pattern: uint8_t { ANIMATIONS =0, SNAKE=1, SOUNDLEVEL=2};
enum  pattern: uint8_t { ANIMATIONS =0, SNAKE=1, NUM_OF_PATTERNS=2};


LedManager led = LedManager();//(PIN_LEDS, NUM_PIXELS);
CNec IRLremote;

ModeBase * currentMode;
uint8_t activeMode = ANIMATIONS;
uint8_t numOfPatterns = NUM_OF_PATTERNS - 1;

void setup() {
  while (!Serial);
  Serial.begin(115200);
  Serial.println(F("Startup LED Matrix"));

  if (!IRLremote.begin(PIN_IR))
    Serial.println(F("No valid pin for infrared!"));

  led.Setup(IRLremote);

  //currentMode = new (objStorageSnake) ModeSnake(led);
  //currentMode = new (objStorageAnimations) ModeAnimations(led);
  switchMode();

  //currentMode->Init();

}

void loop() {
  // Check if new IR protocol data is available
  if (IRLremote.available())
  {
    // Get the new data from the remote
    auto data = IRLremote.read();

    // Print the protocol data
    Serial.print(F("Command: 0x"));
    Serial.println(data.command, HEX);

    if(data.command == IR_KEYCODE_STAR){
      activeMode--;
      if(activeMode<0){activeMode=0;}else{switchMode();}
      
    }else if(data.command == IR_KEYCODE_HASH){
       activeMode++;
      if(activeMode>numOfPatterns){activeMode=numOfPatterns;}else{switchMode();}

    }else{
      currentMode->KeyInput(data.command);
    }
  }

    currentMode->Loop();
}


void switchMode(){
  currentMode = NULL;
  switch(activeMode)
  {
    case ANIMATIONS:
      currentMode = new (objStorageAnimations) ModeAnimations(led);
      break;
    case SNAKE:
      currentMode = new (objStorageSnake) ModeSnake(led);
      break;
    case SOUNDLEVEL:
      currentMode = new (objStorageSoundLevel) ModeSoundLevel(led);
      break;
  }
  currentMode->Init();
}
