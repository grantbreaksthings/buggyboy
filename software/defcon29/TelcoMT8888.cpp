#include "TelcoMT8888.h"

#define WRITE_TX    0
#define WRITE_CTRL  1
#define READ_RX     0
#define READ_STAT   1
#define SEND_MODE   0
#define RECV_MODE   1

#define TOUT        1
#define CP          2
#define IRQ_EN      4
#define RSEL        8

#define NO_BURST    1
#define TEST        2
#define SIN_DUAL    4
#define COL_ROW     8

TelcoMT8888::TelcoMT8888(const uint8_t wr, const uint8_t rd, const uint8_t cs, const uint8_t rs0, const uint8_t d0, const uint8_t d1, const uint8_t d2, const uint8_t d3){
  // Control pins
  _p_wr = wr;
  _p_rd = rd;
  _p_cs = cs;
  _p_rs0 = rs0;
  // Data Bus Pins
  _p_d0 = d0;
  _p_d1 = d1;
  _p_d2 = d2;
  _p_d3 = d3;
}

void TelcoMT8888::begin(){
  _init_ctrl_pins();
  // Init sequence
  // Wait 100ms
  delay(100);
  // Read out the status reguster
  read_status();
  // Clear out all control registers
  write_ctrl(0b0000);
  write_ctrl(0b0000);
  write_ctrl(0b1000);
  write_ctrl(0b0000);
  // Read out the status register
  read_status();
}

// Functions, Telco
void TelcoMT8888::set_data_out(){
  pinMode(this->_p_d0, OUTPUT);
  pinMode(this->_p_d1, OUTPUT);
  pinMode(this->_p_d2, OUTPUT);
  pinMode(this->_p_d3, OUTPUT);
}

void TelcoMT8888::_set_data_in(){
  pinMode(this->_p_d0, INPUT);
  pinMode(this->_p_d1, INPUT);
  pinMode(this->_p_d2, INPUT);
  pinMode(this->_p_d3, INPUT);
}

void TelcoMT8888::_init_ctrl_pins(){
  pinMode(this->_p_wr, OUTPUT);
  pinMode(this->_p_rd, OUTPUT);
  pinMode(this->_p_cs, OUTPUT);
  pinMode(this->_p_rs0, OUTPUT);
  
  digitalWrite(this->_p_rd, HIGH);
  digitalWrite(this->_p_wr, HIGH);
  digitalWrite(this->_p_cs, HIGH);
  digitalWrite(this->_p_rs0, HIGH);
}

uint8_t TelcoMT8888::_read_bus(uint8_t rs0){
  digitalWrite(this->_p_rs0, rs0);
  digitalWrite(this->_p_cs, LOW);
  delay(1);
  
  digitalWrite(this->_p_rd, LOW);
  delay(1);
  this->_set_data_in();
  byte b0 = digitalRead(this->_p_d0);
  byte b1 = digitalRead(this->_p_d1);
  byte b2 = digitalRead(this->_p_d2);
  byte b3 = digitalRead(this->_p_d3);

  digitalWrite(this->_p_rd, HIGH);
  delay(1);

  digitalWrite(this->_p_cs, HIGH);
  delay(1);

  return (b0 << 0) | (b1 << 1) | (b2 << 2) | (b3 << 3);
}

void TelcoMT8888::_write_bus(uint8_t rs0, uint8_t data){
  digitalWrite(this->_p_rs0, rs0);
  digitalWrite(this->_p_cs, LOW);
  delay(1);
  
  digitalWrite(this->_p_wr, LOW);
  delay(1);
  this->set_data_out();
  digitalWrite(this->_p_d0, data & 0x1);
  digitalWrite(this->_p_d1, data & 0x2);
  digitalWrite(this->_p_d2, data & 0x4);
  digitalWrite(this->_p_d3, data & 0x8);
  delay(1);

  digitalWrite(this->_p_wr, HIGH);
  delay(1);

  digitalWrite(this->_p_cs, HIGH);
  delay(1);
}

uint8_t TelcoMT8888::read_status(){
  return this->_read_bus(READ_STAT);
}

uint8_t TelcoMT8888::read_data(){
  return this->_read_bus(READ_RX);
}

void TelcoMT8888::write_ctrl(uint8_t data){
  return this->_write_bus(WRITE_CTRL, data);
}

void TelcoMT8888::write_data(uint8_t data){
  return this->_write_bus(WRITE_TX, data);
}

void TelcoMT8888::send_digit(char digit){
  uint8_t i = this->_char_to_tone_num(digit);
  if(i == 255){
    return;
  }
  this->write_data(i);
  this->write_ctrl(TOUT | IRQ_EN | RSEL);
  this->write_ctrl(NO_BURST);
}

void TelcoMT8888::stop_tone(){
  this->write_ctrl( IRQ_EN | RSEL);
  this->write_ctrl(NO_BURST);
}

bool TelcoMT8888::data_ready(){
  return this->read_status() & 0b100;
}

uint8_t TelcoMT8888::read_digit(){
  return this->read_data();
}

uint8_t TelcoMT8888::_char_to_tone_num(char in){
  switch(in){
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case '0':
      return 10;
    case '*':
      return 11;
    case '#':
      return 12;
    case 'A':
      return 13;
    case 'B':
      return 14;
    case 'C':
      return 15;
    case 'D':
      return 0;
    default:
      return 255;
  }
}
