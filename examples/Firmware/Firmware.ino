#include "Easybot.h"

EasybotNano Robot;

//////////////////////////////////////
void setup()
{
  Robot.init(0);
  Robot.process();
}
///////////////////////////
void loop()
{
  Robot.process();
}