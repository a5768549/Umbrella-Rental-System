/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN    21
#define RST_PIN   22

String Card = "";

MFRC522 RFID(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

void RFID_init()
{
  SPI.begin();
  RFID.PCD_Init();

  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
  }
  
}
void dump_byte_array(byte *buffer, byte bufferSize) 
{
  Card = "";
  String code_temp[4];
  for (byte i = 0; i < bufferSize; i++) 
  {
    code_temp[i] = buffer[i];
    Card += code_temp[i];
  }
  Serial.println(Card);
}

//byte to string,string to char,char to long
long money_dec(byte *buffer ,byte bufferSize)
{
  String tmp[3];
  for (byte i = 0; i < bufferSize; i++) 
  {
    tmp[i] = String(buffer[i], HEX);
  }
  tmp[3] = tmp[0] + tmp[1]; 
  long money = strtol(tmp[3].c_str(), NULL, 16);
  return money;
}

//Get RFID uuid
void read_RFID()
{
  if ( ! RFID.PICC_ReadCardSerial()) 
  {
    delay(50);
    return;
  }
  Serial.println("RFID Tag Detected...");
  Serial.print(F("Card UID:"));
  dump_byte_array(RFID.uid.uidByte, RFID.uid.size);
}

//Get RFID sector
long read_RFID_sector()
{
    byte sector         = 1;
    byte blockAddr      = 4;
    
    byte buffer[18];
    byte size = sizeof(buffer);
    RFID.PICC_DumpMifareClassicSectorToSerial(&(RFID.uid), &key, sector);
    Serial.println();
    //read
    RFID.MIFARE_Read(blockAddr, buffer, &size);
    Serial.print(F("Data in block "));
    Serial.print(blockAddr);
    Serial.println(F(":"));
    long money = dump_byte_array_RW(buffer, 2);
    Serial.println();

    return money;
}


void writeBlock(byte _sector, byte _block, byte _blockData[])
{
  byte blockNum = _sector * 4 + _block;  // 計算區塊的實際編號（0~63）
  byte trailerBlock = _sector * 4 + 3;   // 控制區塊編號
 
  status = (MFRC522::StatusCode) RFID.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(RFID.uid));
  
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(RFID.GetStatusCodeName(status));
    return;
  }
 
  status = (MFRC522::StatusCode) RFID.MIFARE_Write(blockNum, _blockData, 16);
  
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(RFID.GetStatusCodeName(status));
    return;
  }
 
  Serial.println(F("Data was written."));
}
int write_rfid(int money)
{
  byte sector     = 1;
  byte block      = 0;
  String tmp[3];
  tmp[2] =  String(money, HEX);
  tmp[0] = tmp[2].substring(0,2);
  tmp[1] = tmp[2].substring(2);
  
  byte tmp1 = strtol(tmp[0].c_str(), NULL, 16);
  byte tmp2 = strtol(tmp[1].c_str(), NULL, 16);
  byte dataBlock[]    =
  {
      tmp1, tmp2, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00
  };

  writeBlock(sector, block, dataBlock);
}
