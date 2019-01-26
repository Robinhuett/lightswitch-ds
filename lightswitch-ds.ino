#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <WiFiUdp.h>

#include "secrets.h"

/*
Upload Settings:
1M (No SPIFFS)
*/

// Common Settings
const char*  common_name   = "rbn-esp-lightswitch-01"; // Give it a name, like it's a pet.

// WiFi Settings
const char*  wifi_ssid     = secret_wifi_ssid; // Even a pet needs WiFi..
const char*  wifi_password = secret_wifi_ssid; // ..and access to it.
WiFiClient   wifi_client;

// MQTT Settings
const char*  mqtt_server   = "mqtt"; // Also your pet needs someone to play with.
const char*  mqtt_topic    = "places/gutenbergwg/robin/switches/light/set"; // And a place where to play.
const char*  mqtt_payload  = "toggle"; // As well as info on how to play.
PubSubClient mqtt_client(wifi_client);

// OTA Settings
const char*  ota_password  = secret_ota_password; // Your pet needs to be secure!
int          ota_pin       = 2;
bool         ota_lock      = false;

// DeepSleep Settings
int          sleep_base    = -1;
const int    sleep_timeout = 5000;
Ticker       sleep_ticker;

void setup() {
  sleep_ticker.once_ms(sleep_timeout * 3, &initSleepHard);
  //Serial.begin(115200);
  // Init Input
  pinMode(ota_pin, INPUT);
  //Serial.println("Init WiFi");
  initWiFi();
  //Serial.println("Make Request");
  makeMQTTRequest();
  //Serial.println("Init OTA");
  initOTA();
}

void loop() {
  if (sleep_base < 0) {
    //Serial.println("Into da loop");
    sleep_base = millis();
  }

  if (millis() > (sleep_base + sleep_timeout)) {
    initSleep(false);
  }

  if(digitalRead(ota_pin) != 1) {
    //Serial.println("OTA Lock");
    ota_lock = true;
  }

  mqtt_client.loop();
  ArduinoOTA.handle();

  delay(sleep_timeout / 10);
}

// Establish a Wi-Fi connection with your router
void initWiFi() {
  if (WiFi.SSID() != wifi_ssid) {
    //Serial.println("Stuff to do");
    WiFi.mode(WIFI_STA);
    WiFi.hostname(common_name);
    WiFi.begin(wifi_ssid, wifi_password);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
  }
  
  int wifi_timeout = sleep_timeout * 2;
  while(WiFi.status() != WL_CONNECTED  && (wifi_timeout-- > 0)) {
    delay(250);
  }

  if(WiFi.status() != WL_CONNECTED) {
     //Serial.println("Failed to connect");
  }
}

void initOTA() {
  ArduinoOTA.setHostname(common_name);
  ArduinoOTA.setPassword(ota_password);
  ArduinoOTA.onStart([]() {
    //Serial.println("OTA Update");
    ota_lock = true;
  });
  ArduinoOTA.begin();
}

void makeMQTTRequest() {
  mqtt_client.setServer(mqtt_server, 1883);

  while (!mqtt_client.connected()) {
    if (mqtt_client.connect(common_name)) { }
    else {
      delay(250);
    }
  }

  mqtt_client.publish(mqtt_topic, mqtt_payload);
}

void initSleep(bool hard) {
  if (!ota_lock) {
    // If this sleep happened because of timeout, clear the
    // Wifi settings.  (Maybe the AP channel changed, etc)
    if (hard) {
      //Serial.println("Disconnecting WiFi");
      WiFi.disconnect();
    }
    
    //Serial.println("Go to sleep");
    ESP.deepSleep(0);
  }
}

void initSleepHard() {
  initSleep(true);
}
