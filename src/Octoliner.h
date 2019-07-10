/*
 * This file is a part of Dragster car set library.
 *
 * Defines: octoliner object - 8-point line-folowing sensor driver
 * © Amperka LLC (https://amperka.com, dev@amperka.com)
 * 
 * Author: Vasily Basalaev <vasily@amperka.ru>
 * Refactored by: Yury Botov <by@amperka.com>
 * License: GPLv3, all text here must be included in any redistribution.
 */

#include "GpioExpander.h"
#include <Arduino.h>
#include <Wire.h>

#ifndef __OCTOLINER_H__
#define __OCTOLINER_H__

class Octoliner : public GpioExpander {
public:
    Octoliner(uint8_t i2caddress = 42);
    void begin();
    void begin(TwoWire* wire);
    void setSensitivity(uint8_t sense);
    void setBrightness(uint8_t brightness);
    int analogRead(uint8_t sensor);
    uint8_t getBinaryLine(); // fast and dirty
    uint8_t getBinaryLine(uint16_t treshold); //slow and high accuracy
    float mapLine(uint8_t binaryLine);
    float mapLine(int binaryLine[8]);

private:
    const uint8_t _ledBrightnessPin = 9;
    const uint8_t _sensePin = 0;
    const uint8_t _sensorPinMap[8] = {
        4, 5, 6, 8, 7, 3, 2, 1
    };
    float _value;
};

#endif // __OCTOLINER_H__