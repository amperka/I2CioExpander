// библиотека для работы с модулями I²C
#include <Wire.h>
// библиотека для работы cо сборкой датчиков линии
#include <Octoliner.h>

// пин подключение потенциометра регулировки чувствительности приёмника
#define PIN_SENSITIVITY   A0
// пин подключение потенциометра регулировки яркость ИК-светодиодов
#define PIN_BRIGHTNESS    A1

// создаём объект для работы с датчиками линии
Octoliner octoliner(42);

void setup() {
  // открываем Serial-порт
  Serial.begin(115200);
  // начало работы с модулями I²C
  Wire.begin();
  // начало работы с датчиками линии
  octoliner.begin();
}

void loop() {
  // pсчитываем значение с потенциометров
  int sensitivity = map(analogRead(PIN_SENSITIVITY), 0, 1024, 0, 255);
  int brightness = map(analogRead(PIN_BRIGHTNESS), 0, 1024, 0, 255);
  // выставляем чувствительность фотоприёмников
  octoliner.setSensitivity(sensitivity);
  // выставляем яркость свечения ИК-светодиодов
  octoliner.setBrightness(brightness);
  
  // считываем значение с датчиков линии
  for (int i = 0; i < 8; i++) {
    // преобразуем диапазон 0…4096 -> 0…32
    int sharpCount = octoliner.analogRead(i) >> 7;
    // выводим положение линии в графическом виде
    for (int y = 0; y <= sharpCount; y++) {
      Serial.print('#');
    }
    Serial.println();
  }
  // вставляем пустые строки для форматирвоания вывода
  Serial.println("\r\n\r\n");
  delay(100);
}
