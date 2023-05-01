#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

#include <IndicatorLights.h>
#include <DisplayBoard.h>

// Indicator LED's
int redLed = 2;
int greenLed = 0;
int blueLed = 4;

// Display Board
int latchPin = 12; // ST_CP
int clockPin = 14; // SH_CP
int dataPin = 13; // DS

IndicatorLights indicator(redLed, greenLed, blueLed);
DisplayBoard displayBoard(latchPin, clockPin, dataPin);

const char* ssid = "Wireless AP";
const char* password = "123456789000";

//Your Domain name with URL path or IP address with path
const char* serverUrl = "https://scoreboard-api-flask.vercel.app/combined";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// 30 Seconds Delay
unsigned long timerDelay = 10000;

String scores;


String httpGETRequest(const char* serverUrl) {
  indicator.blue();

  // WiFiClient client;
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
    
  // Your IP address with path or Domain name with URL path 
  https.begin(*client, serverUrl);
  
  // Send HTTP POST request
  int httpResponseCode = https.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free the resources
  https.end();

  return payload;
}

void setup() {
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    indicator.red();
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 30 seconds (timerDelay variable), it will take 30 seconds before publishing the first reading.");
}


void loop() {
  // Set Indicator colors
  if(WiFi.status()==WL_CONNECTED){
    indicator.green();
  }else{
    indicator.red();
  }

  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      indicator.green();
      scores = httpGETRequest(serverUrl);
      Serial.println(scores);
      JSONVar scoresObj = JSON.parse(scores);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(scoresObj) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      // Serial.print("JSON object = ");
      // Serial.println(scoresObj);
    
      // scoresObj.keys() can be used to get an array of all the keys in the object
      JSONVar keys = scoresObj.keys();

      // Debug code
      // Serial.print("Key Length : ");
      // Serial.println(sizeof(keys));
      // Serial.print("Key : ");
      // Serial.println(keys);

      JSONVar value = scoresObj[keys[0]];
      Serial.println(value);
      displayBoard.show(int(value));
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

