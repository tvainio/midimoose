// CLOCK_OUTPUT.h
#ifndef CLOCK_OUTPUT_H
#define CLOCK_OUTPUT_H
#include <stdint.h>

enum Divider {FULL, TWO, FOUR, EIGHT, SIXTEEN, THIRTYTWO};

class ClockOutput
{

private:
  uint8_t divider;
  uint8_t pin;
  
public:
  ClockOutput(uint8_t divider, uint8_t pin);
  void setState(uint8_t divider, bool state);
  void setDivider(uint8_t divider);
};

#endif
