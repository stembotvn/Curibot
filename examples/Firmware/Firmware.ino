#include "Curibot.h"

Curibot Robot;

//////////////////////////////////////
void setup()
{
  Robot.init(0);
  Robot.process();
}
//////////////////////////////////////////////////////
void loop()                                     //////
{
  Robot.process();
}
//////////////////////////////////////////////////////