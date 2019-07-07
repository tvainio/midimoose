#include "clock_divider.h"

ClockDivider::ClockDivider(int divider, callBack cb)
{
    this->cb = cb;
    this->divider = divider;
    this->halfWay = divider/2;
}

void ClockDivider::tick()
{
    if (currentStep == 0) {
	cb(true);
    }
	
    currentStep++;
    if (currentStep == halfWay) {
	cb(false);
    }

    currentStep %= divider;
}

void ClockDivider::reset()
{
    currentStep = 0;
}
