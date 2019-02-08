// Arduino Code to measure distance with a Sharp GP2D12 sensor
// www.swanrobotics.com
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Volume.h"

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix();

Volume vol;
int IR_SENSOR = 0; // Sensor is connected to the analog A0
int intSensorResult = 0; //Sensor result
float fltSensorCalc = 0; //Calculated value
const int SPEAKERPIN = 5;
const int PIN_ANALOG_IN = 2;
const int BUTTONPIN = 4;
int status = 0;
LiquidCrystal_I2C screen(0x27, 16, 2);
static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
    
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },

   hi_bmp[] =
  { B00000000,
    B01001010,
    B01001010,
    B01001010,
    B01111010,
    B01001010,
    B01001010,
    B00000000
    };
void setup()
{
  Serial.begin(9600); // Setup communication with computer to present results serial monitor
  screen.init();
  screen.backlight();
  screen.home();
  matrix1.begin(0x70);
  matrix1.clear();
  matrix1.drawBitmap(0, 0, hi_bmp, 8, 8, LED_ON);
  matrix1.writeDisplay();
}
void loop()
{
  // read the value from the ir sensor
  int value;
  int trans;
  intSensorResult = analogRead(IR_SENSOR); //Get sensor value
  fltSensorCalc = (6787.0 / (intSensorResult) - 3.0) - 4.0; //Calculate distance in cm
  int buttonState = digitalRead(BUTTONPIN);

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);
  //Serial.println(intSensorResult);
  trans = map(intSensorResult, 20, 600, 0, 100);
  // Convert envelope value into a message
    if (buttonState == LOW) {
      if (status == 1) status = 0;
      else status = 1; 
    }
    if (status == 1) {
    screen.display();
    Serial.print("Status: ");
    if(trans <= 40)
    {
      vol.tone(1000, 63);
      Serial.println("Come Closer.");
      screen.print("Come closer.");
      matrix1.clear();
      matrix1.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
      matrix1.writeDisplay();
    }
    else if( (trans > 40) && ( trans <= 70) )
    {
      vol.tone(1000, 127);
      Serial.println("You're okay.");
      screen.print("You're okay");
      matrix1.clear();
      matrix1.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
      matrix1.writeDisplay();
    }
    else
    {
      vol.tone(1000, 255);
      Serial.println("Too close.");
      screen.print("Too close.");
      matrix1.clear();
      matrix1.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
      matrix1.writeDisplay();
    }

    // pause for 1 second
    delay(300);
    }
    else {screen.clear();}
}
