#include <MIDI.h>
#include <SoftwareSerial.h>

// Maps midi channels 15 to 1 and 16 to 2. Filter out other channels. Filters
// out everything except note on, note off and mod wheel.

//SoftwareSerial debugSerial(10, 11); // RX, TX

MIDI_CREATE_DEFAULT_INSTANCE();

void handleCC(byte channel, byte number, byte value) {
  if (channel > 14 && number == 0x01) // Only send mod wheel
  {
    MIDI.sendControlChange(number, value, channel-14);
  }
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  if (channel > 14)
  {
    MIDI.sendNoteOn(pitch, velocity, channel-14);
//         debugSerial.print(channel, HEX);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  //    debugSerial.println("off");

  if (channel > 14) {
    MIDI.sendNoteOff(pitch, velocity, channel-14);
    digitalWrite(LED_BUILTIN, LOW);
  }
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
//    debugSerial.begin(115200);
//    debugSerial.println("Hello, world?");

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
