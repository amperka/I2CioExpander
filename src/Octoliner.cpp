#include "Octoliner.h"

Octoliner::Octoliner(uint8_t i2caddress) : GpioExpander(i2caddress) {
}

void Octoliner::begin() {
    begin(&Wire);
}

void Octoliner::begin(TwoWire* wire) {
    GpioExpander::begin(wire);
    GpioExpander::pwmFreq(8000); // ~ 250 different pwm values
}

void Octoliner::setSensitivity(uint8_t sense) {
	analogWrite(_sensePin, sense);
}

void Octoliner::setBrightness(uint8_t brightness) {
	analogWrite(_ledBrightnessPin, brightness);
}

int Octoliner::analogRead(uint8_t sensor) {
	sensor &= 0x07;
	return GpioExpander::analogRead(_sensorPinMap[sensor]);
}

uint8_t Octoliner::getBinaryLine() {
	uint8_t result = 0;
	int port = digitalReadPort();
	for (uint8_t i = 0; i < 8; ++i) {
		int mask = 1;
		mask <<= _sensorPinMap[i];
		if (port & mask)
			result |= 1 << i;
	}
	return result;
}

uint8_t Octoliner::getBinaryLine(uint16_t treshold) {
	uint8_t result = 0;
	for (uint8_t i = 0; i < 8; ++i){
		int value = analogRead(i);
		if (treshold < value)
			result |= 1 << i;
	}
	return result;
}

float Octoliner::mapLine(uint8_t binaryLine) {
	int8_t sum = 0;
	int8_t avg = 0;
	int8_t weight[] = {4, 3, 2, 1, -1, -2, -3, -4};
	for (uint8_t i = 0; i < 8; i++) {
		bool isHigh = binaryLine & (1 << i);
		if (isHigh) {
			sum += 1;
			avg += weight[i];
		}
	}
	if (sum != 0) {
		return (float)avg / (float)sum / 4.0;
	}
	return 0;
}

float Octoliner::mapLine(int binaryLine[8]) {
	long sum = 0;
	long avg = 0;
	int8_t weight[] = {4, 3, 2, 1, -1, -2, -3, -4};
	for (int i = 0; i < 8; i++) {
		if (binaryLine[i]) {
			sum += binaryLine[i];
			avg += binaryLine[i] * weight[i];
		}
	}
	if (sum != 0) {
		return (float)avg / (float)sum / 4.0;
	}
	return 0;
}