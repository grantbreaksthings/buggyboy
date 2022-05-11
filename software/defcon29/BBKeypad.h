/*
  BBKeypad.h - Library for controlling the Keypad on the Buggy Boy
  April 10th, 2022
  Released into the public domain.
*/
#ifndef BBKeypad_h
#define BBKeypad_h

#include "Arduino.h"

class BBKeypad{
  public:
    BBKeypad(const uint8_t data, const uint8_t clk, const uint8_t latch);
    char read_key();
  private:
    uint8_t _p_data;
    uint8_t _p_clk;
    uint8_t _p_latch;
    uint16_t _read_raw();
    char _lut(uint16_t raw);
};

#endif
