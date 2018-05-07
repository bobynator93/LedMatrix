#include "ModeSoundLevel.h"

ModeSoundLevel::ModeSoundLevel(LedManager& ledMgr)
: ModeBase(ledMgr)
{

}

ModeSoundLevel::ModeSoundLevel()
: ModeBase()
{}

void ModeSoundLevel::Init()
{

}

void ModeSoundLevel::Loop()
{
    unsigned long current_millis = millis();
 
    sample = analogRead(0); 
    if (sample < 1024)  // toss out spurious readings
    {
        if (sample > signalMax)
        {
            signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
            signalMin = sample;  // save just the min levels
        }
    }
    
    if(current_millis - last_millis > sampleWindow){
        last_millis = current_millis;
        
        peakToPeak = signalMax - signalMin;
        // Print the protocol data
        Serial.println(peakToPeak);
        
        // map 1v p-p level to the max scale of the display
        int displayPeak = map(peakToPeak, min, max, 0, MATRIX_HEIGHT);
        
        // Update the display:
        for (int x = 1; x < MATRIX_WIDTH; x++)  // shift the display left
        {
            for (int y = 0; y < MATRIX_HEIGHT; y++)  // shift the display left
            {
                matrix[led.XY(x,y)] = matrix[led.XY(x-1,y)];
            }
        }
        
        // draw the new sample
        for (int i = 0; i <= MATRIX_HEIGHT; i++)
        {
            if (i >= displayPeak)  // blank these pixels
            {
                matrix[led.XY(0,i)] = 0x000000;
            }
            else if (i < redZone) // draw in green
            {
                matrix[led.XY(0,i)] = 0x00ff00;
            }
            else // Red Alert!  Red Alert!
            {
                matrix[led.XY(0,i)] = 0xff0000;
            }
        }

        led.SetMatrix(matrix);
        led.Show();

        peakToPeak = 0;   
        signalMax = 0;
        signalMin = 1024;
    }
}

void ModeSoundLevel::KeyInput(uint8_t key)
{
    if (key == IR_KEYCODE_UP){
        max += 100;
        if(max > 1023)
            max = 1023;
    }  
    if (key == IR_KEYCODE_DOWN){
        max -= 100;
        if(max < 0)
            max = 0;
    } 
    if (key == IR_KEYCODE_RIGHT){
        min += 100;
        if(min > 1023)
            min = 1023;
    } 
    if (key == IR_KEYCODE_LEFT){
        min -= 100;
        if(min < 0)
            min = 0;
    } 
    if (key == IR_KEYCODE_1){
        min = peakToPeak;
    }  
    if (key == IR_KEYCODE_2){
        max = peakToPeak;
    } 
}

