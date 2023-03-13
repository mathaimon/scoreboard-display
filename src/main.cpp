#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

#include <IndicatorLights.h>

// Indicator LED's
int redLed = 2;
int greenLed = 0;
int blueLed = 4;

IndicatorLights indicator(redLed, greenLed, blueLed);

const char* ssid = "Wireless AP";
const char* password = "12345678";

//Your Domain name with URL path or IP address with path
const char* serverUrl = "http://192.168.1.223:8000/combined";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// 30 Seconds Delay
unsigned long timerDelay = 30000;

String scores;
int scoresArray[9];


String httpGETRequest(const char* serverUrl) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverUrl);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free the resources
  http.end();

  return payload;
}


void convertToArray(int num){
  int temp = num;
  Serial.println("Inside Function");
  for(int i = 8; i>=0; i--){
    scoresArray[i] = temp%10;
    temp /= 10;
    // Serial.println(temp);
  }
}


void setup() {
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}


void loop() {
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
    
      Serial.print("JSON object = ");
      Serial.println(scoresObj);
    
      // scoresObj.keys() can be used to get an array of all the keys in the object
      JSONVar keys = scoresObj.keys();
    
        JSONVar value = scoresObj[keys[0]];
        Serial.println(value);
        convertToArray(int(value));

        // Print Values in array
        Serial.print("Scores Array : ");
        for(int i=0; i<=8;i++){
          Serial.print(scoresArray[i]);
          Serial.print(",");
        }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

