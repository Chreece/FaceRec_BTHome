#include "Arduino.h"
#include "FaceRec.h"
#include "BTHome.h"
#include "esp_adc_cal.h"

BTHome bthome;

void FaceRec::rcvRecData(){
  static boolean rcving = false;
  static uint8_t iDx = 0;
  byte startByte = SYNCWORD1;
  byte parityByte = 0;
  byte bt;
  while ( Serial1.available() > 0 && !this->rcvComplete && iDx < 256 )
  {
    bt = Serial1.read();
    if ( bt == startByte && !rcving ) {
        rcving = true;
        this->Data[0] = bt;
        iDx = 1;
    } else if ( rcving && iDx <= 12) {
        if (iDx > 2 && parityByte != bt ){
          parityByte += bt;
        }
        this->Data[iDx] = bt;
        iDx++;
        if (bt == parityByte && iDx > 7) {
            rcving = false;
            iDx = 0;
            parityByte = 0;
            this->rcvComplete = true;
            paketReady();
        }
      } else {
        rcving = false;
        parityByte = 0;
        iDx++;
      }
  } 

}

void FaceRec::paketReady(){
  switch(this->Data[7]){
    case IDENT:
      bthome.resetMeasurement();
      switch(this->Data[8]){
        case SUCC:
          byte userId;
          userId = this->Data[9] + this->Data[10];
          bthome.addMeasurement(ID_COUNT, (uint64_t) userId);
          bthome.addMeasurement_state(STATE_PRESENCE, STATE_ON);
          break;
        default:
          bthome.addMeasurement(ID_COUNT, 0.0f);
          bthome.addMeasurement_state(STATE_PRESENCE, STATE_OFF);
          break;
      }
      bthome.sendPacket();
      delay(1500);
      bthome.stop();
      break;
  }
  this->rcvComplete = false;
}

void FaceRec::sendCommand(uint8_t command, uint8_t param1, uint8_t param2){
  byte Output[12];
  byte parityCheck = 0x00;

  Output[0] = SYNCWORD1;
  Output[1] = SYNCWORD2;
  Output[2] = command;

  for(int i = 3; i < 6; i++){
    Output[i] = 0x00;
  }

  uint8_t outputLen = 7;
  
  switch(command){
    case IDENT:
    case REGIS:
    case CLR:
      Output[6] = 0x00; 
      break;
    case DEL:
      Output[6] = 0x02;
      Output[7] = param1;
      Output[8] = param2;
      outputLen+=2;
      break;
    case BACKLIT:
    case DISPL:
    case FLSH:
      Output[6] = 0x01;
      Output[7] = param1;
      outputLen+=1;
  }

  for(int i=2; i < outputLen; i++){
    parityCheck+=Output[i];
  }

  Output[outputLen] = parityCheck;
  outputLen++;

  for(int i=0; i < outputLen; i++){
    Serial1.write(Output[i]);
  }
}

void FaceRec::sendBattery(){
  uint32_t value = 0;
  int rounds = 11;
  esp_adc_cal_characteristics_t adc_chars;

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

  for(int i=1; i<=rounds; i++) {
    value += adc1_get_raw(ADC1_CHANNEL_6);
  }
  value /= (uint32_t)rounds;

  float state = esp_adc_cal_raw_to_voltage(value, &adc_chars)*2.0/1000.0;

  bthome.resetMeasurement();
  if(state < 3.2f){
    bthome.addMeasurement_state(STATE_BATTERY_LOW, STATE_ON);
  } else {
    bthome.addMeasurement_state(STATE_BATTERY_LOW, STATE_OFF);
  }
  bthome.sendPacket();
  delay(1500);
  bthome.stop();
}

void FaceRec::BTHomeInit(String device, bool encrypt, String bind_key){
  bthome.begin(device, encrypt, bind_key);
  bthome.resetMeasurement();
}