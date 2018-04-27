# SevenSegment
Simple to use Arduino library for the SH5461AS 7-segment display.

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
