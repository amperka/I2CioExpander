#include <Arduino.h>
#include <Wire.h>
#include "i2cioCommands.h"

#ifndef _GPIO_EXPANDER_
#define _GPIO_EXPANDER_

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN 0x3
#endif

#ifndef DEFAULT_GPIOEXP_ADDR
#define DEFAULT_GPIOEXP_ADDR 42
#endif

#ifndef I2CIO_PIN_COUNT
#define I2CIO_PIN_COUNT 10
#endif

// we need to say something to identity... Let's say "slot"
#define IS_HE_SAY_SLOT (uint32_t)(((uint32_t)'s' << 24) | ((uint32_t)'l' << 16) | ((uint32_t)'o' << 8) | ((uint32_t)'t'))

class GpioExpander 
{
public:
    GpioExpander(uint8_t i2caddress);
    void begin();
    void begin(TwoWire* wire);
    void digitalWrite(int pin, bool value);
    void pinMode(int pin, uint8_t mode);
    void analogWrite(int pin, uint8_t pulseWidth);
    void analogWrite_16(int pin, uint16_t pulseWidth);
    void pwmFreq(uint16_t freq);
    void changeAddr(uint8_t newAddr);
    void changeAddrWithUID(uint8_t newAddr);
    void saveAddr();
    void reset();
    int  digitalRead(int pin);
    int  analogRead(int pin);//, uint8_t avgCount = 2);
    uint32_t getUID();
    int digitalReadPort();
    void digitalWritePort(uint16_t value);
    void pinModePort(uint16_t value, uint8_t mode);
    void adcSpeed(uint8_t speed);
    void adcFilter(bool enable);
    void setEncoderPins(uint8_t encoder, uint8_t pinA, uint8_t pinB);
    int8_t readEncoderDiff(uint8_t encoder);

private:
    TwoWire* _wire;
    uint8_t _i2caddress;
    void writeCmdPin(IOcommand command, uint8_t pin, bool sendStop = true);
    void writeCmdPin16Val(IOcommand command, uint8_t pin, uint16_t value, bool sendStop = true);
    void writeCmd16BitData(IOcommand command, uint16_t data);
    void writeCmd8BitData(IOcommand command, uint8_t data);
    void writeCmd(IOcommand command, bool sendStop = true);
    int  read16Bit();
    int8_t readInt8Bit();
    uint32_t read32bit();
};

#endif //_GPIO_EXPANDER_
