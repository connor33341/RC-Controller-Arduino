#ifndef I2CSCANNER_H
#define I2CSCANNER_H
#include <Wire.h>

class i2cScanner {
  public: 
    // Constructor
    i2cScanner() {}

    void begin(){
      Wire.begin();
    }

    void scan(){
      byte error, address;
      int nDevices = 0;
      Serial.println("Scanning i2c");
      for (address = 1; address < 127; address ++){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0){
          Serial.print("Device Found at addr 0x");
          if (address < 16){
            Serial.print("0");
          }
          Serial.print(address,HEX);
          Serial.println(" ");
        } else if (error == 4){
          Serial.print("Unknown Error at addr 0x");
          if (address < 16){
            Serial.print("0");
          }
          Serial.println(address,HEX);
        }
      }
    }
};
#endif