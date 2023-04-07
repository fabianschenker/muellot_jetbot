#include <Wire.h>

// I2C-Adresse des Arduino
#define ARDUINO_ADDRESS 0x12

// Puffer für den empfangenen String
char received_string[255];

void setup() {
  // Starten der seriellen Kommunikation
  Serial.begin(9600);

  // Starten der I2C-Kommunikation
  Wire.begin(ARDUINO_ADDRESS);

  // Registrieren der Empfangsfunktion
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Hier können weitere Operationen ausgeführt werden
  // ...

  // Ausgabe des empfangenen Strings im Serial Monitor
  if (strlen(received_string) > 0) {
    Serial.println("Received String: " + String(received_string));
    // String löschen, um Platz für neue Daten zu machen
    received_string[0] = '\0';
  }

  // Hier kann weiterer Code ausgeführt werden
  // ...
}

// Empfangsfunktion für I2C-Daten
void receiveEvent(int numBytes) {
  // Lesen der empfangenen Daten
  for (int i = 0; i < numBytes; i++) {
    received_string[i] = Wire.read();
  }
  // Hinzufügen eines Nullterminators am Ende des empfangenen Strings
  received_string[numBytes] = '\0';
}
