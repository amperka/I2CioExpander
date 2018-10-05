// библиотека для работы с модулями I²C
#include <Wire.h>
// библиотека для работы cо сборкой датчиков линии
#include <Octoliner.h>

// создаём объект для работы с датчиками линии
Octoliner octoliner(42);

void setup() {
  // открываем Serial-порт
  Serial.begin(115200);
  // начало работы с модулями I²C
  Wire.begin();
  // начало работы с датчиками линии
  octoliner.begin();
  // выставляем чувствительность фотоприёмников в диапазоне от 0 до 255
  octoliner.setSensitivity(200);
  // выставляем яркость свечения ИК-светодиодов в диапазоне от 0 до 255
  octoliner.setBrightness(255);

}

void loop() {
  // массив для хранения показателей с датчиков линии
  int dataSensors[8];
  // считываем значение с датчиков линии
  for (int i = 0; i < 8; i++) {
    // выводим значения в Serial-порт
    dataSensors[i] = octoliner.analogRead(i);
    Serial.print(octoliner.analogRead(i));
    Serial.print("\t");
  }
  Serial.println(octoliner.mapLine(dataSensors));
  Serial.println();
  // ждём пол секунды
  delay(500);
}
