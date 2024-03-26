#include <avr/io.h>
#include <avr/interrupt.h>
#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

HX711 scale;


//HX711 scale;
uint8_t dataPin = 6;
uint8_t clockPin = 7;

//LCD parmameter initialize
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display
//

float voltage = 0.0;  // Variable to store the battery voltage
float weight = 0.0;

void setup() {
  // Set internal reference voltage to Vcc (AVcc)
  ADMUX |= (1 << REFS0);
  
  // Enable ADC, enable ADC interrupt, set ADC Auto Trigger Source to Timer/Counter1 Compare Match A, and set ADC prescaler to 128 (16MHz / 128 = 125kHz)
  ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  
  // Set up Timer/Counter1 for interrupt every 1ms (prescaler 64, CTC mode)
  TCCR1B |= (1 << WGM12);       // Configure Timer1 for CTC mode
  OCR1A = 125;                  // Set compare value for 1ms at 16MHz clock (prescaler 64)
  TIMSK1 |= (1 << OCIE1A);      // Enable Timer1 Compare A Match interrupt
  TCCR1B |= (1 << CS11) | (1 << CS10);  // Start Timer1 with prescaler 64

  // Enable global interrupts
  sei();

  _init_Scale();

  lcd.begin();
  lcd.backlight();

  Serial.begin(115200);
}

void loop() {
  if (scale.is_ready()) {
    float weight = scale.get_units(1);
    Serial.println(scale.get_units(1));
     // Clear the LCD display
    lcd.clear();

    // Print the weight on the first line
    lcd.setCursor(0, 0); // Set the cursor to the first column of the first line
    lcd.print("Weight = ");
    lcd.print(weight); // Print the weight value

    // Print the battery voltage on the second line
    lcd.setCursor(0, 1); // Set the cursor to the first column of the second line
    lcd.print("Battery = ");
    lcd.print(voltage); // Print the battery voltage value

    delay(500); // Wait for 500 milliseconds
  }

 

}

ISR(TIMER1_COMPA_vect) {
  // Trigger ADC conversion by setting the ADC Auto Trigger Source to Timer/Counter1 Compare Match A
  ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect) {
  // Read ADC result
  uint16_t adcResult = ADCL; // Read ADCL first
  adcResult |= (ADCH << 8); // Combine ADCH with ADCL to get the full 10-bit result

  // Convert ADC value to voltage (Vref = Vin)
  voltage = (float)adcResult * (5.0 / 1023.0);

  }


void _init_Scale(){
  scale.begin(dataPin, clockPin);
  // load cell factor 5 KG
  scale.set_scale(465.154144);       // TODO you need to calibrate this yourself.
  // reset the scale to zero = 0
  scale.tare(20);
}
