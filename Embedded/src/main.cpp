#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

#define lightswitch 23
#define fanswitch 22
//#define hwswitch 21



/*bool ledonoff[8][3]={
  {false,false,false},
  {false,false,true},
  {false,true,false},
  {false,true,true},
  {true,false,false},
  {true,false,true},
  {true,true,false},
  {true,true,true}
};     
void switchonoff(int i)
{
        digitalWrite(bedswitch,ledonoff[i][0]);
        digitalWrite(lrswitch,ledonoff[i][1]);
        digitalWrite(hwswitch,ledonoff[i][2]);
}*/
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

        float randnum;
        randnum=random(21,33);
      
        doc["temperature"] = randnum;
        //doc["light_switch_2"] = ledonoff[i][1];
        //doc["light_switch_3"] = ledonoff[i][2];
  
      serializeJson(doc, httpRequestData);


      // Send HTTP POST request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;

      // check result of POST request. negative response code means server wasn't reached
     
   
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      http_response = http.getString();
      Serial.println(http_response);
      
      // Free resources
      http.end();

      delay(2000);

      }
    
  }
  else {
    
        Serial.print("Error: ");
        //Serial.println(httpResponseCode);
    //return;
    Serial.println("WiFi Disconnected");
  }
}