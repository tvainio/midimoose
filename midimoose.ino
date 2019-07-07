// Maps midi channels 15 to 1 and 16 to 2. Filter out other channels. Filters
// out everything except note on, note off and mod wheel.


#include <MIDI.h>
#include <SoftwareSerial.h>
#include "clock_divider.h" 
#include "clock_output.h"

#define CLOCK_OUT_CC 3
#define PPQ 96

#define OUT1 8
#define OUT2 9
#define OUT3 10
#define OUT4 11
#define OUT5 12
#define OUT6 13

ClockOutput output[6] = {
	ClockOutput(SIXTEEN, OUT1),
	ClockOutput(SIXTEEN, OUT2),
	ClockOutput(SIXTEEN, OUT3),
	ClockOutput(SIXTEEN, OUT4),
	ClockOutput(SIXTEEN, OUT5),
	ClockOutput(SIXTEEN, OUT6),
};


//SoftwareSerial debugSerial(10, 11); // RX, TX


void cb(bool state) {
	digitalWrite(LED_BUILTIN, state);
}

void cbSixteen(bool state) {
	digitalWrite(LED_BUILTIN, state);
}

ClockDivider thirtytwo(PPQ/8, cb);
ClockDivider sixteen(PPQ/4, cbSixteen);
ClockDivider eight(PPQ/2, cb);
ClockDivider fourth(PPQ, cb);
ClockDivider half(PPQ*2, cb);
ClockDivider full(PPQ*4, cb);

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
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  //    debugSerial.println("off");

  if (channel > 14) {
    MIDI.sendNoteOff(pitch, velocity, channel - 14);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
void handleClock(void) {
	sixteen.tick();
}

void setup()
{
	pinMode(OUT1, OUTPUT);
	pinMode(OUT2, OUTPUT);
	pinMode(OUT3, OUTPUT);
	pinMode(OUT4, OUTPUT);
	pinMode(OUT5, OUTPUT);
	pinMode(OUT6, OUTPUT);
	
  pinMode(LED_BUILTIN, OUTPUT);
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
