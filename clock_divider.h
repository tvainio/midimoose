// CLOCK_DIVIDER.h
#ifndef CLOCK_DIVIDER_H
#define CLOCK_DIVIDER_H

typedef void (*callBack)(bool);

class ClockDivider
{

private:
  callBack cb;
  int currentStep = 0;
  int divider = 2;
  int halfWay = 1;
  
public:
  ClockDivider(int divider, callBack cb);
  void tick();
  void reset();

};

#endif
