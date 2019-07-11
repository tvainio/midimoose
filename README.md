# Midimoose
Simple Arduino based midi utility device to overcome the issues with my TM3030 synth.

## Midi thru harware
Direct midi thru outputs for dividing midi signals to other devices.

## Filtered Midi output for TM3030.
Maps midi channels 15 to 1 and 16 to 2. Filter out other channels. Filters out everything except note on, note off and mod wheel.

## Clock Outputs
6 pulse outputs for syncing analog gear to midi clock. Each output is individually controllable with Midi CC 20-25:
* 32th notes
* 16th notes
* 8th notes
* 4th notes
* half notes
* full notes

Output configuration is stored into EEPROM and preserved over resets.
