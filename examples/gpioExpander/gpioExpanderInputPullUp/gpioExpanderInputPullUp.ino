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
  // выставляем все пины в режиме выхода
  for (int i = 0; i < 9; i++) {
    expander.pinMode(i, INPUT_PULLUP);
  }
  // открываем монитор Serial-порта
  Serial.begin(115200);
}

void loop() {
  // читаем весь порт
  Serial.println(expander.digitalReadPort(), BIN);
  delay(200);
}
