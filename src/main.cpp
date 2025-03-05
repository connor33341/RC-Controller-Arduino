#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "i2cScanner.h"

#define NeutralValue 2.2
#define Vref 5

i2cScanner scanner;

Adafruit_MCP4725 dacThrottle; // 0x62
uint16_t dacThrottleVoltage = 0;

void setNormalized(Adafruit_MCP4725 dac,uint16_t voltage){
  voltage = min(voltage,4095);
  uint16_t dacValue = (voltage*4096)/Vref;
  dac.setVoltage(dacValue,false);
}

void setup() {
  Serial.begin(9600); // Prob should increase the rate
  while (!Serial){}
  scanner.begin();
  Serial.println("INITALIZED");
  dacThrottle.begin(0x62);
  //setNormalized(dacThrottle,0);
}

void loop() {
  scanner.scan();
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
    dacThrottleVoltage = (uint16_t) doc["throttle"];
  }
  setNormalized(dacThrottle,dacThrottleVoltage);
  delay(5000);
}
// Example: {"throttle":4.2}
