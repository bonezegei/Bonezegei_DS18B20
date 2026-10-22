/*
  DS18B20 Library
  Author: Bonezegei (Jofel Batutay)
  Date : October 22, 2026
*/
#ifndef BONEZEGEI_DS18B20_H
#define BONEZEGEI_DS18B20_H

#include <Arduino.h>

class Bonezegei_DS18B20 {
public:
    explicit Bonezegei_DS18B20(uint8_t pin);

    void begin();
    bool read();
    float getTemperatureC();
    float getTemperatureF();

private:
    uint8_t _pin;
    float _tempC;

    // Built-in OneWire driver methods
    uint8_t owReset();
    void owWriteBit(uint8_t v);
    uint8_t owReadBit();
    void owWrite(uint8_t v);
    uint8_t owRead();
    uint8_t owCrc8(const uint8_t *addr, uint8_t len);
};

#endif