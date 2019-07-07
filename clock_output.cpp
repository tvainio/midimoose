#include "clock_output.h"

ClockOutput::ClockOutput(uint8_t divider, uint8_t pin)
{
    this->divider = divider;
    this->pin = pin;
}

void ClockOutput::set(bool state)
{
}
