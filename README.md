# SevenSegment
Easy to use Arduino library for controlling 7-segment displays.

## Usage:
```c++
#include <SevenSegment.h>

SevenSegment display;
//byte segment = 0;

void setup() {
}

void loop() {
  display.displayNumber(0xBEEF);
  //display.displayDigit(0x9, segment);
  //delay(500);
  //++segment;
  //if (segment > 3)
    //segment = 0;
}
```
