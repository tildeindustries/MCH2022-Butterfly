#include <Wire.h>
#define disk1 0x50    //Address of 24LC256 eeprom chip

#define Butterfly_ID  0x00123456
#define Colour1 0x00058240
#define Colour2 0x0087C159
#define Colour3 0x00ADCACB
#define Colour4 0x00FEE3A2
#define Colour5 0x00F3C301
#define White  0x00FFFFFF

byte SAO_ID_header[8] = {0, 0, 0, 0, 0, 0, 0, 0};


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Starting");
  Wire.begin();

  unsigned int address = 0;
  
      writeEEPROM(disk1, address, 'L'); address++;
      writeEEPROM(disk1, address, 'I');address++;
      writeEEPROM(disk1, address, 'F');address++;
      writeEEPROM(disk1, address, 'E');address++;

      writeEEPROM(disk1, address, 0x0C);address++;
      writeEEPROM(disk1, address, 0x0F);address++;
      writeEEPROM(disk1, address, 0x12);address++;
      writeEEPROM(disk1, address, 0x00);address++;

      writeEEPROM(disk1, address, 'M');address++;
      writeEEPROM(disk1, address, 'C');address++;
      writeEEPROM(disk1, address, 'H');address++;
      writeEEPROM(disk1, address, 'b');address++;
      writeEEPROM(disk1, address, 'u');address++;
      writeEEPROM(disk1, address, 't');address++;
      writeEEPROM(disk1, address, 't');address++;
      writeEEPROM(disk1, address, 'e');address++;
      writeEEPROM(disk1, address, 'r');address++;
      writeEEPROM(disk1, address, 'f');address++;
      writeEEPROM(disk1, address, 'l');address++;
      writeEEPROM(disk1, address, 'y');address++;

      writeEEPROM(disk1, address, 'M');address++;
      writeEEPROM(disk1, address, 'C');address++;
      writeEEPROM(disk1, address, 'H');address++;
      writeEEPROM(disk1, address, 'b');address++;
      writeEEPROM(disk1, address, 'u');address++;
      writeEEPROM(disk1, address, 't');address++;
      writeEEPROM(disk1, address, 't');address++;
      writeEEPROM(disk1, address, 'e');address++;
      writeEEPROM(disk1, address, 'r');address++;
      writeEEPROM(disk1, address, 'f');address++;
      writeEEPROM(disk1, address, 'l');address++;
      writeEEPROM(disk1, address, 'y');address++;
      writeEEPROM(disk1, address, '.');address++;
      writeEEPROM(disk1, address, 'p');address++;
      writeEEPROM(disk1, address, 'y');address++;

      writeEEPROM(disk1, address, (uint8_t)((Butterfly_ID & 0x00FF0000) >> 16));address++;
      writeEEPROM(disk1, address, (uint8_t)((Butterfly_ID & 0x0000FF00) >> 8));address++;
      writeEEPROM(disk1, address, (uint8_t)(Butterfly_ID & 0x000000FF));address++;

      writeEEPROM(disk1, address, (uint8_t)((Colour1 & 0x00FF0000) >> 16));address++;
      writeEEPROM(disk1, address, (uint8_t)((Colour1 & 0x0000FF00) >> 8));address++;
      writeEEPROM(disk1, address, (uint8_t)(Colour1 & 0x000000FF));address++;

      writeEEPROM(disk1, address, (uint8_t)((Colour2 & 0x00FF0000) >> 16));address++;
      writeEEPROM(disk1, address, (uint8_t)((Colour2 & 0x0000FF00) >> 8));address++;
      writeEEPROM(disk1, address, (uint8_t)(Colour2 & 0x000000FF));address++;
      
      writeEEPROM(disk1, address, (uint8_t)((Colour3 & 0x00FF0000) >> 16));address++;
      writeEEPROM(disk1, address, (uint8_t)((Colour3 & 0x0000FF00) >> 8));address++;
      writeEEPROM(disk1, address, (uint8_t)(Colour3 & 0x000000FF));address++;
      
      writeEEPROM(disk1, address, (uint8_t)((Colour4 & 0x00FF0000) >> 16));address++;
      writeEEPROM(disk1, address, (uint8_t)((Colour4 & 0x0000FF00) >> 8));address++;
      writeEEPROM(disk1, address, (uint8_t)(Colour4 & 0x000000FF));address++;

      writeEEPROM(disk1, address, (uint8_t)((Colour5 & 0x00FF0000) >> 16));address++;
      writeEEPROM(disk1, address, (uint8_t)((Colour5 & 0x0000FF00) >> 8));address++;
      writeEEPROM(disk1, address, (uint8_t)(Colour5 & 0x000000FF));address++;

      writeEEPROM(disk1, address, 'E');address++;
      writeEEPROM(disk1, address, 'N');address++;
      writeEEPROM(disk1, address, 'D');address++;
      
  address = 0; // resets address
  
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

