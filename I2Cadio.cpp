#include "ADIO.h"
#include <Wire.h>

void ADIO::writeCmdPinVal(IOcommand command, uint8_t pin, uint8_t value, bool sendStop)
{
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.write(pin);
    Wire.write(value);
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
    Wire.requestFrom(_i2caddress, 2);
    uint16_t counter = 0xff;
    while (Wire.available()!=2)
    {
        if (!(--counter))
            return result;
    }
    result = Wire.read();
    result <<= 8;
    result |= Wire.read();
    return result;
}

ADIO::ADIO(uint8_t i2caddress)
{
    _i2caddress = i2caddress;
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

int ADIO::digitalRead(int pin)
{
    int result = -1;
    writeCmd(DIGITAL_READ, false);
    int answer = read16Bit();
    if (answer >= 0) {
        result = ((answer & (1<<pin))? 1 : 0); //:)
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
    }
}

void ADIO::analogWrite(int pin, uint8_t pulseWidth)
{
    writeCmdPinVal(ANALOG_WRITE, (uint8_t)pin, pulseWidth, true);
}

int ADIO::analogRead(int pin, uint8_t avgCount)
{
    writeCmdPinVal(ANALOG_READ, (uint8_t)pin, avgCount, false);
    return read16Bit();
}

void ADIO::pwmFreq(uint16_t freq)
{
    writeCmd16BitData(PWM_FREQ, freq);
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