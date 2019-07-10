// Maps midi channels 15 to 1 and 16 to 2. Filter out other channels. Filters
// out everything except note on, note off and mod wheel.

#include <EEPROM.h>
#include <MIDI.h>
#include "clock_divider.h" 
#include "clock_output.h"

#define NUMBER_OF_OUTPUTS 6

#define CLOCK_OUT_CC_FIRST 20
#define PPQ 96

// weird hardware, weird pin mapping...
#define OUT1_PIN 9
#define OUT2_PIN 10
#define OUT3_PIN 8
#define OUT4_PIN 13
#define OUT5_PIN 12
#define OUT6_PIN 11

uint8_t readDividerFromEEPROM(uint8_t output) {
  uint8_t value = EEPROM.read(output);
  if (value > THIRTYTWO) return SIXTEEN;
}

ClockOutput output[] = {
  ClockOutput(readDividerFromEEPROM(0), OUT1_PIN),
  ClockOutput(readDividerFromEEPROM(1), OUT2_PIN),
  ClockOutput(readDividerFromEEPROM(2), OUT3_PIN),
  ClockOutput(readDividerFromEEPROM(3), OUT4_PIN),
  ClockOutput(readDividerFromEEPROM(4), OUT5_PIN),
  ClockOutput(readDividerFromEEPROM(5), OUT6_PIN),
};

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
  return cc/22; // roughly split the 0-127 midi CC range to 0-5 divider
}

void updateDivider(uint8_t outputNumber, uint8_t ccValue) {
  uint8_t value = mapCCToDivider(ccValue);
  if (value > THIRTYTWO)
    return;
  output[outputNumber].setDivider(value);
  EEPROM.update(outputNumber, value);
}

void handleControlChange(byte channel, byte number, byte value) {
  if (channel > 14) 
  {
    if(number == 0x01) { // Only send mod wheel to acid moose
      MIDI.sendControlChange(number, value, channel - 14);
    }
    // CC 20-26 control midi moose clock outputs
    else if (number == CLOCK_OUT_CC_FIRST) {
      updateDivider(0, value);
    }
    else if (number == CLOCK_OUT_CC_FIRST+1) {
      updateDivider(1, value);
    }
    else if (number == CLOCK_OUT_CC_FIRST+2) {
      updateDivider(2, value);
    }
    else if (number == CLOCK_OUT_CC_FIRST+3) {
      updateDivider(3, value);
    }
    else if (number == CLOCK_OUT_CC_FIRST+4) {
      updateDivider(4, value);
    }
    else if (number == CLOCK_OUT_CC_FIRST+5) {
      updateDivider(5, value);
    }
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (channel > 14)
  {
    MIDI.sendNoteOn(pitch, velocity, channel - 14);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
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

void handleStart(void) {
 thirtytwo.reset();
 sixteen.reset();
 eight.reset();
 fourth.reset();
 two.reset();
 full.reset();
}

void setup()
{
  MIDI.setHandleStart(handleStart);
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
