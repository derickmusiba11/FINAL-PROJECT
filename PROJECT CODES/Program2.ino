#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int waterFlowSensorPin = 2; // Pin connected to the water flow sensor

// LCD object initialization
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// GSM module communication
SoftwareSerial gsmModule(0, 1);  // RX, TX pins for the GSM module

// Variables
float totalWaterUnits = 10.0;    // Total water units initially set to 10 units
float conversionFactor = 10.0;   // Conversion factor from cc to units

// Function to calculate remaining water units and send SMS alert if necessary
void calculateRemainingWaterUnits(float consumedVolume) {
  float consumedUnits = consumedVolume / conversionFactor;
  totalWaterUnits -= consumedUnits;

  // Check if remaining units are below the threshold (e.g., 5 units)
  if (totalWaterUnits < 5.0) {
    // Trigger SMS alert to inform the user about low remaining units
    gsmModule.print("AT+CMGF=1\r");   // Set GSM module to text mode
    delay(1000);
    gsmModule.print("AT+CMGS=\"+1234567890\"\r");  // Replace with user's phone number
    delay(1000);
    gsmModule.print("Low water units! Please refill.");  // Replace with desired SMS message
    delay(1000);
    gsmModule.write(0x1A);  // Send the SMS
    delay(1000);
  }
}

// Function to update the LCD display with water usage and remaining units
void updateLCD(float consumedVolume) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Usage: ");
  lcd.print(consumedVolume);
  lcd.print(" cc");

  lcd.setCursor(0, 1);
  lcd.print("Units: ");
  lcd.print(totalWaterUnits, 1);
  lcd.print(" / 10");
}

void setup() {
  // Initialize the LCD display
  lcd.begin(16, 2);
  lcd.clear();

  // Initialize the GSM module
  gsmModule.begin(9600);
  delay(1000);
}

void loop() {
  // Read water flow sensor and calculate consumed volume
  float consumedVolume = digitalRead(waterFlowSensorPin);  // Assuming digital output from flow sensor
  calculateRemainingWaterUnits(consumedVolume);

  // Update the LCD display
  updateLCD(consumedVolume);

  delay(1000);  // Adjust delay as needed
}
