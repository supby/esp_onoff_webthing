// Network
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "wifi.h"

void setupWiFi(String deviceName) {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName.c_str());
  WiFi.setAutoReconnect(true);

  bool blink = true;
  if (WiFi.SSID() == "") {
    WiFi.beginSmartConfig();

    while (!WiFi.smartConfigDone()) {
      delay(1000);
      digitalWrite(LED_BUILTIN, blink ? HIGH : LOW);
      blink = !blink;
    }
  }

  WiFi.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_BUILTIN, blink ? HIGH : LOW);
    blink = !blink;
  }

  WiFi.stopSmartConfig();

  digitalWrite(LED_BUILTIN, HIGH);
}