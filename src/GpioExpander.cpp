#include "GpioExpander.h"
#include <Wire.h>

GpioExpander::GpioExpander(uint8_t i2cAddress) {
    _i2cAddress = i2cAddress;
}

void GpioExpander::begin() {
    begin(&Wire);
}

void GpioExpander::begin(TwoWire* wire) {
    _wire = wire;
}

void GpioExpander::reset() {
    writeCmd(RESET_SLAVE);
}

void GpioExpander::pinMode(int pin, uint8_t mode) {
    uint16_t sendData = 1<<pin;
    pinModePort(sendData, mode);
}

void GpioExpander::digitalWrite(int pin, bool value) {
    uint16_t sendData = 1<<pin;
    if (value) {
        writeCmd16BitData(DIGITAL_WRITE_HIGH, sendData);
    } else {
        writeCmd16BitData(DIGITAL_WRITE_LOW, sendData);
    }
}

uint16_t GpioExpander::mapResolution(uint16_t value, uint8_t from, uint8_t to)
{
  if (from == to) {
    return value;
  }
  if (from > to) {
    return value >> (from-to);
  }
  return value << (to-from);
}

void GpioExpander::analogWriteResolution(uint8_t res) {
    _analogWriteResolution = res;
}

void GpioExpander::analogReadResolution(uint8_t res) {
    _analogReadResolution = res;
}

void GpioExpander::analogWrite(int pin, uint16_t pulseWidth) {
    uint16_t val = mapResolution(pulseWidth, _analogWriteResolution, 16);
    writeCmdPin16Val(ANALOG_WRITE, (uint8_t)pin, val, true);
}

int GpioExpander::digitalRead(int pin) {
    int result = digitalReadPort();
    if (result >= 0) {
        result = ((result & (1<<pin)) ? 1 : 0); //:)
    }
    return result;
}

int GpioExpander::analogRead(int pin) {
    writeCmdPin(ANALOG_READ, (uint8_t)pin, true);
    int result = read16Bit();
    if (result >= 0) {
        result = (int)mapResolution((uint16_t)result, 12, _analogReadResolution);
    }
    return result;
}

void GpioExpander::changeAddr(uint8_t newAddr) {
    writeCmd8BitData(CHANGE_I2C_ADDR, newAddr);
    _i2cAddress = newAddr;
}

void GpioExpander::changeAddrWithUID(uint8_t newAddr) {
    uint32_t uid = getUID();

    delay(1);
    // address set on class instantiation
    _wire->beginTransmission( _i2cAddress );

    _wire->write((uint8_t)MASTER_READED_UID);
    uint8_t temp;
    temp = (uid >> 24) & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);

    temp = (uid >> 16) & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);

    temp = (uid >> 8) & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);

    temp = (uid) & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);
    _wire->endTransmission();

    delay(1);

    writeCmd8BitData(CHANGE_I2C_ADDR_IF_UID_OK, newAddr);
    _i2cAddress = newAddr;

    delay(1);
}

void GpioExpander::saveAddr() {
    writeCmd(SAVE_I2C_ADDR);
}

void GpioExpander::pinModePort(uint16_t value, uint8_t mode) {
    if (mode == INPUT) {
        writeCmd16BitData(PORT_MODE_INPUT, value);
    } else if (mode == OUTPUT) {
        writeCmd16BitData(PORT_MODE_OUTPUT, value);
    } else if (mode == INPUT_PULLUP) {
        writeCmd16BitData(PORT_MODE_PULLUP, value);
    } else if (mode == INPUT_PULLDOWN) {
        writeCmd16BitData(PORT_MODE_PULLDOWN, value);
    }
}

void GpioExpander::digitalWritePort(uint16_t value) {
    writeCmd16BitData(DIGITAL_WRITE_HIGH, value);
    writeCmd16BitData(DIGITAL_WRITE_LOW, ~value);
}

int GpioExpander::digitalReadPort() {
    // check. Need it? maybe true?
    writeCmd(DIGITAL_READ, false);
    return read16Bit();
}

