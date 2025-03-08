#include <Wire.h>
#ifndef TCASELECT_H
#define TCASELECT_H
#define TCAADR 0x70

void tcaSelect(int channel) {
    if (channel > 7) return;
    Wire.beginTransmission(TCAADR);
    Wire.write(1 << channel);
    Wire.endTransmission();
}

#endif