/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * Client C(NodeMCU V3)
 * This code responsible for return umbrella
 * "Return" divided into two stages(C and D)
 * C is responsible for check umbrella return status and tell server about the rental status
 * 
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include "RFID.h"
#include "post.h"

//Raspberry AP Wifi
const char* ssid = "UMS-001";
const char* password = "12345678";

#define LED_R  D0
#define LED_G  D3
#define LED_B  D4

void setup() 
{
  Serial.begin(9600);

  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,LOW);

  digitalWrite(LED_R,HIGH);
  WiFi.begin(ssid,password);
  WiFi.hostname("C1");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  RFID_init();
  
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_G,HIGH);
  Serial.println("Init Success");
}

void loop() 
{
  if(WiFi.status()== WL_CONNECTED)
  {
     Detect_RFID();
  }
  else
  {
    digitalWrite(LED_R,HIGH);
    digitalWrite(LED_G,LOW);
    Serial.println("Network Error");
    reconnect();
  }
  delay(800);
}

// Detect RFID label on umbrella
void Detect_RFID()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {return;}
  digitalWrite(LED_G,LOW);
  digitalWrite(LED_B,HIGH);
  delay(300);
  read_RFID();
  Upload_um_data(um_code);
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  digitalWrite(LED_B,LOW);
  digitalWrite(LED_G,HIGH);
}

void reconnect() 
{
    Serial.print("Reconnecting..."); 
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
