/*
  DS18B20 Library
  Author: Bonezegei (Jofel Batutay)
  Date : October 22, 2026
*/

#include "Bonezegei_DS18B20.h"

Bonezegei_DS18B20::Bonezegei_DS18B20(uint8_t pin) {
    _pin = pin;
    _tempC = -127.0;
}

void Bonezegei_DS18B20::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

bool Bonezegei_DS18B20::read() {
    if (!owReset()) return false;

    owWrite(0xCC); // Skip ROM
    owWrite(0x44); // Convert Temperature

    delay(750); // 12-bit conversion delay

    if (!owReset()) return false;
    owWrite(0xCC); // Skip ROM
    owWrite(0xBE); // Read Scratchpad

    uint8_t data[9];
    for (uint8_t i = 0; i < 9; i++) {
        data[i] = owRead();
    }

    if (owCrc8(data, 8) != data[8]) {
        return false;
    }

    int16_t raw = (data[1] << 8) | data[0];
    _tempC = (float)raw / 16.0;

    return true;
}

float Bonezegei_DS18B20::getTemperatureC() {
    return _tempC;
}

float Bonezegei_DS18B20::getTemperatureF() {
    return (_tempC * 1.8) + 32.0;
}

// Built-in OneWire Primitive Routines
uint8_t Bonezegei_DS18B20::owReset() {
    pinMode(_pin, INPUT_PULLUP);
    delayMicroseconds(2);
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(480);
    pinMode(_pin, INPUT_PULLUP);
    delayMicroseconds(70);
    uint8_t presence = !digitalRead(_pin);
    delayMicroseconds(410);
    return presence;
}

void Bonezegei_DS18B20::owWriteBit(uint8_t v) {
    noInterrupts();
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    if (v & 1) {
        delayMicroseconds(6);
        pinMode(_pin, INPUT_PULLUP);
        delayMicroseconds(64);
    } else {
        delayMicroseconds(60);
        pinMode(_pin, INPUT_PULLUP);
        delayMicroseconds(10);
    }
    interrupts();
}

uint8_t Bonezegei_DS18B20::owReadBit() {
    noInterrupts();
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(6);
    pinMode(_pin, INPUT_PULLUP);
    delayMicroseconds(9);
    uint8_t r = digitalRead(_pin);
    delayMicroseconds(55);
    interrupts();
    return r;
}

void Bonezegei_DS18B20::owWrite(uint8_t v) {
    for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
        owWriteBit((bitMask & v) ? 1 : 0);
    }
}

uint8_t Bonezegei_DS18B20::owRead() {
    uint8_t r = 0;
    for (uint8_t bitMask = 0x01; bitMask != 0; bitMask <<= 1) {
        if (owReadBit()) r |= bitMask;
    }
    return r;
}

uint8_t Bonezegei_DS18B20::owCrc8(const uint8_t *addr, uint8_t len) {
    uint8_t crc = 0;
    while (len--) {
        uint8_t inbyte = *addr++;
        for (uint8_t i = 8; i; i--) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}