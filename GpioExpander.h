#include <arduino.h>

#ifndef _GPIO_EXPANDER_
#define _GPIO_EXPANDER_

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN 0x3
#endif
//INPUT_PULLDOWN

#ifndef DEFAULT_GPIOEXP_ADDR
#define DEFAULT_GPIOEXP_ADDR 42
#endif 
// DEFAULT_GPIOEXP_ADDR

enum IOcommand {
      WHO_AM_I //Отдали UID
    , RESET // сброс
    , CHANGE_I2C_ADDR // сменить I2C-адрес вручную
    , SAVE_I2C_ADDR // Сохранить текущий адрес во флэш, чтобы стартовать при последующих включениях с него
    , PORT_MODE_INPUT // настроили пины на вход
    , PORT_MODE_PULLUP // .. вход с поддтяжкой вверх
    , PORT_MODE_PULLDOWN // .. вниз
    , PORT_MODE_OUTPUT // .. на выход
    , DIGITAL_READ    // считали состояние виртуального порта
    , DIGITAL_WRITE_HIGH // Выставили пины виртуального порта в высокий уровень
    , DIGITAL_WRITE_LOW // .. в низкий уровень
    , ANALOG_WRITE // Запустить ШИМ
    , ANALOG_READ // Считать значениие с АЦП
    , PWM_FREQ // установка частоты ШИМ (общая для всех PWM-пинов)
    , ADC_SPEED
    , SEND_MASTER_READED_UID
    , CHANGE_I2C_ADDR_IF_UID_OK
};

class GpioExpander 
{
public:
    GpioExpander(uint8_t expCount);
    void digitalWrite(int pin, bool value);
    void pinMode(int pin, uint8_t mode);
    void analogWrite(int pin, uint8_t pulseWidth);
    void analogWrite_16(int pin, uint16_t pulseWidth);
    void pwmFreq(uint8_t expNum, uint16_t freq);
    void changeAddr(uint8_t oldAdddr, uint8_t newAddr);
    void changeAddrWithUID(uint8_t newAddr);
    void saveAddr();
    void reset();
    int  digitalRead(int pin);
    int  analogRead(int pin);//, uint8_t avgCount = 2);
    uint32_t getUID(uint8_t addr = DEFAULT_GPIOEXP_ADDR);
    int digitalReadPort(uint8_t expNum);
    void digitalWritePort(uint8_t expNum, uint16_t value);
    void pinModePort(uint8_t expNum, uint16_t value, uint8_t mode);
    void adcSpeed(uint8_t expNum, uint8_t speed);
    void begin();

private:
//    uint8_t _i2caddress;
    uint8_t *_i2cAddr;
    uint8_t _expCount;
    uint8_t _lastExpNum = 0;
    void writeCmdPin(uint8_t _i2caddress, IOcommand command, uint8_t pin, bool sendStop = true);
    void writeCmdPin16Val(uint8_t _i2caddress, IOcommand command, uint8_t pin, uint16_t value, bool sendStop = true);
    void writeCmd16BitData(uint8_t _i2caddress, IOcommand command, uint16_t data);
    void writeCmd8BitData(uint8_t _i2caddress, IOcommand command, uint8_t data);
    void writeCmd(uint8_t _i2caddress, IOcommand command, bool sendStop = true);
    int  read16Bit(uint8_t _i2caddress);
    uint32_t read32bit(uint8_t _i2caddress);
    uint8_t expNumToAddr(uint8_t expNum);
    uint8_t pinToAddr(int pin);
    

};

#endif //_GPIO_EXPANDER_
