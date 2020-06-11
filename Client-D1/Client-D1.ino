/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * Client D(ESP32)
 * This code responsible for return umbrella
 * "Return" divided into two stages(C and D)
 * D is responsible for deducting money from RFID card and tell server about the rental status
 * 
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include "RFID.h"
#include "post.h"
#include "Seg7.h"

//Raspberry AP Wifi
const char* ssid = "UMS-001";
const char* password = "12345678";

int check_flag = 0;
String Card_code;
String Check_sb_return_Data;

#define LED_R  2
#define LED_G  0
#define LED_B  4

void setup() 
{
  Serial.begin(9600);

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
  WiFi.setHostname("D1");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WIFI Connected!");
  
  RFID_init();

  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);
  
  display.setSegments(status_0);
  Serial.println("Init Success");
}

void loop() 
{
  if(WiFi.status()== WL_CONNECTED)
  {
     if(check_flag == 0)
     {
        Check_sb_return_Data = Check_sb_return();
        Card_code = Check_sb_return_Data;
     }
     if(Check_sb_return_Data != "no")
     {
        check_flag = 1;
     }
     if(check_flag == 1)
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
  if ( ! RFID.PICC_IsNewCardPresent()) {return;}
  
  read_RFID();
  int Check_card_Data = Check_card(Card);
  Serial.println(Check_card_Data);
  
  if(Check_card_Data == 1)
  {
    long money = read_RFID_sector();
    long _use_money = Use_money();

    digitalWrite(LED_G , LOW);
    digitalWrite(LED_B ,HIGH);
      
    Serial.print("last money:");
    Serial.println(money);
    //Return need add 60NTD and deduct use money
    money = money + 60 - _use_money;
    write_rfid(money);
    
    Upload_data();
    check_flag = 0;

    //Get money last three digits to show on seg7
    String money_array[4];
    money_array[0] = String(money);
    money_array[1] = money_array[0].substring(money_array[0].length()-1);
    money_array[2] = money_array[0].substring(money_array[0].length()-2,money_array[0].length()-1);
    money_array[3] = money_array[0].substring(money_array[0].length()-3,money_array[0].length()-2);
      
    Serial.println( ":" + money_array[2]);
    Serial.println();

    //String to seg byte
    uint8_t money_seg[3];
    money_seg[0] = Find_Seg(money_array[1]);
    money_seg[1] = Find_Seg(money_array[2]);
    money_seg[2] = Find_Seg(money_array[3]);

    digitalWrite(LED_B,LOW);
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
    Serial.println("wrong card");
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
    digitalWrite(LED_R,LOW);
    digitalWrite(LED_G,HIGH);
} 
