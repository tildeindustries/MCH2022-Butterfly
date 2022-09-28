#include <Adafruit_NeoPixel.h>
#include "GammaCorrectionLib.h"
#include <Wire.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define disk1 0x50
#define PIN         A3
#define NUMPIXELS   5

long unsigned int Butterfly_ID = 0;
long unsigned int Colour1 = 0;
long unsigned int Colour2 = 0;
long unsigned int Colour3 = 0;
long unsigned int Colour4 = 0;
long unsigned int Colour5 = 0;
bool Butterfly_ID_changed_flag = 1;
unsigned int address = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int value = 0;

void DisplayCorrectedHexColour (int Position, long unsigned int Colour);
void ReadSAO();
byte readEEPROM_1byte(int deviceaddress, unsigned int eeaddress );

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(115200);
  Serial.println("Starting");
  Wire.begin();

  pixels.begin();
  ReadSAO();
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
    delay(3 + i / 16);
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
    DisplayCorrectedHexColour (0, Colour1, i);
    DisplayCorrectedHexColour (1, Colour2, i);
    DisplayCorrectedHexColour (2, Colour3, i);
    DisplayCorrectedHexColour (3, Colour4, i);
    DisplayCorrectedHexColour (4, Colour5, i);

    pixels.show();
    delay(3);
  }
  Butterfly_ID_changed_flag = 0;
  while (!Butterfly_ID_changed_flag)
  {
    ReadSAO();
    delay (500);
  }
}

void DisplayCorrectedHexColour (int Position, long unsigned int Colour, long unsigned int brightness)
{
  pixels.setPixelColor(Position, pixels.Color(
                         GammaCorrection::Correct8(((Colour & 0x00FF0000) >> 16) * brightness / 255),
                         GammaCorrection::Correct8(((Colour & 0x0000FF00) >> 8)* brightness / 255),
                         GammaCorrection::Correct8((Colour & 0x000000FF)* brightness / 255)));
}

void ReadSAO()
{
  address = 0; // resets address
  uint8_t SAO_ID_header[8];

  Serial.println("Attempting to read 1 byte address EEPROM");
  Serial.println("Header:");
  for (int i = 0; i < 8; i++)
  {
    SAO_ID_header[i] = readEEPROM_1byte(disk1, address);
    Serial.print("data received HEX: ");
    Serial.print(SAO_ID_header[i], HEX);
    Serial.print(" DEC: ");
    Serial.print(SAO_ID_header[i], DEC);
    Serial.print(" ASCII: ");
    Serial.println((char)SAO_ID_header[i]);
    address++;
  }

  if (SAO_ID_header[1] == 'I' && SAO_ID_header[2] == 'F' && SAO_ID_header[3] == 'E')
  {
    Serial.println("LIFE parsed, SAO detected");
    uint8_t device_name [SAO_ID_header[4]];
    uint8_t driver_name [SAO_ID_header[5]];
    long unsigned int buffer_data [SAO_ID_header[6]];

    Serial.println("Device Name:");
    for (int i = 0; i < SAO_ID_header[4]; i++)
    {
      device_name[i] = readEEPROM_1byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(device_name[i], HEX);
      Serial.print(" DEC: ");
      Serial.print(device_name[i], DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)device_name[i]);
      address++;
    }

    bool device_name_check = 1;
    if (device_name[0] != 'M') device_name_check = 0;
    if (device_name[1] != 'C') device_name_check = 0;
    if (device_name[2] != 'H') device_name_check = 0;
    if (device_name[3] != 'b') device_name_check = 0;
    if (device_name[4] != 'u') device_name_check = 0;
    if (device_name[5] != 't') device_name_check = 0;
    if (device_name[6] != 't') device_name_check = 0;
    if (device_name[7] != 'e') device_name_check = 0;
    if (device_name[8] != 'r') device_name_check = 0;
    if (device_name[9] != 'f') device_name_check = 0;
    if (device_name[10] != 'l') device_name_check = 0;
    if (device_name[11] != 'y') device_name_check = 0;
    if (device_name_check) Serial.println("Butterfly Addon found!");

    Serial.println("Driver Name:");
    for (int i = 0; i < SAO_ID_header[5]; i++)
    {
      driver_name[i] = readEEPROM_1byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(driver_name[i], HEX);
      Serial.print(" DEC: ");
      Serial.print(driver_name[i], DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)driver_name[i]);
      address++;
    }

    Serial.println("Data Buffer:");
    for (int i = 0; i < SAO_ID_header[6]; i++)
    {
      buffer_data[i] = readEEPROM_1byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data[i], HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data[i], DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data[i]);
      address++;
    }

    long unsigned int new_Butterfly_ID = 0x00000000;

    new_Butterfly_ID = new_Butterfly_ID + (buffer_data[0] << 16);
    new_Butterfly_ID = new_Butterfly_ID + (buffer_data[1] << 8);
    new_Butterfly_ID = new_Butterfly_ID + (buffer_data[2]);

    if (new_Butterfly_ID != Butterfly_ID)
    {
      Colour1 = 0x00000000;
      Colour2 = 0x00000000;
      Colour3 = 0x00000000;
      Colour4 = 0x00000000;
      Colour5 = 0x00000000;
      Butterfly_ID_changed_flag = 1;
      Butterfly_ID = new_Butterfly_ID;
    }

    Colour1 = (buffer_data[0] << 16);
    Colour1 = Colour1 + (buffer_data[1] << 8);
    Colour1 = Colour1 + (buffer_data[2]);

    Colour2 = (buffer_data[3] << 16);
    Colour2 = Colour2 + (buffer_data[4] << 8);
    Colour2 = Colour2 + (buffer_data[5]);

    Colour3 = (buffer_data[6] << 16);
    Colour3 = Colour3 + (buffer_data[7] << 8);
    Colour3 = Colour3 + (buffer_data[8]);

    Colour4 = (buffer_data[9] << 16);
    Colour4 = Colour4 + (buffer_data[10] << 8);
    Colour4 = Colour4 + (buffer_data[11]);

    Colour5 = (buffer_data[12] << 16);
    Colour5 = Colour5 + (buffer_data[13] << 8);
    Colour5 = Colour5 + (buffer_data[14]);
  }
  else Butterfly_ID = 0;
}

byte readEEPROM_1byte(int deviceaddress, unsigned int eeaddress )
{
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1); // more than 6 crashes? unknown reason

  int i = 0;
  while (Wire.available())
  {
    rdata = Wire.read();
  }

  return rdata;
}
