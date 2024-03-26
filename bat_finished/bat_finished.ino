#include <avr/io.h>
#include <avr/interrupt.h>

const int led0Pin = 2;    // 0% level LED
const int led25Pin = 3;   // 25% level LED
const int led50Pin = 4;   // 50% level LED
const int led100Pin = 5;  // 100% level LED

float voltage = 0.0;  // Variable to store the battery voltage

void setup() {
  _init_batind();
}

void loop() {
  // This code runs in the ADC interrupt, so nothing here
}

ISR(TIMER1_COMPA_vect) {
  // Start ADC conversion
  ADCSRA |= (1 << ADSC);
}

ISR(ADC_vect) {
  // Read ADC result
  uint16_t adcResult = ADCL; // Read ADCL first
  adcResult |= (ADCH << 8); // Combine ADCH with ADCL to get the full 10-bit result

  // Convert ADC value to voltage (Vref = Vin)
  voltage = (float)adcResult * (5.0 / 1023.0);

  // Adjust voltage to 100% if it exceeds 90% of Vin
  if (voltage > 4.5) {
    voltage = 5.0;
  }

  // Determine battery level
  int batteryLevel = 0;
  if (voltage < 3.3) {
    batteryLevel = 0;
  } else if (voltage < 3.8) {
    batteryLevel = 1;
  } else if (voltage < 4.1) {
    batteryLevel = 2;
  } else if (voltage < 4.4) {
    batteryLevel = 3;
  } else {
    batteryLevel = 4;
  }

  // Display battery level using LEDs
  switch (batteryLevel) {
    case 0:
      PORTD &= ~((1 << led0Pin) | (1 << led25Pin) | (1 << led50Pin) | (1 << led100Pin)); // All LEDs off
      break;
    case 1:
      PORTD |= (1 << led0Pin); // 0% level LED on
      break;
    case 2:
      PORTD |= (1 << led0Pin) | (1 << led25Pin); // 0% and 25% level LEDs on
      break;
    case 3:
      PORTD |= (1 << led0Pin) | (1 << led25Pin) | (1 << led50Pin); // 0%, 25%, and 50% level LEDs on
      break;
    case 4:
      PORTD |= (1 << led0Pin) | (1 << led25Pin) | (1 << led50Pin) | (1 << led100Pin); // All LEDs on
      break;
  }
}

void _init_batind(){
    // Set LED pins as output using DDR
  DDRD |= (1 << led0Pin) | (1 << led25Pin) | (1 << led50Pin) | (1 << led100Pin);

  // Set internal reference voltage to Vcc (AVcc)
  ADMUX |= (1 << REFS0);
  
  // Enable ADC, enable ADC interrupt, and set ADC prescaler to 128 (16MHz / 128 = 125kHz)
  ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  
  // Set up Timer/Counter1 for interrupt every 50ms (prescaler 64, CTC mode)
  TCCR1B |= (1 << WGM12);       // Configure Timer1 for CTC mode
  OCR1A = 1562;                 // Set compare value for 50ms at 16MHz clock (prescaler 64)
  TIMSK1 |= (1 << OCIE1A);      // Enable Timer1 Compare A Match interrupt
  TCCR1B |= (1 << CS11) | (1 << CS10);  // Start Timer1 with prescaler 64

  // Enable global interrupts
  sei();
}
