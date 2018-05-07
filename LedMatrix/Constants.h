#ifndef Constants_h
#define Constants_h

#define PIN_IR 3
#define PIN_LEDS 13
#define MATRIX_WIDTH 7
#define MATRIX_HEIGHT 7
#define MATRIX_SERPENTINE_LAYOUT true
#define NUM_PIXELS MATRIX_WIDTH * MATRIX_HEIGHT

#define MAX_DIMENSION ((MATRIX_WIDTH>MATRIX_HEIGHT) ? MATRIX_WIDTH : MATRIX_HEIGHT)

#endif