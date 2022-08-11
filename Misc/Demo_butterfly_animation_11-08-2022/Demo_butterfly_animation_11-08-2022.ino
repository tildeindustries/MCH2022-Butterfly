#include <Adafruit_NeoPixel.h>
#include "GammaCorrectionLib.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN         12
#define NUMPIXELS   5

#define Colour1 0x00058240
#define Colour2 0x0087C159
#define Colour3 0x00ADCACB
#define Colour4 0x00FEE3A2
#define Colour5 0x00F3C301

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int value = 0;

void DisplayCorrectedHexColour (int Position, long unsigned int Colour);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
}

void loop() {

  pixels.clear();
  for (int i = 0; i < 180; i++)
  {
    value = GammaCorrection::Correct8(i);
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(value, value, value));
    }
    pixels.show();
    delay(3+i/16);
  }
  delay(500);
    for (int i = 180; i < 255; i++)
  {
    value = GammaCorrection::Correct8(i);
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(value, value, value));
    }
    pixels.show();
    delay(2);
  }
  for (int i = 255; i > 0; i--)
  {
    value = GammaCorrection::Correct8(i);
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(value, value, value));
    }
    pixels.show();
    delay(2);
  }
  delay(500);

  for (int i = 0; i < 255; i++)
  {
    //pixels.setPixelColor(0, pixels.Color(GammaCorrection::Correct8((Colour1 & 0x00FF0000) >> 16), GammaCorrection::Correct8((Colour1 & 0x0000FF00) >> 8), GammaCorrection::Correct8(Colour1 & 0x000000FF)));
    DisplayCorrectedHexColour (0, Colour1, i);
    DisplayCorrectedHexColour (1, Colour2, i);
    DisplayCorrectedHexColour (2, Colour3, i);
    DisplayCorrectedHexColour (3, Colour4, i);
    DisplayCorrectedHexColour (4, Colour5, i);

    pixels.show();
    delay(3);
 }   
while(1);
}

void DisplayCorrectedHexColour (int Position, long unsigned int Colour, long unsigned int brightness)
{
   pixels.setPixelColor(Position, pixels.Color(
   GammaCorrection::Correct8(((Colour & 0x00FF0000) >> 16) * brightness/255), 
   GammaCorrection::Correct8(((Colour & 0x0000FF00) >> 8)* brightness/255), 
   GammaCorrection::Correct8((Colour & 0x000000FF)* brightness/255)));
}
