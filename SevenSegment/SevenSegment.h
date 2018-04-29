/*
* Display numbers from 0 to 15(0xF) on the SH5461AS
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

#define STATE_OFF false
#define STATE_ON true

class SevenSegment
{
private:
	byte blinking;
	byte states;
	bool blinkingEnabled;
	int counter;
private:
	byte transcode(byte value);
	void displayDigitInternal(byte digit);
	void selectDigit(byte which);
	inline bool shouldBlink();
	inline void debugPrint(byte value, byte testValue);
public:
	SevenSegment();
	void displayDigit(byte digit, byte which = 0);
	void displayNumber(int number);
	void setBlinking(byte which, bool blinking);
	void setTurnedOn(byte which, bool turnedOn);
	void disableBlinking();
	void enableBlinking();
	bool isDigitBlinking(byte which);
	bool isDigitTurnedOn(byte which);
	bool isBlinkingEnabled();

	virtual void onStateChanged(bool newState) {}
};

#endif
