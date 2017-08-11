#include <Wire.h>
#include <GpioExpander.h>

GpioExpander adio(42);
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  adio.pinMode(0, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  adio.digitalWrite(0, HIGH);
  delay(1000);
  adio.digitalWrite(0, LOW);
  delay(1000);
}
