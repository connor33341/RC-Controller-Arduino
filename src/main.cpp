#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "i2cScanner.h"
#include "dacList.h"
#include "tcaSelect.hpp"

#define NeutralValue 2.2
#define Vref 4.8

i2cScanner scanner;
DacList dacList;

Adafruit_MCP4725 dacThrottle; // 0x62
Adafruit_MCP4725 dacRudder; //0x63
Adafruit_MCP4725 dacAileron; //0x64
Adafruit_MCP4725 dacElevator;

void setNormalized(Adafruit_MCP4725 dac,uint16_t voltage, int dacIndex){
  tcaSelect(dacIndex);
  voltage = min(voltage,4095);
  uint16_t dacValue = (voltage*4096)/Vref;
  dac.setVoltage(dacValue,false);
}

void setAll(){
  for (size_t i = 0; i < dacList.size(); i++){
    setNormalized(dacList[i].dac,dacList[i].value,dacList[i].index);
  }
}

void setup() {
  Serial.begin(115200); // Prob should increase the rate
  while (!Serial){}
  scanner.begin();
  Wire.begin();
  Serial.println("INITALIZED");
  dacList.addDac(dacThrottle,0,2);
  dacList.addDac(dacRudder,NeutralValue,1);
  dacList.addDac(dacAileron,NeutralValue,3);
  dacList.addDac(dacElevator,NeutralValue,4);
  /*
  tcaSelect(0);
  dacThrottle.begin();
  tcaSelect(1);
  dacRudder.begin();
  tcaSelect(2);
  dacFlaps.begin();
  dacList.addDac(dacThrottle,0);
  dacList.addDac(dacRudder,NeutralValue);
  dacList.addDac(dacFlaps,NeutralValue);
  */
  //start at 1, to skip the throttle
  for (size_t i=1; i < dacList.size(); i++){
    tcaSelect(dacList[i].index);
    dacList[i].dac.setVoltage(NeutralValue,false);
  }
}

void loop() {
  //scanner.scan(); //0x70
  if (Serial.available() > 0){
    String jsonString = Serial.readStringUntil('\n');
    Serial.println(jsonString);
    Serial.println("JSON Data Recv");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc,jsonString);
    if (error){
      Serial.print("Failed to Parse JSON: ");
      Serial.println(error.c_str());
      return;
    }
    if ((bool) doc["scan"] == true){
      scanner.scan();
    };
    dacList[0].value = (uint16_t) doc["throttle"];
    dacList[1].value = (uint16_t) doc["rudder"];
    dacList[2].value = (uint16_t) doc["aileron"];
    dacList[3].value = (uint16_t) doc["elevator"];
  }
  //setNormalized(dacThrottle,dacList[0].value);
  setAll();
  //delay(5000);
}
// Packet: {"scan":true,"throttle":0,"rudder":2.2}
