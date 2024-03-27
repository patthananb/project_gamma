
#include <Wire.h>


uint8_t _addr = 0x27;
uint8_t p_write;

char begin() {
  Wire.begin();
  //if (Wire.begin()) {
    write(0x00);
    write4Bit(0x03);
    delayMicroseconds(4500);
    write4Bit(0x03);
    delayMicroseconds(4500);
    write4Bit(0x03);
    delayMicroseconds(150);
    write4Bit(0x02);

    write4Bit(0x02);
    write4Bit(0x0C);
    write4Bit(0x00);
    write4Bit(0x08);
    write4Bit(0x00);
    write4Bit(0x01);
    write4Bit(0x00);
    write4Bit(0x06);

    write4Bit(0x00);
    write4Bit(0x0F);
    setBacklight(1);
  //  return 1;
  //} else {
    return 1;
  //}
}

void write(uint8_t data) {
  Wire.beginTransmission(_addr);
  Wire.write(data);
  Wire.endTransmission();
}

void latch() {
  p_write = p_write & 0xF9;
  write(p_write);
  delay(1);
  p_write = p_write | 0x04;
  write(p_write);
  delay(1);
  p_write = p_write & 0xF9;
  write(p_write);
  delayMicroseconds(100);
}

void write4Bit(uint8_t data) {
  p_write &= 0x0F;
  p_write |= data << 4;
  latch();
}

void writeByte(uint8_t data) {
  write4Bit(data >> 4);
  write4Bit(data);
}

void writeCommand(uint8_t data) {
  p_write = p_write & 0xF8;  // RS LOW
  write(p_write);
  writeByte(data);
}
void writeData(uint8_t data) {
  p_write = p_write | 0x01;  // RS HIGH
  write(p_write);
  writeByte(data);
}

void setPosition(uint8_t x, uint8_t y) {
  if (y == 0) {
    writeCommand(0x80 | x);
  }
  if (y == 1) {
    writeCommand(0x80 | 0x40 | x);
  }
}
void print(const char *str) {
  while (*str) {
    writeData(*str);
    str += 1;
  }
}

void clear(){
   writeCommand(0x01);
}

void setBacklight(uint8_t data) {
  p_write=0;
  if (data) {
    p_write |= 0x08;
    write(p_write);
  } else {
    p_write &= 0xF7;
    write(p_write);
  }
  delay(50);
}
float weight = 132.5;
float volatge = 3.5;
void setup(){
  begin();
  clear();
  setPosition(0, 0);
  print("Guten Morgen");
  delay(500);
  setPosition(0, 1);
  print("Zeig ?");
  delay(500);
  clear();
  setPosition(0, 0);
  print("HEIL !");
  delay(1000);
  clear();
  
}
void loop(){
  
  float weight = 123.4;
  char weightbuffer[4];
  dtostrf(weight, 4, 1, weightbuffer);

  setPosition(0, 0);
  print("Weight = ");
  print(weightbuffer);



  float voltage = 4.5;
  char voltagebuffer[5];
  dtostrf(voltage, 2, 1, voltagebuffer);
  
  setPosition(0,1);
  print("Battery =");
  print(voltagebuffer);
  delay(300);
    
}