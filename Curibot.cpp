#include "Arduino.h"
#include "Curibot.h"

void Curibot::moveForward(int speed){
  int movespeed = speed*255/100; 
  LeftMotor.run(movespeed);
  RightMotor.run(movespeed);
}
////
void Curibot::moveBack(int speed){
  int  movespeed = speed*255/100; 
  LeftMotor.run(-movespeed);
  RightMotor.run(-movespeed);	
}
////////////////////////////////////////////
void Curibot::moveForward(int leftspeed,int rightspeed)
{
 	int Lspeed = leftspeed*255/100;
 	int Rspeed = rightspeed*255/100;
 	LeftMotor.run(Lspeed);
 	RightMotor.run(Rspeed);
}
///////////////////////////////////////////////////////////////
void Curibot::moveRight(int speed)
{	
  moveForward(speed,0);
}
 ///
void Curibot::moveLeft(int speed)
{
  moveForward(0,speed);
}
 
//////////////////////////////
void Curibot::stop()
{
	LeftMotor.stop();
  RightMotor.stop();
}
///////////////////////////////

void Curibot::turnRight(int speed)
{
  int turnspeed = speed*255/100;
  LeftMotor.run(turnspeed);
  RightMotor.run(-turnspeed);
}
/////////////////////////////////////////
void Curibot::turnRight(int speed,int time)
{
  int turnspeed = speed*255/100;
  int i = 0;

  LeftMotor.run(turnspeed);
  RightMotor.run(-turnspeed);
  for (i=0;i<time;i++)
  {
    delay(100);
  }
  stop();
  delay(200);
}
/////////////////////
void Curibot::turnLeft(int speed)
{
  int turnspeed = speed*255/100;
	LeftMotor.run(-turnspeed);
	RightMotor.run(turnspeed);
}
//////////
void Curibot::turnLeft(int speed,int time)
{
  int turnspeed = speed*255/100;
  int i = 0;
  LeftMotor.run(-turnspeed);
  RightMotor.run(turnspeed);
  for (i=0;i<time;i++)
  {
    delay(100);
  }
  stop();
  delay(200);
}
////////////////////////////////
void Curibot::setup_lineSensor(int color, int threshold_detect)
{
  _LINE_COLOR = color;
  _line_detect = threshold_detect; 
  pinMode(lineSensor_enable,OUTPUT);
}
//////////////////////////////////
int Curibot::readSensor(int channel)
{
  digitalWrite(lineSensor_enable,HIGH);
  int line_value = analogRead(channel);
  digitalWrite(lineSensor_enable,LOW);
  return line_value;
}
///////////////////////////////////////////////////////
int Curibot::leftSensor()
{	
  int value;
  digitalWrite(lineSensor_enable,HIGH);
  value = analogRead(leftline_pin);
  digitalWrite(lineSensor_enable,LOW);
  return value;
}
////////////////////////////////////////////////////////
int Curibot::rightSensor()
{
  int value;		
  digitalWrite(lineSensor_enable,HIGH);
  value = analogRead(rightline_pin);
  digitalWrite(lineSensor_enable,LOW);
  return value;
}
///////////////////////////////////////////////////
int Curibot::centerSensor()
{
  int value;	
  digitalWrite(lineSensor_enable,HIGH);
  value = analogRead(centerline_pin);
  digitalWrite(lineSensor_enable,LOW); 
  return value;
}
///////////////////////////////////
float Curibot::readSonar() 
{
  float range = Distance.Ranging(CM);
  delay(100);
  return range;
}
void Curibot::setServo(int Angle)
{
  // if (!servo.attached) 
  servo.attach(Servo_Pin);
  servo.write(Angle);
}
void Curibot::lightfollow()
{
  int left = getLight(1);
  int right = getLight(0);
  if((right < (medium + 10)) && (left < medium + 10)){
    stop();
  }
  else if((left - right) > 10){
    turnLeft(70);
  }
  else if((right - left) > 10){
    turnRight(70);
  }
  else if((right > (medium + 10)) && (left > medium + 10)){
    moveForward(80);
  }
  
}
void Curibot::avoidobstacle()
{
  int distance = getDistance();
  if (distance > 15){
    moveForward(80);
  }
  else { 
    stop();
    delay(300);
    moveBack(80);
    delay(500);
    stop();
    turnLeft(70);
    delay(500);
    stop();
    if (getDistance()<15){
      turnRight(70);
      delay(1000);
      stop();
    }
  }
}
void Curibot::linefollow()
{
  if(centerSensor() > 350)
  {
    moveForward(80,80);
  }
  else if(rightSensor() > 350)
  {
    turnRight(70);
  }
  else if(leftSensor() > 350)
  {
    turnLeft(70);
  }
}
//////////// NRF24L01 /////////////////////////////////////////////////////////////////////////
void Curibot::init(int _address)
{ 
  pinMode(Buzzer_Pin, OUTPUT);
  pinMode(RGB_Pin, OUTPUT);  
  pixels.begin();
  medium = (getLight(LEFT) + getLight(RIGHT)) / 2;
  Serial.begin(115200);
    #ifdef DEBUG
    Serial.print("Init.... ");
    #endif
  delay(1000);
   
  initNRF(_address);
  if (interface == NRF24L01_INTERFACE) {
    State = READ_RF;
    setColor(255,0,0);
    #ifdef DEBUG
    Serial.print("Curie Start with WireLess interface, Wireless address: ");
    Serial.println(myNode); 
  #endif
  }
  else  { 
    State = READ_SERIAL;
    setColor(0,0,255);
    Serial.println("Curie Start with Serial USB interface");

  }
  delay(1000);
  setColor(0, 0, 0);
  if (interface == NRF24L01_INTERFACE){
  if(readButton())
  {
    inConfig();
  }
  else Sound.sing(Mode3);
  }
  else Sound.sing(Mode3);
  first_run = true; 
  processMode = ONLINE;
}
///////////////////////////////////
int Curibot::getLight(byte side){
  if (!side) {  //LEFT
    int LDRL = analogRead(LDR2);
    LDRL = map(LDRL,0,1000,0,100);
    LDRL = LDRL > 100 ? 100 : LDRL;
    return LDRL;
  } 
  else {
    int LDRR = analogRead(LDR1);
    LDRR = map(LDRR,0,1000,0,100);
    LDRR = LDRR > 100 ? 100 : LDRR;
    return LDRR;
  }
}
float Curibot::getDistance()
{
  return Distance.Ranging(CM);
}
void Curibot::setColor(byte R, byte G, byte B)
{
  R = R > 255 ? 255 : R;
  G = G > 255 ? 255 : G;
  B = B > 255 ? 255 : B;

  pixels.setPixelColor(0, pixels.Color(R,G,B));
  pixels.show();
}
void Curibot::offRGB()
{
  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.show();
}
bool Curibot::readButton()
{
  int BT = analogRead(SET);
  if(BT > 1020)
    return 1;
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
void Curibot::initNRF(int _address)
{
  //config_Address(2,10); 
  bool NRFConnected = false;
  if (!_address){
    #if DEBUG
    Serial.println("Pairing Mode loading...");
    #endif
    load_address();
    connection = PAIRING;
  }
  else  {                  //Network addressing Mode
    myNode = _address;     
    toNode = 0;            //set Master address 
    connection = NETWORK;
  } 
  Radio.setDynamicPayload(false); // disable Dynamic Payload;
  //  Radio.RFbegin();    //init with my Node address
   NRFConnected = Radio.RFbegin();
  if (NRFConnected) {
  Radio.setDataSpeed(RF24_1MBPS); 
  Radio.setChannelRF(108);
  Radio.setPowerRF(RF24_PA_LOW);
  //Radio.setRetry(10,3);
  Serial.println("NRF wireless ready!"); 
  Radio.setAutoACK(true);
  Radio.init(myNode);
  interface = NRF24L01_INTERFACE;
  first_run = true;      //set first run for next State
  }
  else {
      Serial.println("NRF Module is missing, switch to Serial connection"); 
      interface = SERIAL_INTERFACE; 
      first_run = true;      //set first run for next State

  }
}
void Curibot::resetNRF(){
  #ifdef DEBUG
  Serial.println("Reset RF module");
  #endif
  stop();
  Radio.RFpowerDown();
  for (int i=0;i<3;i++){
  setColor(255,0,0);
  delay(250);
  setColor(0,0,0);
  delay(250);
  }
 Radio.setDynamicPayload(false); // disable Dynamic Payload;
 Radio.init(myNode);    //init with my Node address
 Sound.sing(Happy_short);
  setColor(255,0,0);
  delay(250);
  setColor(0,0,0);
  //State = READ_RF;
  first_run = true;      //set first run for next State
  
}
void Curibot::load_address()
{ 
   #ifdef DEBUG
   Serial.println("Loading my Config... ");
   #endif
   myNode = EEPROM_readInt(0);
   toNode = EEPROM_readInt(2);
   #ifdef DEBUG
  Serial.print("My Address: ");
  Serial.println(myNode);
   Serial.print("Target Address: ");
  Serial.println(toNode);
    #endif
  // Radio.init(myNode);    //init with my Node address
}
/////
bool Curibot::inConfig() //check if press CONFIG KEY
{
  uint8_t size;
  long _duration=0;
  bool accessed = false;
  if(readButton())
  {
    #ifdef DEBUG
    Serial.println("CONFIG KEY PRESSED!");
    Serial.println("Wait 3s...");
    #endif 
    _startTime = millis();
    while(readButton()) {
    _duration = millis() - _startTime; 
      if(_duration > 3000) {
        accessed = true; 
        Sound.sing(Sad);
      }
    }    // wait to check holding key timing
    if(accessed)
    { 
      setColor(255,0,255);
      Radio.init(Default_Addr);  // set Default Address to listen for Config Address Mode 
      #ifdef DEBUG
      Serial.print("GOING TO CONFIG MODE WITH DEFAULT CONFIG ADDRESS...:");Serial.println(Default_Addr);
      #endif 
      Mode = CONFIG_MODE; 
      accessed = false; 
     }
    else
    {
      #ifdef DEBUG
      Serial.println("CONFIG MODE IS NOT ACCESSED!");
      #endif 
     // tick(1,1000,1000);
     Sound.sing(Disconnection);
    }

  }
  else return 0;
 // State = READ_RF; //back to wait RF message
 // Mode = CONFIG_MODE; 
//  first_run = true; 
}
////////////////
void Curibot::config_Address(uint16_t myAddress,uint16_t toAddress){

  myNode = myAddress;   //need be checked endianess
  toNode = toAddress;  
  #ifdef DEBUG
    Serial.println("Receive New addressing!");    
    Serial.print("My new Address:"); Serial.println(myNode);
    Serial.print("Sending to Address:"); Serial.println(toNode);
  #endif 
  if (toNode!=0) {   // not in Network mode, do not save     
    EEPROM_writeInt(0,myNode);  //saving my new address
    EEPROM_writeInt(2,toNode);    //saving target address
  }
  #ifdef DEBUG
    Serial.print("Set address to RF:"); Serial.println(myNode);
  #endif
  // myNode = new_addr; 
  //Radio.SetAddress(myNode);       
  Radio.init(myNode);    //init with my Node address  
  #ifdef DEBUG
    Serial.print("Set address done:"); Serial.println(myNode);
  #endif                
}
///////////////////////////////////////////////////////////////////////
int Curibot::changeMode()
{
  stop();
  Sound.sing(Disconnection);
  processMode++;
  if(processMode == 5)
    processMode = ONLINE;

  if(processMode == ONLINE)
  {
    #ifdef DEBUG
      Serial.println(String("Process Mode: ") + processMode);
    #endif
    setColor(255,0,0);

    delay(500);
    offRGB();
  }
  else if(processMode == LIGHT)
  {
    #ifdef DEBUG
      Serial.println(String("Process Mode: ") + processMode);
    #endif
    setColor(0,255,0);
    delay(500);
    offRGB();
  }
  else if(processMode == LINE)
  {
    #ifdef DEBUG
      Serial.println(String("Process Mode: ") + processMode);
    #endif
    setColor(0,0,255);
    delay(500);
    offRGB();
  }
  else if(processMode == AVOID)
  {
    #ifdef DEBUG
      Serial.println(String("Process Mode: ") + processMode);
    #endif
    setColor(255,255,0);
    delay(500);
    offRGB();
  }
  else if(processMode == OFFLINE)
  {
    #ifdef DEBUG
      Serial.println(String("Process Mode: ") + processMode);
    #endif
    setColor(255,255,255);
    delay(500);
    offRGB();
  }
  return processMode;
}
void Curibot::process()
{ 
 //
  static bool first_process = true; 
  do
  {
    switch(processMode)
    {
      case ONLINE:
      {
        run();
      }
        break;
      
      case LIGHT:
      {
        lightfollow();
      }
        break;
      
      case LINE:
      {
        linefollow();
      }
        break;
      
      case AVOID:
      {
        avoidobstacle();
       }
        break;

      case OFFLINE:
      {
      }
      break; 
      
    }
   // Serial.println("Check button");
    if(readButton()) changeMode();

  }
  while(processMode != OFFLINE);
}
///////////////////////////////////////////////////////////////////////
void Curibot::run(){
  if (first_run)  {
    timeStart = millis();
    #ifdef DEBUG 
      Serial.print("State No: ");
      Serial.print(State);
      Serial.println("   Running");
    #endif
    first_run = false; 
  }  
  switch  (State) {
    case READ_RF: {     //check and read RF data comming if any, if not, go to check setting Address mode
      readRF();
    }
    break;
    case PARSING: {
      parseData();
    }
    break;
    case WRITE_RF: {
      writeRF();
    }
    break;
    case RC : {
     // RC_Run();
    }
    break;
    ///
    case READ_SERIAL: {
      readSerial();
    }
    break;
    case WRITE_SERIAL: {
      writeSerial();
    }
    break;
 }
}
////////////////////////////////////////////
void Curibot::runFunction(int device)
{
  //0xff 0x55 0x6 0x0 0x1 0xa 0x9 0x0 0x0 0xa
  //int port = buffer[6];
  //int pin = port;
  switch(device)
  {
    case STOP:
    {
      stop();
      Mode = RUN_MODE;
      break;
    }
    case SETMOTOR:
    {
      int leftspeed = readShort(6);
      int rightspeed = readShort(8);
      moveForward(leftspeed, rightspeed);
      Mode = RUN_MODE;
      break;
    }
    case FORWARD:
    {
      int speed = readShort(6);
      moveForward(speed);
      Mode = RUN_MODE;
      break;
    }
    case BACKWARD:
    {
      int speed = readShort(6);
      moveBack(speed);
      Mode = RUN_MODE;
      break;
    }
    case TURNLEFT:
    {
      int speed = readShort(6);
      turnLeft(speed);
      Mode = RUN_MODE;
      break;
    }
    case TURNRIGHT:
    {
      int speed = readShort(6);
      turnRight(speed);
      Mode = RUN_MODE;
      break;
    }
    case SETSERVO:
    {
      int angle = readShort(6);
      setServo(angle);
      Mode = RUN_MODE;
      break;
    }
    case DISABLE_SERVO:
    {
      disableServo();
      Mode = RUN_MODE;
      break;
    }
    case PLAYMEMODY:
    {
      float noteFrequency = readShort(6);
      float noteDuration = readShort(8);
      Sound._playNote(noteFrequency, noteDuration);
      Mode = RUN_MODE;
      break;
    }
    case SETCOLOR:
    {
      byte R = readBuffer(6);
      byte G = readBuffer(7);
      byte B = readBuffer(8);
      setColor(R,G,B);
      Mode = RUN_MODE;
      break;
    }
    case OFFCOLOR:
    {
      offRGB();
      Mode = RUN_MODE;
      break;
    }
    case CREATESOUND:
    {
      int sounds = readShort(6);
      Sound.sing(sounds);
      Mode = RUN_MODE;
      break;
    }
    case CONFIG:
    {
      if (Mode = CONFIG_MODE) {
      uint16_t myAddress = readShort(6);
      uint16_t toAddress = readShort(8);
      config_Address(myAddress,toAddress); //saving new addressing pair
      Sound.sing(Connection);
      // Mode = RUN_MODE;
      }
    }break;

    case RCDATA:
    {
      Mode = RC_MODE; 
      keyState = buffer[6];
      varSlide = buffer[7];
      remoteProcessing();      
      
    }break;
  } 
}
////////////////////////////////////////////////////
void Curibot::readSensors(int device)
{
  /*****************Recevice*************************
      ff 55 len idx action device port slot data a
      0  1  2   3   4      5      6    7    8
  *********************Response**********************
      ff 55 idx type data \r \n
  ***************************************************/
  float value=0.0;
  int port,slot,pin;
  port = readBuffer(6);
  pin = port;
  switch(device)
  {
     case DISTANCE:
    {
      sendFloat((float)getDistance());
    }
    break;

    case LIGHT_LEVEL:
    {
     uint8_t side = readBuffer(6); 
     sendFloat((float)getLight(side));
    }
    break;

    case CENTERLINE:
    {
      sendShort((int)centerSensor());
    }
    break;

    case RIGHTLINE:
    {
      sendShort((int)rightSensor());
    }
    break;

    case LEFTLINE:
    {
      sendShort((int)leftSensor());
    }
    break;

    case DONE:
    {
     sendFloat(actionDone);
    }
    break;
  } 
}
////////////////////////////////////////////////////
void Curibot::RC_Run(){
  switch (RC_type){ 
    case LIGHT_FOLLOW: {
      int left = getLight(1);
      int right = getLight(0);
      if((right < (medium + 10)) && (left < medium + 10)){
        stop();
      }
      else if((right > (medium + 10)) && (left > medium + 10)){
        moveForward(speed);
      }
      else if((left - right) > 10){
        turnLeft(speed);
      }
      else if((right - left) > 10){
        turnRight(speed);
      }
    } break;

    case AVOID_OBSTACLE: {
      int distance = getDistance();
      if (distance > 15){
        moveForward(speed);
      }
      else { 
        stop();
        delay(300);
        moveBack(speed);
        delay(500);
        stop();
        turnLeft(speed);
        delay(300);
        stop();
        if (getDistance()<15){
          turnRight(speed);
          delay(600);
          stop();
        }
      }
    } break;
    case CREATE_SOUND: {
      //Sound.playMusic(songname);
      RC_type = RC_MANUAL;
      songname--;
      if(songname <= 0)  songname = 3;
    } break;
  }
  State = READ_RF; 
  first_run = true;
}
/////////////////////////////////////////////
void Curibot::readSerial(){

//isAvailable = false; 
if (Serial.available() > 0){
  isAvailable = true; 
  serialRead = Serial.read();

    #ifdef DEBUG_SERIAL
    Serial.print(".");
    #endif
   unsigned char c = serialRead&0xff;
    if(c==0x55&&isStart==false){
     if(prevc==0xff){
      index=1;
      isStart = true;
      #ifdef DEBUG_SERIAL
     // Serial.print("*");
      #endif 
      //buffer[index]=c;
    }
    }else{
      prevc = c;  
      if(isStart){
        if(index==2){
         dataLen = c; 
          #ifdef DEBUG_SERIAL
         Serial.print(c,HEX);
         Serial.print(" ");
         #endif 
          }else if(index>2){
          dataLen--;
            #ifdef DEBUG_SERIAL
             Serial.print(dataLen);
             Serial.print(" ");
            #endif
        }
      }
    } 
     buffer[index]=c;
     index++;
     if(index>51){
      index=0; 
      isStart=false;
     }
     if(isStart&&dataLen==0&&index>3){ 
         isStart = false;
         State = PARSING; // Serial Data available now, State change to parsing 
         first_run = true;      //set first run for next State
         #ifdef DEBUG 
         Serial.print("Valid Data coming ");
         for (int i=0;i<index+1;i++) {
           Serial.print(buffer[i],HEX); Serial.print("-");
         }
         Serial.println();
         Serial.println("Goto Parsing");
         #endif
        index=0;
     }
  }
 //else Serial.println("No serial comming");
 //else LEDdebug();
  
   
}  

////////////////////////////////////////////
void Curibot::readRF(){
    RFread_size = 0; 
 //  LEDdebug();
  if ( Radio.RFDataCome() )  {
    #ifdef DEBUG
    Serial.println("RF data come!");
    Serial.println(Radio.checkCarrier() ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
    #endif
    while (Radio.RFDataCome()) {
      Radio.RFRead(buffer,sizeof(buffer));
    }
    isAvailable = true; 
   
   // if (RFread_size <3) return;
    // int plen = buffer[2];
     if (buffer[0]==0xFF && buffer[1] == 0x55 ){  // check Start bytes and ending byte for validating data
      #ifdef DEBUG 
      Serial.print("received valid Scratch RF data: ");
      PrintDebug(buffer,buffer[2]+3);
      Serial.println();
      #endif 
      State = PARSING;
      first_run = true;      //set first run for next State
      #ifdef DEBUG
        Serial.println("Go to State 1: PARSING");
      #endif  
      return;
    }
    else  {
      #ifdef DEBUG
        Serial.println("invalid data received"); 
        PrintDebug(buffer,buffer[2]+3);
        Serial.println();
      #endif
      callOK();          // if invalid data, do nothing and send back ACK
      State = WRITE_RF;
    //    State = READ_RF; 

      first_run = true;      //set first run for next State
      return;
    }
   //Data available, go to Parsing
  }
  /*
  else {
    inConfig();  //if not receive RF data, check the config key 
    //State = IN_CONFIG;    //if not received RF message, go to check config mode access
    //first_run = true; 
    if (Mode == RC_MODE && RC_type != RC_MANUAL) { State = RC; first_run = true; } 
  } 
  */
}
////
void Curibot::PrintDebug(unsigned char *buf,int len){
  for (int i=0;i<len;i++)
  {
    Serial.print(*(buf+i),HEX); Serial.print("-");
  }
  Serial.println();
}
//////////////////////////////////////////////////
/*
mBlock to Robot: 
ff 55 len idx action device port  slot  data \n
0  1   2   3   4      5      6     7     8
//////////////////////
Robot to mBlock (Response Get action) /////////////
ff 55 idx Type  data  0xa  0xd
0  1   2    3     4 
*/
////////////////////////////////////////////////////
void Curibot::parseData()
{ 
  ind = 0; //reset RF_buffer (send) index
  isStart = false;
  int idx = buffer[3];
  command_index = (uint8_t)idx;
  int action = buffer[4];
  int device = buffer[5];
  switch(action){
    case GET:{
      if(device != ULTRASONIC_SENSOR){
        writeHead();
        writeBuffer(ind++,idx);
      }
      readSensors(device);
      writeEnd();
      if (interface == NRF24L01_INTERFACE)  State = WRITE_RF;
      else State = WRITE_SERIAL;
      first_run = true;      //set first run for next State
    }
    break;
    case RUN:{ //need DEBUG Here
      runFunction(device);
      if (Mode == CONFIG_MODE) { 
        if (device == CONFIG) {
          Mode = RUN_MODE; //done config, go back to run mode, no ACK response
          State = READ_RF;
          clearBuffer(buffer,sizeof(buffer));
          first_run = true; 
          offRGB();
        }
      }
      else if (Mode == RC_MODE) {    //in RC Mode 
     
        //  callOK();   //response OK when complete action
          State = READ_RF;
         clearBuffer(buffer,sizeof(buffer));
         clearBuffer(RF_buf,sizeof(RF_buf));        
          Mode = RUN_MODE;          
          first_run = true; 
       
      }  
      else  {  //in USB wireless mode or Serial mode for Scratch API processing 
        callOK();   //response OK when complete action
        #ifdef DEBUG 
          Serial.println("Scratch command Done, go to send response");
        #endif 
        // delay(2000);
      if (interface == NRF24L01_INTERFACE)  State = WRITE_RF;
      else State = WRITE_SERIAL;      
        first_run = true;   //set first run for next State
      }      
    }
    break;  
    case RESET: {
    callOK();
      if (interface == NRF24L01_INTERFACE)  State = WRITE_RF;
      else State = WRITE_SERIAL;      
        first_run = true;
    }
    break;
    default: 
    callOK();
      if (interface == NRF24L01_INTERFACE)  State = WRITE_RF;
      else State = WRITE_SERIAL;      
        first_run = true;
    break;
  }
  clearBuffer(buffer,sizeof(buffer));  //clear receiving buffers 
}
///////////
void Curibot::writeRF() {
  bool OK = Radio.RFSend(toNode,RF_buf,sizeof(RF_buf));
  if (OK) {
    #ifdef DEBUG 
      Serial.print("Sent buffer: ");
      PrintDebug(RF_buf,ind+1);
      Serial.println();
    #endif 
  }
  else {
    #ifdef DEBUG 
      Serial.println("Sent FAIL ");
    #endif 
    resetNRF();
   // Sound.sing(OhOoh); 
  }  
  ind = 0; 
  State = READ_RF; 
  clearBuffer(buffer,sizeof(buffer));
  clearBuffer(RF_buf,sizeof(RF_buf));
  first_run = true;      //set first run for next State
}
//////////////////////////
void Curibot::writeSerial(){
for (int i=0;i<ind+1;i++) {
  Serial.write(RF_buf[i]);
  delayMicroseconds(100);
 }
  clearBuffer(buffer,sizeof(buffer));
  clearBuffer(RF_buf,sizeof(RF_buf));
  State = READ_SERIAL; 
}
//////////////////////////////////////////////////////////////
void Curibot::remoteProcessing(){
  ////////////////////////////////////////////////
  ///keyState  7  6  5  4  3  2  1   0        ////
  ///          F4 F3 F2 F1 L  R Bwd Fwd       ////
  ////////////////////////////////////////////////
  int _angle = 0;
  bool shift = bitRead(keyState,7);  // F4 key pressed; 
  _angle = map(varSlide,0,100,0,180); //mapping from 0-100% to real delay value of steps 150 ms - 50ms
  if (shift) speed = 50; else speed = 100; 
  if (bitRead(keyState,0)) { //forward
  
      moveForward(speed);

   
  } 
  else if (bitRead(keyState,1)) {
   
      moveBack(speed);
  
  }
  else if (bitRead(keyState,2)) {
  
      turnRight(speed);
   
  }
  else if (bitRead(keyState,3)) {
 
      turnLeft(speed);
   
  } 
  else if((keyState & 0x0F) == 0  )  //if the direction control is released, stop
  {
    stop();
  }
  if (bitRead(keyState,4)) {   //F1 key press(shift)
  }
  else if (bitRead(keyState,5)) {  //F2 key press, control the Servo
  setServo(_angle);
  #if DEBUG 
   Serial.print("Servo control: "); Serial.println(_angle);
  #endif
  }
  else if (bitRead(keyState,6)) {  //F3 key press

  }
  else if (bitRead(keyState,7)) {  //F4 key press

  }
  /*
  else{
    stop();
    RC_type = RC_MANUAL;
  }*/
}     
///////////////////////////////////
void Curibot::remoteInit(){
 bool NRFConnected = false;
  
    #if DEBUG
    Serial.println("Pairing Mode loading...");
    #endif
    load_address();
    connection = PAIRING;

  Radio.setDynamicPayload(false); // disable Dynamic Payload;
  //  Radio.RFbegin();    //init with my Node address
   NRFConnected = Radio.RFbegin();
  if (NRFConnected) {
  Radio.setDataSpeed(RF24_1MBPS); 
  Radio.setChannelRF(108);
  Radio.setPowerRF(RF24_PA_LOW);
  Radio.setRetry(10,3);
  Serial.println("NRF wireless ready!"); 
  Radio.setAutoACK(true);
  Radio.init(myNode);
  interface = NRF24L01_INTERFACE;
  first_run = true;      //set first run for next State
  }
  else {
      Serial.println("NRF Module is missing"); 
         //set first run for next State

  }
}


//////////////////////////////////////////////////////////////
void Curibot::remoteReading(){
  ////////////////////////////////////////////////
  ///keyState  7  6  5  4  3  2  1   0        ////
  ///          F4 F3 F2 F1 L  R Bwd Fwd       ////
  ////////////////////////////////////////////////
  int remote_data[2];
  int _angle = 0;
  if (first_remoteInit)  {
    first_remoteInit = false; 
    remoteInit();
  }
  if ( Radio.RFDataCome() )  {
    #ifdef DEBUG
    Serial.println("RF data come!");
    Serial.println(Radio.checkCarrier() ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
    #endif
    while (Radio.RFDataCome()) {
      Radio.RFRead(remote_data,sizeof(remote_data));
    }
   
   // if (RFread_size <3) return;
    // int plen = buffer[2];
   
      #ifdef DEBUG 
      Serial.print("received valid Scratch RF data: ");
      for (int i=0;i<(sizeof(remote_data)/sizeof(remote_data[0]));i++){
       Serial.print(remote_data[i]);
      }
      Serial.println();
      #endif 
      
      #ifdef DEBUG
        Serial.println("Update_data");
      #endif  
      keyState = remote_data[0];
      varSlide = remote_data[1];
     // _angle = map(varSlide,0,100,0,180); //mapping from 0-100% to real delay value of steps 150 ms - 50ms

  }
  
}  
///////////////////////////////////////////////////////////
void Curibot::EEPROM_writeInt(int address,uint16_t value) 
{
  //Decomposition from a int to 2 bytes by using bitshift.
  //One = Most significant -> Two = Least significant byte
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);
  //Write the 2 bytes into the eeprom memory.
  EEPROM.write(address, two);
  EEPROM.write(address + 1, one);    
}
/////////////////////////
uint16_t Curibot::EEPROM_readInt(int address)
{
  uint16_t two = EEPROM.read(address);
  uint16_t one = EEPROM.read(address+1); 
  return ((two & 0xFF) + ((one<<8)&0xFFFF));
}
///////////////////////////
void Curibot::clearBuffer(unsigned char *buf, int leng){
  for (int i=0;i<leng;i++) {
    *(buf+i) = 0; 
  }
}

//Private method for data package
void Curibot::writeHead(){
  ind = 0;
  RF_buf[ind++]=0xff;
  RF_buf[ind++]=0x55;
}
void Curibot::writeEnd(){
RF_buf[ind++] = 0xd; 
RF_buf[ind] = 0xa; 
}

unsigned char Curibot::readBuffer(int index){    
  return buffer[index]; 
}
void Curibot::writeBuffer(int index,unsigned char c)
{
  RF_buf[index]=c;
}
void Curibot::callOK()
{ ind = 0;
  writeBuffer(ind++,0xff);
  writeBuffer(ind++,0x55);
  writeEnd();
}
void Curibot::sendByte(char c)
{
  writeBuffer(ind++,1);
  writeBuffer(ind++,c);
}
void Curibot::sendString(String s)
{
  int l = s.length();
  writeBuffer(ind++,4);
  writeBuffer(ind++,l);
  for(int i=0;i<l;i++)
  {
    writeBuffer(ind++,s.charAt(i));
  }
}
void Curibot::sendFloat(float value)
{
  writeBuffer(ind++,0x2);
  val.floatVal = value;
  writeBuffer(ind++,val.byteVal[0]);
  writeBuffer(ind++,val.byteVal[1]);
  writeBuffer(ind++,val.byteVal[2]);
  writeBuffer(ind++,val.byteVal[3]);
}
void Curibot::sendShort(double value)
{
  writeBuffer(ind++,3);
  valShort.shortVal = value;
  writeBuffer(ind++,valShort.byteVal[0]);
  writeBuffer(ind++,valShort.byteVal[1]);
}
void Curibot::sendDouble(double value)
{
  writeBuffer(ind++,2);
  valDouble.doubleVal = value;
  writeBuffer(ind++,valDouble.byteVal[0]);
  writeBuffer(ind++,valDouble.byteVal[1]);
  writeBuffer(ind++,valDouble.byteVal[2]);
  writeBuffer(ind++,valDouble.byteVal[3]);
}
short Curibot::readShort(int idx)
{
  valShort.byteVal[0] = readBuffer(idx);
  valShort.byteVal[1] = readBuffer(idx+1);
  return valShort.shortVal;
}

float Curibot::readFloat(int idx)
{
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.floatVal;
}
long Curibot::readLong(int idx)
{
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.longVal;
}
