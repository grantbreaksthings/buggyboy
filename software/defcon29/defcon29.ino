#include "TelcoMT8888.h"
#include "BBKeypad.h"
#include <LiquidCrystal.h>

// Defines, Telco
#define TC_IRQ  2
#define TC_WR   A0
#define TC_RD   A1
#define TC_CS   A2
#define TC_RS0  A3
#define TC_D0   7
#define TC_D1   8
#define TC_D2   9
#define TC_D3   10
// Defines, Keypad
#define KP_DATA  13
#define KP_CLOCK 11
#define KP_LATCH 12
// Defines, Screen
#define SC_RS 3
#define SC_EN 4
#define SC_D4 7
#define SC_D5 8
#define SC_D6 9
#define SC_D7 10 
#define SC_WIDTH  16
#define SC_HEIGHT 2

TelcoMT8888 telco(TC_WR, TC_RD, TC_CS, TC_RS0, TC_D0, TC_D1, TC_D2, TC_D3);
BBKeypad keypad(KP_DATA, KP_CLOCK, KP_LATCH);
LiquidCrystal lcd(SC_RS, SC_EN, SC_D4, SC_D5, SC_D6, SC_D7);

void setup() {
  Serial.begin(9600);
  telco.begin();
  lcd.begin(SC_WIDTH, SC_HEIGHT);
  Serial.println("Starting...");
  lcd.print("Hewwo! UwU");
  lcd.setCursor(0, 1);
}

void loop() {
    char data = keypad.read_key();
    if(data){
      telco.set_data_out();
      lcd.setCursor(0, 0);
      lcd.print("Hewwo! OwO      ");
      lcd.setCursor(0, 1);
      lcd.print(data);
      telco.send_digit(data);
    }
    else{
      telco.stop_tone();
      telco.set_data_out();
      lcd.setCursor(0, 0);
      lcd.print("Hewwo! UwU      ");
    }
    if(telco.data_ready()){
      telco.set_data_out();
      lcd.setCursor(0, 0);
      lcd.print("Got data! >w<    ");
      lcd.setCursor(0, 1);
      lcd.print(telco.read_digit());
    }
}


