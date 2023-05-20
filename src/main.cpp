#include <Arduino.h>
#include <FaceRec.h>
#include <BTHome.h>
#define DEVICE_NAME "Face Recognition" // This name will appear in HA as new device
#define ENABLE_ENCRYPT // Remove this line along with the following 3 lines for a non secure BTHome advertise

#ifdef ENABLE_ENCRYPT
  String BIND_KEY = "231d39c1d7cc1ab1aee224cd096db932"; // Change this key with 32 a-f and 0-9 charakters (hex) this will be asked in HA
#endif 

#define RXD2 19 // The RX pin of your ESP32 board
#define TXD2 23 // The TX pin of your ESP32 board
#define INP 25 // The ESP32 pin where the out from 5G mmwave sensor is connected

FaceRec facerec;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);

#ifdef ENABLE_ENCRYPT
  facerec.BTHomeInit(DEVICE_NAME, true, BIND_KEY); // Initialize BTHome
#else
  facerec.BTHomeInit(DEVICE_NAME);
#endif
  
  pinMode(INP, INPUT_PULLDOWN);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_25, HIGH); // If you changed the INP change also the number here

  uint8_t comm = IDENT; // This means we will send an Identification command
  // facerec.sendBattery(); This will probably make sence in the future if the sensor will eventually be, battery friendly
  
  facerec.sendCommand(DISPL, TURNON); // Turn on the display
  facerec.rcvRecData();
  delay(500);
  facerec.sendCommand(BACKLIT, TURNON); // Turn on the backlight
  facerec.rcvRecData();
  
  delay(1500);
  while(digitalRead(INP)==HIGH){ // While mmwave detects presence
    facerec.sendCommand(comm);  // Send identify command
    facerec.rcvRecData();       // Read data
    delay(3000);
  }
  
  // There is no presence any more, enter power saving mode
  facerec.sendCommand(DISPL, TURNOFF); // Display off
  facerec.sendCommand(BACKLIT, TURNOFF); // Backlight off
  facerec.sendCommand(FLSH, TURNOFF); // Flash off

  delay(500);
  esp_deep_sleep_start(); // Esp go to sleep
}

void loop() {
}