#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

 /**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64(double number) {
	return *((uint64_t *)(&number));
}

bool getBit(const uint64_t number, const uint8_t index) {
	return number >> index & 1;
}

uint64_t getExponent(uint64_t number) {
	return (number << 1) >> 53;
}

uint64_t getFraction(uint64_t number) {
	return (number << 12) >> 12;
}

/**
* Checkers here:
*/

bool checkForPlusZero(uint64_t number) {

	return number == 0x0000000000000000;
}

bool checkForMinusZero(uint64_t number) {
	return number == 0x8000000000000000;
}

bool checkForPlusInf(uint64_t number) {
	/// Your code here.
	return !getBit(number, 63) & getExponent(number) == 0x7ff & getFraction(number) == 0;
}

bool checkForMinusInf(uint64_t number) {
	/// Your code here.
	return getBit(number, 63) & getExponent(number) == 0x7ff & getFraction(number) == 0;
}

bool checkForPlusNormal(uint64_t number) {
	/// Your code here.
	uint16_t temp = getExponent(number);
	return !getBit(number, 63) & temp != 0 & temp != 0x7ff;
}

bool checkForMinusNormal(uint64_t number) {
	/// Your code here.
	uint16_t temp = getExponent(number);
	return getBit(number, 63) & temp != 0 & temp != 0x7ff;
}

bool checkForPlusDenormal(uint64_t number) {
	/// Your code here.
	return !getBit(number, 63) & getExponent(number) == 0 & getFraction(number) != 0;
}

bool checkForMinusDenormal(uint64_t number) {
	/// Your code here.
	return getBit(number, 63) & getExponent(number) == 0 & getFraction(number) != 0;
}

bool checkForSignalingNan(uint64_t number) {
	/// Your code here.
	return getExponent(number) == 0x7ff & getFraction(number) != 0 & !getBit(number, 51);
}

bool checkForQuietNan(uint64_t number) {
	/// Your code here.
	return  getExponent(number) == 0x7ff & getBit(number, 51);
}


void classify(double number) {
	if (checkForPlusZero(convertToUint64(number))) {
		printf("Plus zero\n");
	}

	else if (checkForMinusZero(convertToUint64(number))) {
		printf("Minus zero\n");
	}

	else if (checkForPlusInf(convertToUint64(number))) {
		printf("Plus inf\n");
	}

	else if (checkForMinusInf(convertToUint64(number))) {
		printf("Minus inf\n");
	}

	else if (checkForPlusNormal(convertToUint64(number))) {
		printf("Plus regular\n");
	}

	else if (checkForMinusNormal(convertToUint64(number))) {
		printf("Minus regular\n");
	}

	else if (checkForPlusDenormal(convertToUint64(number))) {
		printf("Plus Denormal\n");
	}

	else if (checkForMinusDenormal(convertToUint64(number))) {
		printf("Minus Denormal\n");
	}

	else if (checkForSignalingNan(convertToUint64(number))) {
		printf("Signailing NaN\n");
	}

	else if (checkForQuietNan(convertToUint64(number))) {
		printf("Quiet NaN\n");
	}

	else {
		printf("Error.\n");
	}
}

void test() {
	uint64_t value = 0x0000'0000'0000'0000;
	classify(*(double*)&value);
	value = 0x8000'0000'0000'0000;
	classify(*(double*)&value);
	value = 0x7ff0'0000'0000'0000;
	classify(*(double*)&value);
	value = 0xfff0'0000'0000'0000;
	classify(*(double*)&value);
	value = 0x7fd0'0000'0000'0000;
	classify(*(double*)&value);
	value = 0xffd0'0000'0000'0000;
	classify(*(double*)&value);
	value = 0x0000'10c0'000e'0012;
	classify(*(double*)&value);
	value = 0x8000'10c0'000e'0012;
	classify(*(double*)&value);
	value = 0xfff0'10c0'000e'0012;
	classify(*(double*)&value);
	value = 0xffff'10c0'000e'0012;
	classify(*(double*)&value);
}

int main() {
	test();
	return 0;
}