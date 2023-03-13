#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

#define lightswitch 23
#define fanswitch 22

void setup() {
  Serial.begin(9600);
	  pinMode(fanswitch, OUTPUT);
    pinMode(lightswitch, OUTPUT);
  //pinMode(hwswitch, OUTPUT);
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_USER, WIFI_PASS,6);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  float randnum;
        randnum=random(21,33);
    // Establish a connection to the server
    http.begin(API_URL);
    
    // Specify content-type header
    //for(int i=0; i<8; i++){
      

      http.addHeader("Content-Type", "application/json");
     // http.addHeader("X-API-Key", API_KEY);
      //http.addHeader("Content-length", "76");

      // Serialise JSON object into a string to be sent to the API
      StaticJsonDocument<72> doc;
      String httpRequestData;
      
        //switchonoff(i);

        
      
        doc["temperature"] = randnum;
        //doc["light_switch_2"] = ledonoff[i][1];
        //doc["light_switch_3"] = ledonoff[i][2];
  
      serializeJson(doc, httpRequestData);


      // Send HTTP POST request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;

      // check result of POST request. negative response code means server wasn't reached
     
   if (httpResponseCode>0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      http_response = http.getString();
      Serial.println(http_response);
   }
   else {
    
        Serial.print("Error: ");
        Serial.println(httpResponseCode);
    //return;
    Serial.println("WiFi Disconnected");
  }
      // Free resources
      http.end();

      delay(2000);

     http.begin(API_URL);
    //http.addHeader("X-API-KEY","Emma#4451");
   

    int httpResponseCode = http.GET();

    if (httpResponseCode>0){
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.print(http_response);
    }
  else{
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    //return;
  }
  http.end();

  StaticJsonDocument<1024> docc;

  DeserializationError error = deserializeJson(docc, http_response);
  if (error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
      }

      bool temperature=docc["fan"];
      bool light=docc["light"];
      digitalWrite(fanswitch,temperature);
      digitalWrite(lightswitch,light);

      http.end();
      }
    else {
    
        Serial.print("Error: ");
        //Serial.println(httpResponseCode);
    //return;
    Serial.println("WiFi Disconnected");
  }
  }
  
