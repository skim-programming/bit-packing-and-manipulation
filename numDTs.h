#pragma once
#include <stdint.h>
#include <string>
#include <stdexcept>
#include <bitset>
// Mostly low level stuff, just working with bit masks, bit operations, etc.

struct numDT1 {
	/*
	Properties:
	Byte 1 used as multiplier
	Byte 2 used as multiplicand
	Bit 7 of byte 1(multiplier) used as adding +1
	*/
private:
	uint8_t high = 0b00000000;
	uint8_t low = 0b00000000;
public:
	numDT1() = default;
	numDT1(uint8_t high_, uint8_t low_) : high(high_), low(low_) {};
	numDT1(uint16_t num) { high = num >> 8; low = num & 0xFF; };

	uint16_t const read() {
		return ((uint16_t)high) << 8 | low;
	}

	void write(const uint8_t high_ = 0b00000000, const uint8_t low_ = 0b00000000) {
		high = high_;
		low = low_;
	}

	uint16_t const val() {
		uint8_t flag = (high & 0b10000000) ? 1 : 0;
		uint8_t multiplier = high & 0b01111111;

		return multiplier * low + flag;
	}
};

// Signed number with unsigned number(two's complement)
struct sint {
	uint16_t num;

	signed short val() const {
		if (num & 0x8000) {
			return -(signed short)((~num)) - 1;
		}
		return (signed short)num;
	}
	void signedToSint(signed short num) {
		this->num = static_cast<uint16_t>(num);
	}
};

// Bitfield testing
struct Flags {
	unsigned ready : 1;
	unsigned error : 1;
};

uint8_t parseByte(std::string s) {
	if (s.size() != 8) throw std::invalid_argument("Must be 8 bits long");
	uint8_t result = 0;
	for (const auto& c : s) {
		result <<= 1;
		if (c == '1') result |= 1;
		else if (c != '0') throw std::invalid_argument("Invalid character");
	}
	return result;
}
