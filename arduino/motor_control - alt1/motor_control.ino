#include <Wire.h>
#include <Servo.h>
#include <L298N.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// Pin definition
#define RXPin 4
#define TXPin 3
#define GPSBaud 9600

#define sp 9
#define IN1 7
#define IN2 8
#define EN 5

#define SLAVE_ADDRESS 0x04

volatile boolean receiveFlag = false;

// Create one motor instance
L298N motor(EN, IN1, IN2);

// create servo object to control a servo
Servo myservo;

// TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
//SoftwareSerial ss(RXPin, TXPin);

// Definitions for motor control over i2c
char temp[32];
int servoState = 90;
int motorState = 0;
unsigned int ms = 0;
String message = "";

// Define a structure to hold the float numbers
typedef struct {
  float lat;
  float lon;
} FloatArray;

FloatArray data;

void setup() {
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveEvent);
  //Wire.onRequest(requestEvent); // register event

  Serial.begin(GPSBaud);
  //ss.begin(GPSBaud);
  //Serial.println("Ready!");

  myservo.attach(sp);  // attaches the servo on pin to the servo object
  //Serial.println(myservo.attach(sp));
  myservo.write(90);
}

void receiveEvent(int howMany) {
  for (int i = 0; i < howMany; i++) {
    temp[i] = Wire.read();
    temp[i + 1] = '\0'; //add null after ea. char
  }

  //RPi first byte is cmd byte so shift everything to the left 1 pos so temp contains our string
  for (int i = 0; i < howMany; ++i){
    temp[i] = temp[i + 1];
  }

  message = String(temp);
  //Serial.println(message);
  if (message.startsWith("ss")){
    message.replace("ss","");
    servoState = message.toInt();
  }
  if (message.startsWith("ms")){
    message.replace("ms","");
    motorState = message.toInt();
  }

  receiveFlag = true;
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  data.lat = gps.location.lat();
  data.lon = gps.location.lng();
  // Send the structure as a byte array
  Wire.write((byte*)&data, sizeof(data));
}


void servoControl(){
  Serial.println("Servostate: " + String(servoState));
  //analogWrite(5, servoState);
   
  myservo.write(servoState);
}

void motorControl(){
  ms = abs(motorState);
  if(ms > 0 || ms < 256){
    motor.setSpeed(ms);
    //Serial.println("ms: "+ String(ms));
    //Serial.println("motorState: "+ String(motorState));
    if (motorState < 0){
      motor.backward();
    } else if (motorState > 0){
      motor.forward();
    } else{
      motor.stop();
    }
  }
}

void loop() {

  if (receiveFlag == true) {
    //servoControl();
    motorControl();
    receiveFlag = false;
  }
  

  while (Serial.available() > 0)
    if (gps.encode(Serial.read()));

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    //Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

/*
void displayInfo(){
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }
}

void printSomeInfo(){
  Serial.print("Motor is moving = ");
  Serial.print(motor.isMoving());
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeed());
}
*/
