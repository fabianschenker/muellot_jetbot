#include <Wire.h>
#include <Servo.h>
#include <L298N.h>

#define SLAVE_ADDRESS 0x04

// Pin definition
const unsigned int IN1 = 7;
const unsigned int IN2 = 8;
const unsigned int EN = 9;

// Create one motor instance
L298N motor(EN, IN1, IN2);

Servo myservo;  // create servo object to control a servo

char temp[32];
int servoState = 0;
int motorState = 0;
String motorDirection = "RELEASE";
String message = "";

void setup() {
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("Ready!");

  myservo.attach(3);  // attaches the servo on pin 3 to the servo object
  myservo.write(90);

  motor.setSpeed(0);
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
  Serial.print(message);
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
    motor.setSpeed(abs(motorState));
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
  servoControl();
  motorControl();
}
