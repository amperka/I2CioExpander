#include <Wire.h>
#include <I2Cadio.h>

ADIO adio(42);
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; ++i)
    adio.pinMode(i, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(adio.digitalReadPort(), BIN);
  delay(200);
}
