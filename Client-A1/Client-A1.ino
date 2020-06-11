/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * Client A(ESP32)
 * This code responsible for borrow umbrella
 * "Borrow" divided into two stages(A and B)
 * A is responsible for deducting money from RFID card and tell server about the rental status
 * 
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include "RFID.h"
#include "post.h"
#include "Seg7.h"

//Raspberry AP Wifi
const char* ssid = "UMS-001"; 
const char* password = "12345678";

#define Relay 17

#define LED_R  2
#define LED_G  0
#define LED_B  4

void setup() 
{
  Serial.begin(9600);
  
  pinMode(Relay,OUTPUT);
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);

  display.setBrightness(0x0F);
  display.setSegments(status_no_wifi);
  digitalWrite(LED_R,HIGH);
  
  WiFi.begin(ssid,password);
  WiFi.setHostname("A1");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WIFI Connected!");
  
  RFID_init();
  
  digitalWrite(Relay,HIGH);
  
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);
  display.setSegments(status_0);
  Serial.println("Init Success");
}

void loop() 
{
  if(WiFi.status()== WL_CONNECTED)
  {
     bool Check_sb_borrow_Data = Check_sb_borrow();
     if(Check_sb_borrow_Data == 1)
     {
        digitalWrite(Relay ,LOW);
        delay(5000);
        digitalWrite(Relay,HIGH);
     }
     else
     {
        Detect_RFID();
     }
     delay(300);
  }
  else
  {
    digitalWrite(LED_R,HIGH);
    digitalWrite(LED_G,LOW);
    Serial.println("Network Error"); 
    reconnect(); 
  }
  delay(1000);
}

// I use sector1 block0(block 4) byte1 and byte2 to save money value
// String byte1 + string byte2 = money 
// If byte1 = FF(255),byte2 = FF(255) and money = FFFF(65535))
void Detect_RFID()
{
  if ( ! RFID.PICC_IsNewCardPresent()){return;}

  read_RFID();
  bool   Card_inquire_Data = Card_inquire(Card_code);
  bool   User_check_Data   = User_check(Card_code);
  String Upload_data_Data  = "";
  
  if(Card_inquire_Data == 1)
  {
    if(User_check_Data == 0)
    {
      display.setSegments(status_1);
      digitalWrite(LED_G , LOW);
      digitalWrite(LED_B ,HIGH);
      long money = read_RFID_sector();
      //Borrow need deduct 60NTD
      money -= 60;
      delay(500);
      write_rfid(money);

      Serial.print("last money:");
      Serial.println(money);
      
      Upload_data_Data = Upload_data(Card_code);
      
      //Get money last three digits to show on seg7
      String money_array[4];
      money_array[0] = String(money);
      money_array[1] = money_array[0].substring(money_array[0].length()-1);
      money_array[2] = money_array[0].substring(money_array[0].length()-2,money_array[0].length()-1);
      money_array[3] = money_array[0].substring(money_array[0].length()-3,money_array[0].length()-2);

      //String to seg byte
      uint8_t money_seg[3];
      money_seg[0] = Find_Seg(money_array[1]);
      money_seg[1] = Find_Seg(money_array[2]);
      money_seg[2] = Find_Seg(money_array[3]);
      
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_G,HIGH);
      display.setSegments(status_S);
      delay(1000);

      uint8_t money_seg_data[] = {0x38,money_seg[2],money_seg[1],money_seg[0]}; //LNNN N=money-3
      display.setSegments(money_seg_data);
      delay(3000);

      display.setSegments(status_0);
    }
    else
    {
      display.setSegments(status_b);
      delay(3000);
      display.setSegments(status_0);
    }
  }
  else
  {
     display.setSegments(status_n);
     delay(3000);
     display.setSegments(status_0);
  }

  RFID.PICC_HaltA();
  RFID.PCD_StopCrypto1();
}

void reconnect() 
{
    Serial.print("Reconnecting..."); 
    display.setSegments(status_no_wifi);
    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED)
    {  
        delay(500);  
        Serial.print(".");
    }  
    Serial.println();
    Serial.println("WIFI Connected!");
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G,HIGH);
}  
