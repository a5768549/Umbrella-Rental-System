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

String Check_sb_return()
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "R";
  post_data["device"] = "D1";
  post_data["action"] = "Check_sb_return";
  post_data["data"]   = "no";

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(json);
  String payload = http.getString();
  
  Serial.println(httpCode);
  Serial.println(payload);

  http.end();

  return payload;
}

int Check_card(String Card_Number)
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "R";
  post_data["device"] = "D1";
  post_data["action"] = "Check_card";
  post_data["data"]   = Card_Number;

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(json);
  String payload = http.getString();
  
  Serial.println(httpCode);
  Serial.println(payload);

  http.end();

  return payload.toInt();
}

int Use_money()
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "R";
  post_data["device"] = "D1";
  post_data["action"] = "Use_money";
  post_data["data"]   = "no";

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(json);
  String payload = http.getString();
  
  Serial.println(httpCode);
  Serial.println(payload);

  http.end();

  return payload.toInt();
}


String Upload_data()
{
  DynamicJsonDocument post_data(2048);
  post_data["type"]   = "R";
  post_data["device"] = "D1";
  post_data["action"] = "Upload_data";
  post_data["data"]   = "no";

  String json;
  serializeJson(post_data, json);
  HTTPClient http;
 
  http.begin(host + "api.php"); 
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(json);
  String payload = http.getString();
  
  Serial.println(httpCode);
  Serial.println(payload);

  http.end();

  return payload;
}
