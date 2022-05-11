/*
  TelcoMT8888.h - Library for controlling the MT8888
  April 10th, 2022
  Released into the public domain.
*/
#ifndef TelcoMT8888_h
#define TelcoMT8888_h

#include "Arduino.h"

class TelcoMT8888
{
  public:
    TelcoMT8888(const uint8_t wr, const uint8_t rd, const uint8_t cs, const uint8_t rs0, const uint8_t d0, const uint8_t d1, const uint8_t d2, const uint8_t d3);
    uint8_t read_status();
    uint8_t read_data();
    void write_ctrl(uint8_t data);
    void write_data(uint8_t data);
    void send_digit(char digit);
    void stop_tone();
    void begin();
    bool data_ready();
    uint8_t read_digit();
    void set_data_out();
  private:
    // Utility Functions
    void _reset_data_pins();
    void _init_pins();
    uint8_t _read_bus(uint8_t rs0);
    void _write_bus(uint8_t rs0, uint8_t data);
    void _init_ctrl_pins();
    void _set_data_in();
    uint8_t _char_to_tone_num(char in);
    // Control Pins
    uint8_t _p_wr;    
    uint8_t _p_rd;
    uint8_t _p_cs;
    uint8_t _p_rs0;
    // Data Bus Pins
    uint8_t _p_d0;
    uint8_t _p_d1;
    uint8_t _p_d2;
    uint8_t _p_d3;
    // State
};

#endif
