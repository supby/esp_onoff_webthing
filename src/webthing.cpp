#include "webthing.h"
#include "settings.h"
#include "logging.h"

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

void updateThingProperties() {
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