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

struct ob {
    private: uint64_t data = 0;
    
    public:
    std::bitset<sizeof(data)*8> writeData(decltype(data) val) { // Only write with 0bXXXXXXXX
        data = val;
        return std::bitset<sizeof(data)*8>(data);
    }
    std::bitset<sizeof(data)*8> setFlag(decltype(data) index){
        data = data ^ 1 << index /*% (sizeof(data)*8)*/;
        return std::bitset<sizeof(data)*8>(data);
    }
    decltype(data) readData() const {
        return data;
    }
    std::bitset<sizeof(data)*8> readBs() const {
        return std::bitset<sizeof(data)*8>(data);
    }
    std::ostringstream returnFlags() const {
        std::ostringstream out;
        std::bitset<sizeof(data)*8> bs = data;
        for(decltype(data) i = 0; i < sizeof(data) * 8; i++){
            if(bs[i]){
                out << "flag " << i << " set\n";
            }
        }
        return out;
    }
    bool getFlag(decltype(data) index) const {
        if(data & 1 << index/*%(sizeof(data)*8)*/){
            return true;
        }
        return false;
    }
    static decltype(data) getType(){
        return decltype(data)(0);
    }
    ob() = default;
    ob(decltype(data) data_) {(void)this->writeData(data_);};
};

struct sf{
    ob bytes[4];
    uint32_t btott(){
        uint32_t out = 0;
        for(int i = sizeof(bytes)-1; i>=0; i--){
            out |= (uint32_t)bytes[i].readData() << 8*i;
        }
        return out;
    }
    template <typename T>
    sf tttob(T inp){
        decltype(ob::getType()) in = static_cast<decltype(ob::getType())>(inp);
        sf out;
        int partition = (sizeof(in)*8)/(sizeof(ob::getType())*8);
        std::cout << (sizeof(in)*8)/(sizeof(ob::getType())*8) << "\n";
        int inc = sizeof(in)*8/partition;
        std::cout << inc << "\n";
        for(int i = 0; i<partition; i++){
            out[i] = (decltype(ob::getType()))((in >> inc * i) & ~(decltype(ob::getType()))(0));
        }
        return out;
    }
    friend std::ostream& operator<<(std::ostream& os, sf const& save){
        for(int i = sizeof(bytes)/sizeof(ob::getType())-1; i>=0; i--){
            os << save.bytes[i].readBs() << " \n";
        }
        return os;
    }
    ob& operator[](int index){
        return bytes[index];
    }
    const ob& operator[](int index) const{
        return bytes[index];
    }
};

