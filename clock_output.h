// CLOCK_OUTPUT.h
#ifndef CLOCK_OUTPUT_H
#define CLOCK_OUTPUT_H
#include <stdint.h>

enum Divider { THIRTYTWO, SIXTEEN, EIGTH, FOUR, TWO, FULL };

class ClockOutput
{

private:
  uint8_t divider = SIXTEEN;
  uint8_t pin;
  
public:
  ClockOutput(uint8_t divider, uint8_t pin);
  void set(bool state);
};

#endif
