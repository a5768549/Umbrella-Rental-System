/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN    4
#define RST_PIN   5

String um_code;

MFRC522 RFID(SS_PIN, RST_PIN);

void RFID_init()
{
  SPI.begin();
  RFID.PCD_Init();
}

void dump_byte_array(byte *buffer, byte bufferSize) 
{
  String um_code_temp[4];
  um_code = "";
  for (byte i = 0; i < bufferSize; i++) 
  {
    um_code_temp[i] = buffer[i];
    um_code  += um_code_temp[i];
  }
  Serial.println(um_code);
  Serial.println();
  
}

void read_RFID()
{
  if ( ! RFID.PICC_ReadCardSerial()) 
  {
    delay(50);
    return;
  }
  dump_byte_array(RFID.uid.uidByte, RFID.uid.size);
}
