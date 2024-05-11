#define LARGE_JSON_BUFFERS 1
#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ticker.h>

#include "logging.h"
#include "settings.h"
#include "ota.h"
#include "wifi.h"
#include "webthing.h"

Ticker updateThingTimer;

void setup() {
  // outputs
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, RELAY_OFF);
  digitalWrite(LED_PIN, LED_ON);
  
  delay(1000);

  Serial.begin(9600);
  while(!Serial && !Serial.available()){}
  INFO_PRINTLN("\n");

  String deviceName(DEVICE_NAME);
  deviceName.concat("-");
  deviceName.concat(ESP.getChipId());
  deviceName.toLowerCase();
  
  setupWiFi(deviceName);

  DEBUG_PRINTLN("OK");
  DEBUG_PRINTLN("");

  digitalWrite(LED_PIN, LED_OFF);

  INFO_PRINT("IP address: ");
  INFO_PRINTLN(WiFi.localIP());

  setupWebThing(deviceName);

  setupOTA(deviceName.c_str());

  updateThingTimer.attach_ms(THING_ADAPTER_UPDATE_MS, updateThing);
}

void loop() {
  ArduinoOTA.handle();
}