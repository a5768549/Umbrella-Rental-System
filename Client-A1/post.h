/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Raspberry php server(Intranet)
String host = "http://10.3.141.1/UMS/";

int Card_inquire(String Card_Number)
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "B";
  post_data["device"] = "A1";
  post_data["action"] = "Card_inquire";
  post_data["data"]   = Card_Number;

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int    httpCode = http.POST(json);
  String payload  = http.getString();
  
  Serial.println(payload);

  http.end();
  int get_data = payload.toInt();
  return get_data;
}

int User_check(String Card_Number)
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "B";
  post_data["device"] = "A1";
  post_data["action"] = "User_check";
  post_data["data"]   = Card_Number;

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int httpCode   = http.POST(json);
  String payload = http.getString();
  
  Serial.println(payload);

  http.end();
  int get_data = payload.toInt();
  return get_data;
}

String Upload_data(String Card_Number)
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "B";
  post_data["device"] = "A1";
  post_data["action"] = "Upload_data";
  post_data["data"]   = Card_Number;

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int httpCode   = http.POST(json);
  String payload = http.getString();
  
  Serial.println(payload);

  http.end();
  return payload;
}


int Check_sb_borrow()
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "B";
  post_data["device"] = "A1";
  post_data["action"] = "Check_sb_borrow";
  post_data["data"]   = "no";

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int    httpCode = http.POST(json);
  String payload  = http.getString();
  
  Serial.println(payload);

  http.end();
  int get_data = payload.toInt();
  return get_data;
}
