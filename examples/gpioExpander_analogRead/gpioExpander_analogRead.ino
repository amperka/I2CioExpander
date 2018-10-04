#include <Wire.h>
#include <GpioExpander.h>

GpioExpander adio(42);
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  adio.begin();
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 9; ++i)
  {
    Serial.print(adio.analogRead(i));
    Serial.print("\t");
  }
  Serial.println();
  delay(500);
}
