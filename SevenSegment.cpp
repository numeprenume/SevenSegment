#include "Arduino.h"
#include "SevenSegment.h"

//#define DEBUG

#ifndef DEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif

#ifndef COMMON_ANODE
#define COMMON_ANODE 0
#else
#define COMMON_ANODE 1
#endif
#define COMMON_CATHODE !COMMON_ANODE

#if COMMON_CATHODE
#define ENABLE 1
#else
#define ENABLE 0
#endif

#define DISABLE !ENABLE

#define MASK 0b00000010
#ifndef FREQ
#define FREQ 200 //frequency in Hz, 200 default
#endif

#if DEBUG
#define DELAY 100
#else
#define DELAY (1000/FREQ)
#endif

#define BLINKING_DELAY 250 //TODO: make this customizable

enum Pins : byte
{
	PIN_A = 2, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G,
	PIN_D1, PIN_D2, PIN_D3, PIN_D4
};

SevenSegment::SevenSegment()
{
	blinking = 0;
	states = 0xFF;
	counter = 0;
	blinkingEnabled = true;

	for (byte i = Pins::PIN_A; i <= Pins::PIN_D4; i++)
		pinMode(i, OUTPUT);

	if (DEBUG)
	{
		Serial.begin(9600);
		Serial.println("DELAY: ");
		Serial.print(DELAY, DEC);
		Serial.println("\n");
	}
}

void SevenSegment::displayDigit(byte digit, byte which = 0)
{
	if (isDigitBlinking(which) && shouldBlink())
	{
		setTurnedOn(which, !isDigitTurnedOn(which));
		return; //eat this one so we don't get weird flashes of the previous digit
	}
	if (!isDigitTurnedOn(which))
	{
		which = -1; //select none
	}

	selectDigit(which);
	displayDigitInternal(digit);
	delay(DELAY);
}

void SevenSegment::displayDigitInternal(byte digit)
{
	byte out = transcode(digit);

	digitalWrite(Pins::PIN_A, ((out & MASK) != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_B, ((out & (MASK << 1)) != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_C, ((out & (MASK << 2)) != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_D, ((out & (MASK << 3)) != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_E, ((out & (MASK << 4)) != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_F, ((out & (MASK << 5)) != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_G, ((out & (MASK << 6)) != 0) ? ENABLE : DISABLE);
}

void SevenSegment::displayNumber(int number)
{
	if (isBlinkingEnabled())
		counter += 4 * DELAY;

	displayDigit(number & 0x000F, 0);
	displayDigit((number & 0x00F0) >> 4, 1);
	displayDigit((number & 0x0F00) >> 8, 2);
	displayDigit((number & 0xF000) >> 12, 3);

	if (shouldBlink())
		counter = 0;
}

void SevenSegment::setBlinking(byte which, bool blinking)
{
	if (blinking == isDigitBlinking(which))
		return;
	if (!isBlinkingEnabled())
		return;
	this->blinking = blinking
		? this->blinking | 1 << which
		: this->blinking & ~(1 << which);
	setTurnedOn(which, true);
}

bool SevenSegment::isDigitBlinking(byte which)
{
	if (!isBlinkingEnabled())
		return false;
	return this->blinking & (1 << which);
}

void SevenSegment::setTurnedOn(byte which, bool turnedOn)
{
	if (turnedOn == isDigitTurnedOn(which))
		return;
	this->states = turnedOn
		? this->states | 1 << which
		: this->states & ~(1 << which);
	onStateChanged(turnedOn);
}
void SevenSegment::disableBlinking()
{
	blinkingEnabled = false;
}
void SevenSegment::enableBlinking()
{
	blinkingEnabled = true;
}
bool SevenSegment::isDigitTurnedOn(byte which)
{
	return this->states & (1 << which);
}

bool SevenSegment::isBlinkingEnabled()
{
	return blinkingEnabled;
}

void SevenSegment::selectDigit(byte which)
{
	digitalWrite(Pins::PIN_D1, (which != 0) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_D2, (which != 1) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_D3, (which != 2) ? ENABLE : DISABLE);
	digitalWrite(Pins::PIN_D4, (which != 3) ? ENABLE : DISABLE);
}

byte SevenSegment::transcode(byte value)
{
	byte result;

	switch (value)
	{
	case 0x0:
		result = 0b01111110;
		break;
	case 0x1:
		result = 0b00001100;
		break;
	case 0x2:
		result = 0b10110110;
		break;
	case 0x3:
		result = 0b10011110;
		break;
	case 0x4:
		result = 0b11001100;
		break;
	case 0x5:
		result = 0b11011010;
		break;
	case 0x6:
		result = 0b11111010;
		break;
	case 0x7:
		result = 0b00001110;
		break;
	case 0x8:
		result = 0b11111110;
		break;
	case 0x9:
		result = 0b11011110;
		break;
	case 0xA:
		result = 0b11101110;
		break;
	case 0xB:
		result = 0b11111000;
		break;
	case 0xC:
		result = 0b01110010;
		break;
	case 0xD:
		result = 0b10111100;
		break;
	case 0xE:
		result = 0b11110010;
		break;
	case 0xF:
		result = 0b11100010;
		break;
	default:
		result = 0;
	}
	if (COMMON_ANODE)
		result = ~result;

	debugPrint(value, result);
	return result;
}

bool SevenSegment::shouldBlink()
{
	return (counter > BLINKING_DELAY) && isBlinkingEnabled();
}

void SevenSegment::debugPrint(byte value, byte testValue)
{
	if (DEBUG)
	{
		Serial.print(value, HEX);
		Serial.print('\t');
		Serial.print(testValue, BIN);
		Serial.print('\n');
	}
}

