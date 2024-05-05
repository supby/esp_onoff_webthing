#define LARGE_JSON_BUFFERS 1
#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>

// webthings
#include <WebThingAdapter.h>
#include <Thing.h>

// Network
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>


#include "logging.h"
#include "settings.h"
#include "ota.h"



WebThingAdapter* adapter;

const char* relayTypes[] = {"OnOffSwitch", nullptr};
ThingDevice relay("relay", DEVICE_NAME, relayTypes);
ThingProperty relayOn("on", "Whether the relay is turned on", BOOLEAN, "OnOffProperty");

bool lastOn = false;


void setupWebThing(String deviceName) {
  adapter = new WebThingAdapter(deviceName, WiFi.localIP(), 80, true);
  relay.addProperty(&relayOn);
  adapter->addDevice(&relay);
  adapter->begin();

  INFO_PRINT("Thing URL: http://");
  INFO_PRINT(deviceName);
  INFO_PRINT(".local/things/");
  INFO_PRINTLN(relay.id);
  INFO_PRINTLN("");
}

void setupWiFi(String deviceName) {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName.c_str());
  WiFi.setAutoReconnect(true);

  bool blink = true;
  if (WiFi.SSID() == "") {
    WiFi.beginSmartConfig();

    while (!WiFi.smartConfigDone()) {
      delay(1000);
      digitalWrite(LED_PIN, blink ? HIGH : LOW);
      blink = !blink;
    }
  }

  WiFi.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_PIN, blink ? HIGH : LOW);
    blink = !blink;
  }

  WiFi.stopSmartConfig();

  digitalWrite(LED_BUILTIN, HIGH);
}

void checkProp() {
  bool enabled = relayOn.getValue().boolean;
  if (enabled != lastOn) {
    DEBUG_PRINTLN("Adapter will change relay state");
    
    digitalWrite(RELAY_PIN, enabled ? RELAY_ON : RELAY_OFF);
    digitalWrite(LED_PIN, enabled ? LED_ON : LED_OFF);

    DEBUG_PRINT_VAR("Relay state has changed: ", enabled);
  }
  lastOn = enabled;
  
  adapter->update();
}

void setup() {

  delay(1000);
  Serial.begin(115200);
  INFO_PRINTLN("\n");

  // outputs
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, RELAY_OFF);


  digitalWrite(LED_PIN, LED_ON);
  delay(500);
  digitalWrite(LED_PIN, LED_OFF);

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
}

void loop() {
  ArduinoOTA.handle();
  checkProp();
}