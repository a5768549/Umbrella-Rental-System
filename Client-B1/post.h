/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//Raspberry php server(Intranet)
String host = "http://10.3.141.1/UMS/";

void Upload_um_data(String Card_Number)
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "B";
  post_data["device"] = "B1";
  post_data["action"] = "Upload_um_data";
  post_data["data"]   = Card_Number;

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");
      
  int httpCode = http.POST(json);
  String payload = http.getString();
  
  Serial.println(payload);
 
  http.end();
}