/**
 OLD CODE

// Includes
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

// Defines, LED
#define PIN        6
#define NUMPIXELS  12
#define DELAYVAL 1 // Time (in milliseconds) to pause between pixels
#define RGB_MIN 0
#define RGB_MAX 150
#define PIXEL_DELAY 10
// Defines, Screen
#define SC_RS 3
#define SC_EN 4
#define SC_D4 7
#define SC_D5 8
#define SC_D6 9
#define SC_D7 10 
#define SC_WIDTH 16
// Defines, Keypad
#define SERI 13
#define CLOCKI 11
#define SHFLOAD 12
// Defines, Telco
#define TC_IRQ  2
#define TC_WR   A0
#define TC_RD   A1
#define TC_CS   A2
#define TC_RS0  A3
#define TC_D0   7
#define TC_D1   8
#define TC_D2   9
#define TC_D3   10
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

// Defines, indicators
#define LED_GREEN   A4
#define LED_RED     A5

// Globals, Telco
const char key_lookup[] = {
 'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
  '*', '#', 'A', 'B', 'C', 'X', 'Y', 'Z'
  };
int tx_data = 0;
int rx_data = 0;
int snd_rcv = 0;
char last_sent[SC_WIDTH + 1];
char last_recv[SC_WIDTH + 1];
int sent_index = 0;
int recv_index = 0;

// Globals, LED
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int r = 0;
int g = 0;
int b = 0;
int led_mux = 0;

// Globals, Screen
LiquidCrystal lcd(SC_RS, SC_EN, SC_D4, SC_D5, SC_D6, SC_D7);

// Globals, keypad
int key_num = 10;
char key_ch = '0';
int last_key_num = key_num;
char last_key_ch = key_ch;

void telco_irq();

void setup() {
  Serial.begin(9600);
  pixels.begin();
  lcd.begin(16, 2);
  init_kp();
  init_telco();
  pinMode(TC_IRQ, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(TC_IRQ), telco_irq, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  pixel_loop();
  screen_loop();
  kp_loop();
  telco_loop();
}


// Functions, LED
void pixel_loop() {
  rainbow_async();
}

int j = 0;
int i = 0;

void rainbow_async(){
  j++;
  for(i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel((i+j) & 255));
  }
  if(j>256){
    j=0;
  }
  pixels.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Functions, Keypad
int poll_kp(){
   int r1 = 0;
   int r2 = 0;
   digitalWrite(SHFLOAD, LOW);
   digitalWrite(CLOCKI, LOW); // makes input clock low state
   digitalWrite(CLOCKI, HIGH); // makes input clock low state   
   digitalWrite(SHFLOAD, HIGH);
   r1 = shiftIn(SERI, CLOCKI, LSBFIRST); // Read shift 8 bits of data in from 74HC166
   r2 = shiftIn(SERI, CLOCKI, LSBFIRST); // Read shift 8 bits of data in from 74HC166
   int r = ((r2 << 8) | r1) & 0xFFFF;
   r = r & 0xFFFF;
   //Serial.println (r1,BIN);
   //Serial.println (r2,BIN);
  if (r == 0x11)
    return 0b10000;
  if (r == 0x1100)
    return 0b10001;
  
  if (r & 0x01)
    return 0b0000;
  if (r & 0x02)
    return 0b1100;
  if (r & 0x04)
    return 0b1010;
  if (r & 0x08)
    return 0b1011;

  if (r & 0x10)
    return 0b1111;
  if (r & 0x20)
    return 0b1001;
  if (r & 0x40)
    return 0b1000;
  if (r & 0x80)
    return 0b0111;

  if (r & 0x100)
    return 0b1110;
  if (r & 0x200)
    return 0b0110;
  if (r & 0x400)
    return 0b0101;
  if (r & 0x800)
    return 0b0100;

  if (r & 0x1000)
    return 0b1101;
  if (r & 0x2000)
    return 0b0011;
  if (r & 0x4000)
    return 0b0010;
  if (r & 0x8000)
    return 0b0001;
  return 0b10010;
}

void init_kp(){
  pinMode(SERI, INPUT);
  pinMode(CLOCKI, OUTPUT);
  pinMode(SHFLOAD, OUTPUT);
  digitalWrite(CLOCKI, LOW);
  digitalWrite(SHFLOAD, HIGH);
}

void kp_loop(){
  last_key_num = key_num;
  last_key_ch = key_ch;
  int n = poll_kp();
  key_num = n;
  key_ch = key_to_char(key_num);
}

bool new_key_pressed(){
  return last_key_num != key_num;
}

// Functions, Screen
void screen_loop(){
  if(snd_rcv == SEND_MODE){
    lcd.print("TRANSMITTING...");
    lcd.setCursor(0, 1);
    lcd.print(last_sent);
    lcd.setCursor(0, 0);
  }
  if(snd_rcv == RECV_MODE){
    lcd.print("RECIEVING...");
    lcd.setCursor(0, 1);
    lcd.print(last_recv);
    lcd.setCursor(0, 0);
  }

}

// Functions, Telco
void reset_telco_data_pins(){
  // These pins are shared by the screen, 
  // they need to be reset to outputs
  pinMode(TC_D0, OUTPUT);
  pinMode(TC_D1, OUTPUT);
  pinMode(TC_D2, OUTPUT);
  pinMode(TC_D3, OUTPUT);
}

void init_telco_pins(){
  reset_telco_data_pins();
  pinMode(TC_WR, OUTPUT);
  pinMode(TC_RD, OUTPUT);
  pinMode(TC_CS, OUTPUT);
  pinMode(TC_RS0, OUTPUT);
  pinMode(TC_IRQ, INPUT);
  
  digitalWrite(TC_RD, HIGH);
  digitalWrite(TC_WR, HIGH);
  digitalWrite(TC_CS, HIGH);
  digitalWrite(TC_RS0, HIGH);
}

int telco_read(int rs0){
  digitalWrite(TC_RS0, rs0);
  digitalWrite(TC_CS, LOW);
  delay(1);
  
  digitalWrite(TC_RD, LOW);
  delay(1);
  
  pinMode(TC_D0, INPUT);
  pinMode(TC_D1, INPUT);
  pinMode(TC_D2, INPUT);
  pinMode(TC_D3, INPUT);
  byte b0 = digitalRead(TC_D0);
  byte b1 = digitalRead(TC_D1);
  byte b2 = digitalRead(TC_D2);
  byte b3 = digitalRead(TC_D3);

  digitalWrite(TC_RD, HIGH);
  delay(1);

  digitalWrite(TC_CS, HIGH);
  delay(1);

  reset_telco_data_pins();
  return (b0 << 0) | (b1 << 1) | (b2 << 2) | (b3 << 3);
}

void telco_write(int rs0, int data){
  digitalWrite(TC_RS0, rs0);
  digitalWrite(TC_CS, LOW);
  delay(1);
  
  digitalWrite(TC_WR, LOW);
  delay(1);

  digitalWrite(TC_D0, data & 0x1);
  digitalWrite(TC_D1, data & 0x2);
  digitalWrite(TC_D2, data & 0x4);
  digitalWrite(TC_D3, data & 0x8);
  delay(1);

  digitalWrite(TC_WR, HIGH);
  delay(1);

  digitalWrite(TC_CS, HIGH);
  delay(1);
}

int telco_read_status(){
  return telco_read(READ_STAT);
}

int telco_read_data(){
  return telco_read(READ_RX);
}

void telco_write_ctrl(int data){
  return telco_write(WRITE_CTRL, data);
}

void telco_write_tx(int data){
  return telco_write(WRITE_TX, data);
}

void init_telco(){
  init_telco_pins();
  telco_read_status();
  telco_write_ctrl(0b0000);
  telco_write_ctrl(0b0000);
  telco_write_ctrl(0b1000);
  telco_write_ctrl(0b0000);
  reset_telco_buffers();
}

void reset_telco_buffers(){
  for(int i=0; i < SC_WIDTH; i++){
    last_sent[i] = ' ';
    last_recv[i] = ' ';
  }
  last_sent[SC_WIDTH] = 0;
  last_recv[SC_WIDTH] = 0;
  sent_index = 0;
  recv_index = 0;
}

void telco_send_digit(int i){
  telco_write_tx(i);
  telco_write_ctrl(TOUT | IRQ_EN | RSEL);
  telco_write_ctrl(NO_BURST);
  // while(!(telco_read_status() & 0b1)){
  //   continue;
  // }

  if(!new_key_pressed()){
    return;
  }
  if(sent_index > SC_WIDTH){
    sent_index = 0;
  }
  last_sent[sent_index] = key_to_char(i);
  sent_index++;
  delay(100);
}

int telco_read_digit(){
  if(!(telco_read_status() & 0b10)){
    Serial.println("No data!");
    // return -1;
  }
  
  int data = telco_read_data();
  if(recv_index > SC_WIDTH){
    recv_index = 0;
  }
  last_recv[recv_index] = key_to_char(data);
  recv_index++;
  Serial.println(data);
  return data;
}

char key_to_char(int i){
  return key_lookup[i];
}

void telco_loop(){
  if(key_ch == 'X'){
    snd_rcv = SEND_MODE;
    reset_telco_buffers();
  }
  if(key_ch == 'Y'){
    snd_rcv = RECV_MODE;
    reset_telco_buffers();
  }

  if(snd_rcv == SEND_MODE){
    if(key_ch != 'Z'){
      digitalWrite(LED_GREEN, HIGH);
      telco_send_digit(key_num);
      digitalWrite(LED_GREEN, LOW);
    }
    else{
      telco_write_ctrl(IRQ_EN);
    }
  }
  if(snd_rcv == RECV_MODE){
    // telco_read_digit();
  }

}

void telco_irq(){
  if(snd_rcv == SEND_MODE){
    return;
  }
  // Serial.println("Got data!");
  digitalWrite(LED_RED, HIGH);
  telco_read_digit();
  digitalWrite(LED_RED, HIGH);
}

**
 */
