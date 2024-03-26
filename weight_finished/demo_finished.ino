//
//    FILE: HX_is_ready.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711


#include "HX711.h"
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"

HX711 scale;
uint8_t dataPin = 6;
uint8_t clockPin = 7;
Adafruit_7segment matrix = Adafruit_7segment();


void setup()
{
  Serial.begin(115200);
  _init_Scale();
  matrix.begin(0x70);
}


void loop()
{
  if (scale.is_ready());
  {
    float weight = scale.get_units(1);
    matrix.print(weight);
    matrix.writeDisplay();
    delay(100);
  }
 
}

void _init_Scale(){
  scale.begin(dataPin, clockPin);
  // load cell factor 5 KG
  scale.set_scale(465.154144);       // TODO you need to calibrate this yourself.
  // reset the scale to zero = 0
  scale.tare(20);
}