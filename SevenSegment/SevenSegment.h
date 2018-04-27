/*
* Display numbers from 0 to 15(0xA) on the SH5461AS
* A->G: pin 2->pin 8
* D1->D4: pin 9->pin 12
*     A
*    ----
* F |    | B
*   | G  |
*    ----
* E |    | C
*   |    |
*    ----
*     D
*/


#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

class SevenSegment
{
  private:
    byte transcode(byte value);
	void displayDigitInternal(byte digit);
	void selectSegment(byte which);
    inline void debugPrint(byte value, byte testValue);
  public:
    SevenSegment();
    void displayDigit(byte digit, byte segment = 0);
    void displayNumber(long number);
};

#endif
