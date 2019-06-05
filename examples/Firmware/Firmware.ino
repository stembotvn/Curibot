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
  /*
  if (Robot.remoteReading())  {
  //  Robot.remoteProcessing();
  
  if (Robot.remoteReadButton(REMOTE_FWD)) Robot.moveForward(100);
  else if (Robot.remoteReadButton(REMOTE_BWD)) Robot.moveBack(100);
  else if (Robot.remoteReadButton(REMOTE_RIGHT)) Robot.turnRight(100);
  else if (Robot.remoteReadButton(REMOTE_LEFT)) Robot.turnLeft(100);
  else Robot.stop();
  int _angle = map(Robot.remoteReadPot(),0,100,0,180);
  Robot.setServo (_angle);     
  } */
}
//////////////////////////////////////////////////////