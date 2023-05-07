// Arduino Code
#include <Wire.h>

// Define a structure to hold the float numbers
typedef struct {
  float lat;
  float lon;
} FloatArray;

// Create an instance of the structure
FloatArray data;

void setup() {
  // Initialize I2C communication as slave with address 8
  Wire.begin(0x04);
  // Register a callback function to handle requests from master
  Wire.onRequest(requestEvent);
}

void loop() {
  // Set some random values for the float numbers
  data.lat = 47.522130;
  data.lon = 8.244435;
  // Wait for a second
  delay(1000);
}

// This function is called when the master requests data
void requestEvent() {
  // Send the structure as a byte array
  Wire.write((byte*)&data, sizeof(data));
}