// Maps midi channels 15 to 1 and 16 to 2. Filter out other channels. Filters
// out everything except note on, note off and mod wheel.


#include <MIDI.h>
#include <SoftwareSerial.h>
#include "clock_divider.h" 
#include "clock_output.h"

#define NUMBER_OF_OUTPUTS 7

#define CLOCK_OUT_CC 3
#define PPQ 96

#define OUT1 8
#define OUT2 9
#define OUT3 10
#define OUT4 11
#define OUT5 12
#define OUT6 13

ClockOutput output[] = {
	ClockOutput(SIXTEEN, LED_BUILTIN),
	ClockOutput(SIXTEEN, OUT1),
	ClockOutput(SIXTEEN, OUT2),
	ClockOutput(SIXTEEN, OUT3),
	ClockOutput(SIXTEEN, OUT4),
	ClockOutput(SIXTEEN, OUT5),
	ClockOutput(SIXTEEN, OUT6),

};


//SoftwareSerial debugSerial(10, 11); // RX, TX


void nop(bool state) {
}

void updateOutputs(uint8_t divider, bool state) {
	for (uint8_t i=0;i<NUMBER_OF_OUTPUTS; i++) {
		output[i].set(SIXTEEN, state);
	}
}

void cbSixteen(bool state) {
	updateOutputs(SIXTEEN, state);
}

ClockDivider thirtytwo(PPQ/8, nop);
ClockDivider sixteen(PPQ/4, cbSixteen);
ClockDivider eight(PPQ/2, nop);
ClockDivider fourth(PPQ, nop);
ClockDivider half(PPQ*2, nop);
ClockDivider full(PPQ*4, nop);

byte counter = 0;
int clockOutDivision = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

void handleCC(byte channel, byte number, byte value) {
  if (channel > 14 && number == 0x01) // Only send mod wheel
  {
    MIDI.sendControlChange(number, value, channel - 14);
  }
}
/*
void handleControlChange(byte channel, byte number, byte value)
{
  if (channel > 14)
  {
    if (number == CLOCK_OUT_CC)
     // change clock out division
  }
  
}
*/
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (channel > 14)
  {
    MIDI.sendNoteOn(pitch, velocity, channel - 14);
    //         debugSerial.print(channel, HEX);
//    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  //    debugSerial.println("off");

  if (channel > 14) {
    MIDI.sendNoteOff(pitch, velocity, channel - 14);
//    digitalWrite(LED_BUILTIN, LOW);
  }
}
void handleClock(void) {
	sixteen.tick();

}

void setup()
{
	
  //    debugSerial.begin(115200);
  //    debugSerial.println("Hello, world?");

  MIDI.setHandleClock(handleClock);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange (handleCC);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  MIDI.turnThruOff();

}

void loop()
{
  MIDI.read();
}
