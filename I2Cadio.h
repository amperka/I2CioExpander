#include <arduino.h>

#ifndef _I2C_ADIO_
#define _I2C_ADIO_

enum IOcommand {
      WHO_AM_I //Отдали UID
    , RESET // сброс
    , CHANGE_I2C_ADDR // сменить I2C-адрес вручную
    , SAVE_I2C_ADDR // Сохранить текущий адрес во флэш, чтобы стартовать при последующих включениях с него
    , PORT_MODE_INPUT // настроили пины на вход
    , PORT_MODE_PULLUP // .. вход с поддтяжкой вверх
    , PORT_MODE_OUTPUT // .. на выход
    , DIGITAL_READ    // считали состояние виртуального порта
    , DIGITAL_WRITE_HIGH // Выставили пины виртуального порта в высокий уровень
    , DIGITAL_WRITE_LOW // .. в низкий уровень
    , PWM_FREQ // установка частоты ШИМ (общая для всех PWM-пинов)
    , ANALOG_WRITE // Запустить ШИМ
    , ANALOG_READ // Считать значениие с АЦП
};

class ADIO 
{
public:
    ADIO(uint8_t i2caddress);
    void digitalWrite(int pin, bool value);
    void pinMode(int pin, uint8_t mode);
    void analogWrite(int pin, uint8_t pulseWidth);
    void pwmFreq(uint16_t freq);
    void changeAddr(uint8_t newAddr);
    void saveAddr();
    void reset();
    int  digitalRead(int pin);
    int  analogRead(int pin, uint8_t avgCount = 2);

private:
    uint8_t _i2caddress;
    void writeCmdPinVal(IOcommand command, uint8_t pin, uint8_t value, bool sendStop = true);
    void writeCmd16BitData(IOcommand command, uint16_t data);
    void writeCmd8BitData(IOcommand command, uint8_t data);
    void writeCmd(IOcommand command, bool sendStop = true);
    int  read16Bit();
};

#endif //_I2C_ADIO_
