#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN         12

#define Colour1 0x00058240
#define Colour2 0x0087C159
#define Colour3 0x00ADCACB
#define Colour4 0x00FEE3A2
#define Colour5 0x00F3C301
#define White   0x00FFFFFF

Adafruit_NeoPixel pixels(5, PIN, NEO_GRB + NEO_KHZ800);

const static uint8_t GammaTable[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

void DisplayCorrectedHexColour (int Position, long unsigned int Colour, long unsigned int brightness)
{
  pixels.setPixelColor(Position, pixels.Color(
                         GammaTable[(((Colour & 0x00FF0000) >> 16) * brightness / 255)],
                         GammaTable[((Colour & 0x0000FF00) >> 8)* brightness / 255],
                         GammaTable[(Colour & 0x000000FF)* brightness / 255]));
  pixels.show();
}

void delay_ (unsigned long time_delay)
{
  delay(time_delay);
}

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
    for (int j = 0; j < 5; j++) {
      DisplayCorrectedHexColour (j, White, i);
    }
    delay_(3 + i / 16);
  }
  delay_(500);
  for (int i = 180; i < 255; i++)
  {
    for (int j = 0; j < 5; j++) {
      DisplayCorrectedHexColour (j, White, i);
    }
    delay_(2);
  }
  for (int i = 255; i > 0; i--)
  {
    for (int j = 0; j < 5; j++) {
      DisplayCorrectedHexColour (j, White, i);
    }
    delay_(2);
  }
  delay_(500);

  for (int i = 0; i < 255; i++)
  {
    DisplayCorrectedHexColour (0, Colour1, i);
    DisplayCorrectedHexColour (1, Colour2, i);
    DisplayCorrectedHexColour (2, Colour3, i);
    DisplayCorrectedHexColour (3, Colour4, i);
    DisplayCorrectedHexColour (4, Colour5, i);
    delay_(3);
  }
  while (1);
}
