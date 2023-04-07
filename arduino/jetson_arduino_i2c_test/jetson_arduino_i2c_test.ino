#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

int i2cAddress = 0x40;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

//int pos = 0;    // variable to store the servo position

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(4);

void setup()
{
  Wire.begin(i2cAddress);                // join i2c bus with address #0x40
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(sendData);
  
  //Serial.begin(9600);           // start serial for output
  myservo.attach(3);  // attaches the servo on pin 3 to the servo object
  myservo.write(90);

  AFMS.begin();
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);
}

int servoState = 0;
int motorState = 0;
String motorDirection = "RELEASE";
String message = "";

void receiveEvent(String bytes) {
  message = Wire.read();  // read one character from the I2C
  if (message.startsWith("ss")){
    message.replace("ss","");
    servoState = message.toInt();
  }
  if (message.startsWith("ms")){
    message.replace("ms","");
    motorState = message.toInt();
  }
}

void servoControl(){
  if(servoState > 45 || servoState < 135) { 
    myservo.write(servoState);
  }
}

void motorControl(){
  if(abs(motorState) > 0 || abs(motorState) < 256){
    myMotor->setSpeed(abs(motorState));
    if (motorState < 0){
      myMotor->run("BACKWARD");
    } else if (motorState > 0){
      myMotor->run("FORWARD");
    } else{
      myMotor->run("RELEASE");
    }
  }
}

void loop()
{
  servoControl();
  motorControl();
}

void sendData(){
    Wire.write(servoState);
}