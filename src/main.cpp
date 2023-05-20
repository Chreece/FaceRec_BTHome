#include <Arduino.h>
#include <FaceRec.h>
#include <BTHome.h>
#define DEVICE_NAME "Face Recognition"
#define ENABLE_ENCRYPT

#ifdef ENABLE_ENCRYPT
  String BIND_KEY = "231d39c1d7cc1ab1aee224cd096db932";
#endif 

#define RXD2 19
#define TXD2 23
#define INP 25

FaceRec facerec;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RXD2, TXD2);

#ifdef ENABLE_ENCRYPT
  facerec.BTHomeInit(DEVICE_NAME, true, BIND_KEY);
#else
  facerec.BTHomeInit(DEVICE_NAME);
#endif
  
  pinMode(INP, INPUT_PULLDOWN);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_25, HIGH);

  uint8_t comm = IDENT;
  facerec.sendBattery();
  
  facerec.sendCommand(DISPL, TURNON);
  facerec.rcvRecData();
  delay(500);
  facerec.sendCommand(BACKLIT, TURNON);
  facerec.rcvRecData();
  
  delay(1500);
  while(digitalRead(INP)==HIGH){
    facerec.sendCommand(comm);
    facerec.rcvRecData();
    delay(3000);
  }
  
  facerec.sendCommand(DISPL, TURNOFF);
  facerec.sendCommand(BACKLIT, TURNOFF);
  facerec.sendCommand(FLSH, TURNOFF);

  delay(500);
  esp_deep_sleep_start();
}

void loop() {
}