#include <WiFi.h>
#include <HTTPClient.h>
#include "base64.h"

#define BUTTON_PIN 18

int lastState = LOW;  // the previous state from the input pin
int buttonState;      // the current reading from the input pin

const char WIFI_SSID[] = "abcd";//Your Wifi Name Here
const char WIFI_PASSWORD[] = "abcd";//Your wifi password here

String authUsername = "admin";//name for loxone config
String authPassword = "admin";//password for loxone config

String HOST_NAME = "http://10.0.0.209";//miniserver's IP adress
String PATH_NAME_ON = "/dev/sps/io/Relay/On";//
String PATH_NAME_OFF = "/dev/sps/io/Relay/Off";
String queryString = "";

void setup() {
  pinMode(18, INPUT_PULLUP);
  Serial.begin(9600);
//connecting to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());





  // httpCode will be negative on error
}
void loop() {
  HTTPClient http;
  // read the state of the switch/button:
  buttonState = digitalRead(BUTTON_PIN);
//depends on the buttons state atm, sends signals
  if (lastState == HIGH && buttonState == LOW) {
    http.begin(HOST_NAME + PATH_NAME_OFF + queryString);  //HTTP
    String auth = base64::encode(authUsername + ":" + authPassword);
    http.addHeader("Authorization", "Basic " + auth);
    int httpCode = http.GET();
    lastState = LOW;
    if (httpCode > 0) {
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      } else {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else if (lastState == LOW && buttonState == LOW) {
    http.begin(HOST_NAME + PATH_NAME_ON + queryString);  //HTTP
    String auth = base64::encode(authUsername + ":" + authPassword);
    http.addHeader("Authorization", "Basic " + auth);
    int httpCode = http.GET();
    lastState = HIGH;
    if (httpCode > 0) {
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      } else {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }


  delay(100);
}