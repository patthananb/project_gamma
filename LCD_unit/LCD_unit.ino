// Include the necessary libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

float weight = 12.34; // Example weight value
float vbat = 3.67; // Example battery voltage value

void setup() {
  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
}

void loop() {
  // Clear the LCD display
  lcd.clear();

  // Print the weight on the first line
  lcd.setCursor(0, 0); // Set the cursor to the first column of the first line
  lcd.print("Weight = ");
  lcd.print(weight); // Print the weight value

  // Print the battery voltage on the second line
  lcd.setCursor(0, 1); // Set the cursor to the first column of the second line
  lcd.print("Battery = ");
  lcd.print(vbat); // Print the battery voltage value

  delay(500); // Wait for 500 milliseconds
}