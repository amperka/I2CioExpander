#include "GpioExpander.h"
#include <Wire.h>

void GpioExpander::writeCmdPin(uint8_t _i2caddress, IOcommand command, uint8_t pin, bool sendStop)
{
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.write(pin);
    Wire.endTransmission(sendStop);
}

void GpioExpander::writeCmdPin16Val(uint8_t _i2caddress, IOcommand command, uint8_t pin, uint16_t value, bool sendStop)
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


void GpioExpander::writeCmd16BitData(uint8_t _i2caddress, IOcommand command, uint16_t data)
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

void GpioExpander::writeCmd8BitData(uint8_t _i2caddress, IOcommand command, uint8_t data)
{
    Wire.beginTransmission( _i2caddress ); // Address set on class instantiation
    Wire.write((uint8_t)command);
    Wire.write(data); // Data/setting to be sent to device
    Wire.endTransmission();
}

void GpioExpander::writeCmd(uint8_t _i2caddress, IOcommand command, bool sendStop)
{
    Wire.beginTransmission( _i2caddress );
    Wire.write((uint8_t)command);
    Wire.endTransmission(sendStop);
}

int GpioExpander::read16Bit(uint8_t _i2caddress)
{
    int result = -1;
    uint8_t byteCount = 2;
    Wire.requestFrom(_i2caddress, byteCount);
    uint16_t counter = 0xffff;
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

uint32_t GpioExpander::read32bit(uint8_t _i2caddress)
{
    uint32_t result = 0xffffffff; // https://www.youtube.com/watch?v=y73hyMP1a-E
    uint8_t byteCount = 4;
    Wire.requestFrom(_i2caddress, byteCount);
    uint16_t counter = 0xffff;

    while (Wire.available() < byteCount)
    {
        if (!(--counter))
            return result;
    }

    result = 0;
    for (uint8_t i = 0; i < byteCount-1; ++i) {
      result |= Wire.read();
      result <<= 8;
    }
    result |= Wire.read();
    return result;
}

GpioExpander::GpioExpander(uint8_t expCount)
{
//    _i2caddress = i2caddress;

    _expCount = (expCount > 127) ? 127 : expCount;
    _i2cAddr = new uint8_t [_expCount];
}

uint8_t GpioExpander::expNumToAddr(uint8_t expNum)
{
    if (expNum < _expCount)
        return _i2cAddr[expNum];
    else
        return 0;
}

uint8_t GpioExpander::pinToAddr(int pin)
{
    uint8_t expNum = pin/10;
    return expNumToAddr(expNum);
}

void GpioExpander::digitalWritePort(uint8_t expNum, uint16_t value)
{
    uint8_t addr = expNumToAddr(expNum);
    writeCmd16BitData(addr, DIGITAL_WRITE_HIGH, value);
    writeCmd16BitData(addr, DIGITAL_WRITE_LOW, ~value);
}

void GpioExpander::digitalWrite(int pin, bool value)
{
    uint8_t addr = pinToAddr(pin);
    uint16_t sendData = 1<<(pin % 10);
    if (value) {
        writeCmd16BitData(addr, DIGITAL_WRITE_HIGH, sendData);
    } else {
        writeCmd16BitData(addr, DIGITAL_WRITE_LOW, sendData);
    }
}

int GpioExpander::digitalReadPort(uint8_t expNum)
{
    uint8_t addr = expNumToAddr(expNum);
    writeCmd(addr, DIGITAL_READ, false);
    return read16Bit(addr);
}

int GpioExpander::digitalRead(int pin)
{
    int result = digitalReadPort(expNumToAddr(pin/10));
    if (result >= 0) {
        result = ((result & (1<<(pin % 10)))? 1 : 0); //:)
    }
    return result;
}

void GpioExpander::pinModePort(uint8_t expNum, uint16_t value, uint8_t mode)
{
    uint8_t addr = expNumToAddr(expNum);
    if (mode == INPUT) {
        writeCmd16BitData(addr, PORT_MODE_INPUT, value);
    } else if (mode == OUTPUT) {
        writeCmd16BitData(addr, PORT_MODE_OUTPUT, value);
    } else if (mode == INPUT_PULLUP) {
        writeCmd16BitData(addr, PORT_MODE_PULLUP, value);
    } else if (mode == INPUT_PULLDOWN) {
        writeCmd16BitData(addr, PORT_MODE_PULLDOWN, value);
    }

}

void GpioExpander::pinMode(int pin, uint8_t mode)
{
    uint16_t sendData = 1<<(pin % 10);
    pinModePort(expNumToAddr(pin/10), sendData, mode);

}

void GpioExpander::analogWrite_16(int pin, uint16_t pulseWidth)
{
    writeCmdPin16Val(pinToAddr(pin), ANALOG_WRITE, (uint8_t)(pin % 10), pulseWidth, true);
}

void GpioExpander::analogWrite(int pin, uint8_t pulseWidth)
{
    uint16_t val = map(pulseWidth, 0, 255, 0, 65535);
    writeCmdPin16Val(pinToAddr(pin), ANALOG_WRITE, (uint8_t)(pin % 10), val, true);
}

int GpioExpander::analogRead(int pin)
{
    uint8_t addr = pinToAddr(pin);
    writeCmdPin(addr, ANALOG_READ, (uint8_t)(pin % 10), true);
    return read16Bit(addr);
}

void GpioExpander::pwmFreq(uint8_t expNum, uint16_t freq)
{
    writeCmd16BitData(expNumToAddr(expNum), PWM_FREQ, freq);
}

void GpioExpander::adcSpeed(uint8_t expNum, uint8_t speed)
{
    // speed must be < 8. Smaller is faster, but dirty
    writeCmd8BitData(expNumToAddr(expNum), ADC_SPEED, speed);
}

void GpioExpander::changeAddr(uint8_t oldAdddr, uint8_t newAddr)
{
    writeCmd8BitData(oldAdddr, CHANGE_I2C_ADDR, newAddr);
//    _i2caddress = newAddr;
}

void GpioExpander::changeAddrWithUID(uint8_t newAddr)
{
    uint32_t uid = getUID();

    delay(0);

    Wire.beginTransmission( DEFAULT_GPIOEXP_ADDR ); // Address set on class instantiation

    Wire.write((uint8_t)SEND_MASTER_READED_UID);
    uint8_t temp;
    temp = (uid >> 24) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device

    temp = (uid >> 16) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device

    temp = (uid >> 8) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device

    temp = (uid) & 0xff;
    Wire.write(temp); // Data/setting to be sent to device
    Wire.endTransmission();

    delay(1);

    writeCmd8BitData(DEFAULT_GPIOEXP_ADDR, CHANGE_I2C_ADDR_IF_UID_OK, newAddr);
//    _i2caddress = newAddr;

    delay(1);
}
void GpioExpander::begin()
{
    uint8_t newAddr = 0;
    for (uint8_t i = 0; i < _expCount; ++i) {
        do {
            ++newAddr;
            Wire.beginTransmission(newAddr);
        } while (Wire.endTransmission() == 0);

        changeAddrWithUID(newAddr);
        _i2cAddr[i] = newAddr;
    }
}

void GpioExpander::saveAddr()
{
    //TODO
    for (int i = 0; i < _expCount; ++i)
    {
        writeCmd(expNumToAddr(i), SAVE_I2C_ADDR);
    }
}

void GpioExpander::reset()
{
    //TODO
    for (int i = 0; i < _expCount; ++i)
    {
        writeCmd(i, RESET);
    }
}

uint32_t GpioExpander::getUID(uint8_t addr)
{
    writeCmd(addr, WHO_AM_I);
    return read32bit(addr);
}
