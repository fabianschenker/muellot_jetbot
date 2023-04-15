#include <Wire.h>
#include <Servo.h>
#include <L298N.h>

#define SLAVE_ADDRESS 0x04

volatile boolean receiveFlag = false;

// Pin definition
const unsigned int IN1 = 7;
const unsigned int IN2 = 8;
const unsigned int EN = 5;

const int sp = 9;

// Create one motor instance
L298N motor(EN, IN1, IN2);

Servo myservo;  // create servo object to control a servo

char temp[32];
int servoState = 90;
int motorState = 0;
unsigned int ms = 0;
String message = "";

void setup() {
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("Ready!");

  myservo.attach(sp);  // attaches the servo on pin 3 to the servo object
  //Serial.println(myservo.attach(sp));
  myservo.write(90);

  // Wait for Serial Monitor to be opened
  while (!Serial)
  {
    //do nothing
  }

  motor.setSpeed(70);
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
  Serial.println(message);
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

void servoControl(){
  Serial.println("Servostate: " + String(servoState));
  //analogWrite(5, servoState);
   
  myservo.write(servoState);
  
}

void motorControl(){
  ms = abs(motorState);
  if(ms > 0 || ms < 256){
    motor.setSpeed(ms);
    Serial.println("ms: "+ String(ms));
    Serial.println("motorState: "+ String(motorState));
    if (motorState < 0){
      motor.backward();
      printSomeInfo();
    } else if (motorState > 0){
      motor.forward();
      printSomeInfo();
    } else{
      motor.stop();
      printSomeInfo();
    }
  }
}

void loop() {
  /* // Tell the motor to go forward (may depend by your wiring)
  motor.forward();
  analogWrite(5, 135);
  delay(2000);

  

  //print the motor status in the serial monitor
  printSomeInfo();

  // Stop
  motor.stop();
  analogWrite(5, 45);
  delay(2000);
*/

  if (receiveFlag == true) {
    servoControl();
    motorControl();
    receiveFlag = false;
  }
}

void printSomeInfo()
{
  Serial.print("Motor is moving = ");
  Serial.print(motor.isMoving());
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeed());
}
