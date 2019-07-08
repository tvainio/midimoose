// Maps midi channels 15 to 1 and 16 to 2. Filter out other channels. Filters
// out everything except note on, note off and mod wheel.

#include <MIDI.h>
#include <SoftwareSerial.h>
#include "clock_divider.h" 
#include "clock_output.h"

#define NUMBER_OF_OUTPUTS 6

#define CLOCK_OUT_CC_FIRST 20
#define PPQ 96

// weird hardware, weird pin mapping...
#define OUT1 9
#define OUT2 10
#define OUT3 8
#define OUT4 13
#define OUT5 12
#define OUT6 11

ClockOutput output[] = {
	ClockOutput(FULL, OUT1),
	ClockOutput(TWO, OUT2),
	ClockOutput(FOUR, OUT3),
	ClockOutput(EIGHT, OUT4),
	ClockOutput(SIXTEEN, OUT5),
	ClockOutput(THIRTYTWO, OUT6),
};

//SoftwareSerial debugSerial(10, 11); // RX, TX

void updateOutputs(uint8_t divider, bool state) {
	for (uint8_t i=0;i<NUMBER_OF_OUTPUTS; i++) {
		output[i].setState(divider, state);
	}
}

void cbThirtytwo(bool state) {
	updateOutputs(THIRTYTWO, state);
}
void cbSixteen(bool state) {
	updateOutputs(SIXTEEN, state);
}

void cbEight(bool state) {
	updateOutputs(EIGHT, state);
}

void cbFour(bool state) {
	updateOutputs(FOUR, state);
}

void cbTwo(bool state) {
	updateOutputs(TWO, state);
}

void cbFull(bool state) {
	updateOutputs(FULL, state);
}

ClockDivider thirtytwo(PPQ/8, cbThirtytwo);
ClockDivider sixteen(PPQ/4, cbSixteen);
ClockDivider eight(PPQ/2, cbEight);
ClockDivider fourth(PPQ, cbFour);
ClockDivider two(PPQ*2, cbTwo);
ClockDivider full(PPQ*4, cbFull);

MIDI_CREATE_DEFAULT_INSTANCE();

uint8_t mapCCToDivider(uint8_t cc) {
	return cc/22; // roughly split the 0-127 range to 0-5
}

void handleControlChange(byte channel, byte number, byte value) {
  if (channel > 14) 
  {
		if(number == 0x01) { // Only send mod wheel to acid moose
			MIDI.sendControlChange(number, value, channel - 14);
		}
		// CC 20-26 control midi moose clock outputs
    else if (number == CLOCK_OUT_CC_FIRST) {
			output[0].setDivider(mapCCToDivider(value));
		}
    else if (number == CLOCK_OUT_CC_FIRST+1) {
			output[1].setDivider(mapCCToDivider(value));
		}
    else if (number == CLOCK_OUT_CC_FIRST+2) {
			output[2].setDivider(mapCCToDivider(value));
		}
    else if (number == CLOCK_OUT_CC_FIRST+3) {
			output[3].setDivider(mapCCToDivider(value));
		}
    else if (number == CLOCK_OUT_CC_FIRST+4) {
			output[4].setDivider(mapCCToDivider(value));
		}
    else if (number == CLOCK_OUT_CC_FIRST+5) {
			output[5].setDivider(mapCCToDivider(value));
		}
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (channel > 14)
  {
    MIDI.sendNoteOn(pitch, velocity, channel - 14);
    //         debugSerial.print(channel, HEX);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  //    debugSerial.println("off");

  if (channel > 14) {
    MIDI.sendNoteOff(pitch, velocity, channel - 14);
  }
}

void handleClock(void) {
 thirtytwo.tick();
 sixteen.tick();
 eight.tick();
 fourth.tick();
 two.tick();
 full.tick();
}

void setup()
{
  //    debugSerial.begin(115200);
  //    debugSerial.println("Hello, world?");

  MIDI.setHandleClock(handleClock);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  MIDI.turnThruOff();

}

void loop()
{
  MIDI.read();
}
