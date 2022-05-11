#include "BBKeypad.h"

#define KEY_D    0b1
#define KEY_OCTO 0b10  // it is not a hashtag
#define KEY_C    0b100
#define KEY_9    0b1000
#define KEY_B    0b10000
#define KEY_6    0b100000
#define KEY_A    0b1000000
#define KEY_3    0b10000000
#define KEY_0    0b100000000
#define KEY_STAR 0b1000000000
#define KEY_8    0b10000000000
#define KEY_7    0b100000000000
#define KEY_5    0b1000000000000
#define KEY_4    0b10000000000000
#define KEY_2    0b100000000000000
#define KEY_1    0b1000000000000000

BBKeypad::BBKeypad(const uint8_t data, const uint8_t clk, const uint8_t latch){
  _p_clk = clk;
  _p_data = data;
  _p_latch = latch;
  pinMode(_p_data, INPUT);
  pinMode(_p_clk, OUTPUT);
  pinMode(_p_latch, OUTPUT);
  digitalWrite(_p_clk, LOW);
  digitalWrite(_p_latch, HIGH);
}

uint16_t BBKeypad::_read_raw(){
   uint8_t r1 = 0;
   uint8_t r2 = 0;
   digitalWrite(this->_p_latch, LOW);
   digitalWrite(this->_p_clk, LOW); // makes input clock low state
   digitalWrite(this->_p_clk, HIGH); // makes input clock low state   
   digitalWrite(this->_p_latch, HIGH);
   r1 = shiftIn(this->_p_data, this->_p_clk, LSBFIRST); // Read shift 8 bits of data in from 74HC166
   r2 = shiftIn(this->_p_data, this->_p_clk, LSBFIRST); // Read shift 8 bits of data in from 74HC166
   return ((r2 << 8) | r1) & 0xFFFF;
}

char BBKeypad::read_key(){
  return this->_lut(this->_read_raw());
}

char BBKeypad::_lut(uint16_t raw){
  switch(raw){
    case KEY_A | KEY_B:
      return 'X';
    case KEY_C | KEY_D:
      return 'Y';
    case KEY_D:
      return 'D';
    case KEY_OCTO:
      return '#';
    case KEY_C:
      return 'C';
    case KEY_9:
      return '9';
    case KEY_B:
      return 'B';
    case KEY_6:
      return '6';
    case KEY_A:
      return 'A';
    case KEY_3:
      return '3';
    case KEY_0:
      return '0';
    case KEY_STAR:
      return '*';
    case KEY_8:
      return '8';
    case KEY_7:
      return '7';
    case KEY_5:
      return '5';
    case KEY_4:
      return '4';
    case KEY_2:
      return '2';
    case KEY_1:
      return '1';
    default:
      return 0;
  }
}
