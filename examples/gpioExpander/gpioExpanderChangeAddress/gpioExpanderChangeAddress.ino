// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с модулем Slot Expander (I²C IO)
#include <GpioExpander.h>
// создаём объект expander класса GpioExpander по адресу 42
GpioExpander expander(42);

void setup() {
  // включаем шину I²C
  Wire.begin();
  // начало работы с модулем expander
  expander.begin();
  // меняем адрес модуля на «43»
  expander.changeAddr(45);
  // ждём 100 мс
  delay(100);
  // сохраняем адрес во Flash-памяти контроллера
  expander.saveAddr();
}

void loop() {

}
