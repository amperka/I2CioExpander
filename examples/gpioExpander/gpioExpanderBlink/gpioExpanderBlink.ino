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
  // настраиваем пин «0» в режим выхода
  expander.pinMode(0, OUTPUT);
}

void loop() {
  // подаём на пин светодиода «высокий сигнал»
  expander.digitalWrite(0, HIGH);
  // ждём пол секунды
  delay(500);
  // подаём на пин светодиода «низкий сигнал»
  expander.digitalWrite(0, LOW);
  // ждём пол секунды
  delay(500);
}
