#ifndef DACLIST_H
#define DACLIST

#include <Adafruit_MCP4725.h>
#include "tcaSelect.hpp"
#include <stdint.h>

#define DAC_MAX 5

struct DacEntry {
    Adafruit_MCP4725 dac;
    uint16_t value;
    int index;
};

class DacList {
    public:
        DacList(): entryCount(0) {}

        bool addDac(const Adafruit_MCP4725& dac, uint16_t value, int index){
            if (entryCount >= DAC_MAX){
                return false; // Full
            }
            entries[entryCount].dac = dac;
            entries[entryCount].value = value;
            entries[entryCount].index = index;
            tcaSelect(index);
            entries[entryCount].dac.begin();
            entryCount++;
            return true;
        }

        size_t size() const {
            return entryCount;
        }

        DacEntry& operator[](size_t index){
            return entries[index];
        }

        const DacEntry& operator[](size_t index) const {
            return entries[index];
        }
    
    private:
        DacEntry entries[DAC_MAX];
        size_t entryCount;
};

#endif