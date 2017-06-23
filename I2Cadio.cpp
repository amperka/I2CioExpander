#include "I2Cadio.h"
#include <Wire.h>

void ADIO::writeCmdPin(IOcommand command, uint8_t pin, bool sendStop)
{
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.write(pin);
    Wire.endTransmission(sendStop);
}

void ADIO::writeCmdPin16Val(IOcommand command, uint8_t pin, uint16_t value, bool sendStop)
{
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.write(pin);
    uint8_t temp;
    temp = (value >> 8) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    temp = value & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    Wire.endTransmission(sendStop);
}


void ADIO::writeCmd16BitData(IOcommand command, uint16_t data)
{
    Wire.beginTransmission( _i2caddress ); // Address set on class instantiation
    Wire.write((uint8_t)command);
    uint8_t temp;
    temp = (data >> 8) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    temp = data & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    Wire.endTransmission();
}

void ADIO::writeCmd8BitData(IOcommand command, uint8_t data)
{
    Wire.beginTransmission( _i2caddress ); // Address set on class instantiation
    Wire.write((uint8_t)command);
    Wire.write(data); // Data/setting to be sent to device
    Wire.endTransmission();
}

void ADIO::writeCmd(IOcommand command, bool sendStop)
{
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.endTransmission(sendStop);
}

int ADIO::read16Bit()
{
    int result = -1;
    uint8_t byteCount = 2;
    Wire.requestFrom(_i2caddress, byteCount);
    uint16_t counter = 0xff;
    while (Wire.available() < byteCount)
    {
        if (!(--counter))
            return result;
    }
    result = Wire.read();
    result <<= 8;
    result |= Wire.read();
    return result;
}

uint32_t ADIO::read32bit()
{
    uint32_t result = 0xffffffff; // https://www.youtube.com/watch?v=y73hyMP1a-E
    uint8_t byteCount = 4;
    Wire.requestFrom(_i2caddress, byteCount);
    uint16_t counter = 0xff;
    while (Wire.available() < byteCount)
    {
        if (!(--counter))
            return result;
    }
    for (uint8_t i = 0; i < 3; ++i) {
      result = Wire.read();
      result <<= 8;
    }
    result |= Wire.read();
    return result;
}

ADIO::ADIO(uint8_t i2caddress)
{
    _i2caddress = i2caddress;
}

void ADIO::digitalWritePort(uint16_t value)
{
    writeCmd16BitData(DIGITAL_WRITE_HIGH, value);
    writeCmd16BitData(DIGITAL_WRITE_LOW, ~value);
}

void ADIO::digitalWrite(int pin, bool value)
{
    uint16_t sendData = 1<<pin;
    if (value) {
        writeCmd16BitData(DIGITAL_WRITE_HIGH, sendData);
    } else {
        writeCmd16BitData(DIGITAL_WRITE_LOW, sendData);
    }
}

int ADIO::digitalReadPort()
{
    writeCmd(DIGITAL_READ, false);
    return read16Bit();
}

int ADIO::digitalRead(int pin)
{
    int result = digitalReadPort();
    if (result >= 0) {
        result = ((result & (1<<pin))? 1 : 0); //:)
    }
    return result;
}

void ADIO::pinMode(int pin, uint8_t mode)
{
    uint16_t sendData = 1<<pin;
    if (mode == INPUT) {
        writeCmd16BitData(PORT_MODE_INPUT, sendData);
    } else if (mode == OUTPUT) {
        writeCmd16BitData(PORT_MODE_OUTPUT, sendData);
    } else if (mode == INPUT_PULLUP) {
        writeCmd16BitData(PORT_MODE_PULLUP, sendData);
    } else if (mode == INPUT_PULLDOWN) {
        writeCmd16BitData(PORT_MODE_PULLDOWN, sendData);
    }

}

void ADIO::analogWrite_16(int pin, uint16_t pulseWidth)
{
    writeCmdPin16Val(ANALOG_WRITE, (uint8_t)pin, pulseWidth, true);
}

void ADIO::analogWrite(int pin, uint8_t pulseWidth)
{
    uint16_t val = map(pulseWidth, 0, 255, 0, 65535);
    writeCmdPin16Val(ANALOG_WRITE, (uint8_t)pin, val, true);
}

int ADIO::analogRead(int pin)
{
    writeCmdPin(ANALOG_READ, (uint8_t)pin, true);
    return read16Bit();
}

void ADIO::pwmFreq(uint16_t freq)
{
    writeCmd16BitData(PWM_FREQ, freq);
}

void ADIO::adcSpeed(uint8_t speed)
{
    // speed must be < 8. Smaller is faster, but dirty
    writeCmd8BitData(ADC_SPEED, speed);
}

void ADIO::changeAddr(uint8_t newAddr)
{
    writeCmd8BitData(CHANGE_I2C_ADDR, newAddr);
    _i2caddress = newAddr;
}

void ADIO::saveAddr()
{
    writeCmd(SAVE_I2C_ADDR);
}

void ADIO::reset()
{
    writeCmd(RESET);
}

uint32_t ADIO::getUID()
{
    writeCmd(WHO_AM_I);
    return read32bit();
}