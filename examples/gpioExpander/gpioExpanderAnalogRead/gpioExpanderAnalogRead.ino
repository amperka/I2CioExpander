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
  // открываем монитор Serial-порта
  Serial.begin(115200);
}

void loop() {
  // считываем показатели в аналогом виде
  // и выводим их в Serial-порт
  for (int i = 0; i < 9; i++) {
    Serial.print(expander.analogRead(i));
    Serial.print("\t");
  }
  Serial.println();
  delay(500);
}
