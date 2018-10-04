#include <Wire.h>
#include <Octoliner.h>

Octoliner octoliner(42);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  octoliner.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t sensitivity = map(analogRead(A0), 0, 1024, 0, 255);
  uint8_t irLedBrightness = map(analogRead(A4), 0, 1024, 0, 255);
  
  for (int i = 0; i < 8; ++i) {
    
  }
}