byte device_name [SAO_ID_header[4]];
byte driver_name [SAO_ID_header[5]];
byte data_buffer [SAO_ID_header[6]];

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
    data_buffer[i] = readEEPROM_1byte(disk1, address);
    Serial.print("data received HEX: ");
    Serial.print(data_buffer[i], HEX);
    Serial.print(" DEC: ");
    Serial.print(data_buffer[i], DEC);
    Serial.print(" ASCII: ");
    Serial.println((char)data_buffer[i]);
    address++;
  }



  
/*
  if (SAO_ID_header[0] == 'L', SAO_ID_header[1] == 'I', SAO_ID_header[2] == 'F', SAO_ID_header[3] == 'E')
  {
    Serial.println("1 byte address EEPROM detected");
    Serial.println("Device name:");
    for (int i = 0; i < SAO_ID_header[4]; i++)
    {
      byte buffer_data = readEEPROM_1byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data, HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data, DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data);
      address++;
    }
    Serial.println("Driver name:");
    for (int i = 0; i < SAO_ID_header[5]; i++)
    {
      byte buffer_data = readEEPROM_1byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data, HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data, DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data);
      address++;
    }
    Serial.println("Driver data:");
    for (int i = 0; i < SAO_ID_header[6]; i++)
    {
      byte buffer_data = readEEPROM_1byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data, HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data, DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data);
      address++;
    }
  }
  else Serial.println("No 1 byte address found");



  address = 0; // reset address to start reading from address 0 of EEPROM

  Serial.println("Attempting to read 2 byte address EEPROM");
  for (int i = 0; i < 8; i++)
  {
    SAO_ID_header[i] = readEEPROM_2byte(disk1, address);
    Serial.print("data received HEX: ");
    Serial.print(SAO_ID_header[i], HEX);
    Serial.print(" DEC: ");
    Serial.print(SAO_ID_header[i], DEC);
    Serial.print(" ASCII: ");
    Serial.println((char)SAO_ID_header[i]);
    address++;
  }

  if (SAO_ID_header[0] == 'L', SAO_ID_header[1] == 'I', SAO_ID_header[2] == 'F', SAO_ID_header[3] == 'E')
  {
    Serial.println("2 byte address detected");
    Serial.println("Device name:");
    for (int i = 0; i < SAO_ID_header[4]; i++)
    {
      byte buffer_data = readEEPROM_2byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data, HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data, DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data);
      address++;
    }
    Serial.println("Driver name:");
    for (int i = 0; i < SAO_ID_header[5]; i++)
    {
      byte buffer_data = readEEPROM_2byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data, HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data, DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data);
      address++;
    }
    Serial.println("Driver data:");
    for (int i = 0; i < SAO_ID_header[6]; i++)
    {
      byte buffer_data = readEEPROM_2byte(disk1, address);
      Serial.print("data received HEX: ");
      Serial.print(buffer_data, HEX);
      Serial.print(" DEC: ");
      Serial.print(buffer_data, DEC);
      Serial.print(" ASCII: ");
      Serial.println((char)buffer_data);
      address++;
    }
  }*/
   Serial.println("End");
}

void loop() {
  while (1)
  delay (1000);
 }

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(5);
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

byte readEEPROM_2byte(int deviceaddress, unsigned int eeaddress )
{
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(deviceaddress, 1);

  while (Wire.available())
  {
    rdata = Wire.read();
  }

  return rdata;
}