void GpioExpander::pwmFreq(uint16_t freq) {
    writeCmd16BitData(PWM_FREQ, freq);
}

uint32_t GpioExpander::getUID() {
    writeCmd(UID);
    return read32bit();
}

void GpioExpander::adcSpeed(uint8_t speed) {
    // speed must be < 8. Smaller is faster, but dirty
    writeCmd8BitData(ADC_SPEED, speed);
}

void GpioExpander::adcFilter(bool enable) {
    IOcommand command = enable ? ADC_LOWPASS_FILTER_ON : ADC_LOWPASS_FILTER_OFF;
    writeCmd(command);
}

void GpioExpander::setEncoderPins(uint8_t encoder, uint8_t pinA, uint8_t pinB) {
    uint8_t pins = (pinA<<4) | (pinB & 0x0f);
    uint16_t payload = ((uint16_t)encoder << 8) | pins;
    writeCmd16BitData(ENCODER_SET_PINS, payload);
}

int8_t GpioExpander::readEncoderDiff(uint8_t encoder) {
    int8_t result = 0;
    writeCmdPin(ENCODER_GET_DIFF_VALUE, encoder, true);
    result = readInt8Bit();
    return result;
}

void GpioExpander::writeCmdPin(IOcommand command, uint8_t pin, bool sendStop) {
    _wire->beginTransmission( _i2cAddress );
    _wire->write((uint8_t)command);
    _wire->write(pin);
    _wire->endTransmission(sendStop);
}

void GpioExpander::writeCmdPin16Val(IOcommand command, uint8_t pin, uint16_t value, bool sendStop) {
    _wire->beginTransmission( _i2cAddress );
    _wire->write((uint8_t)command);
    _wire->write(pin);
    uint8_t temp;
    temp = (value >> 8) & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);
    temp = value & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);
    _wire->endTransmission(sendStop);
}


void GpioExpander::writeCmd16BitData(IOcommand command, uint16_t data) {
    // address set on class instantiation
    _wire->beginTransmission( _i2cAddress );
    _wire->write((uint8_t)command);
    uint8_t temp;
    temp = (data >> 8) & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);
    temp = data & 0xff;
    // data/setting to be sent to device
    _wire->write(temp);
    _wire->endTransmission();
}

void GpioExpander::writeCmd8BitData(IOcommand command, uint8_t data) {
    // address set on class instantiation
    _wire->beginTransmission( _i2cAddress );
    _wire->write((uint8_t)command);
    // data/setting to be sent to device
    _wire->write(data);
    _wire->endTransmission();
}

void GpioExpander::writeCmd(IOcommand command, bool sendStop) {
    _wire->beginTransmission( _i2cAddress );
    _wire->write((uint8_t)command);
    _wire->endTransmission(sendStop);
}

int8_t GpioExpander::readInt8Bit() {
    int result = 0;
    uint8_t byteCount = 1;
    _wire->requestFrom(_i2cAddress, byteCount);
    uint16_t counter = 0xffff;
    while (_wire->available() < byteCount) {
        if (!(--counter))
            return result;
    }
    result = (int8_t)_wire->read();
    return result;
}

int GpioExpander::read16Bit() {
    int result = -1;
    uint8_t byteCount = 2;
    _wire->requestFrom(_i2cAddress, byteCount);
    uint16_t counter = 0xffff;
    while (_wire->available() < byteCount)
    {
        delay(0);
        if (!(--counter))
            return result;
    }
    result = _wire->read();
    result <<= 8;
    result |= _wire->read();
    return result;
}

uint32_t GpioExpander::read32bit() {
    // https://www.youtube.com/watch?v=y73hyMP1a-E
    uint32_t result = 0xffffffff;
    uint8_t byteCount = 4;
    _wire->requestFrom(_i2cAddress, byteCount);
    uint16_t counter = 0xffff;

    while (_wire->available() < byteCount)
    {
        delay(0);
        if (!(--counter))
            return result;
    }

    result = 0;
    for (uint8_t i = 0; i < byteCount - 1; ++i) {
      result |= _wire->read();
      result <<= 8;
    }
    result |= _wire->read();
    return result;
}
