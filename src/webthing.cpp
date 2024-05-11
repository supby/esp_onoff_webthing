#include "webthing.h"
#include "settings.h"
#include "logging.h"

WebThingAdapter* adapter;

const char* relayTypes[] = {"asyncProperty", nullptr};
ThingDevice relay("relay", DEVICE_NAME, relayTypes);
ThingProperty onOffProp("on", "OnOffProperty", BOOLEAN, "OnOffProperty", onOffChanged);

bool lastOn = false;

void setupWebThing(String deviceName) {
  onOffProp.readOnly = false;
  onOffProp.title = "OnOff";

  adapter = new WebThingAdapter(deviceName, WiFi.localIP(), 80, true);
  relay.addProperty(&onOffProp);
  adapter->addDevice(&relay);
  adapter->begin();

  INFO_PRINT("Thing URL: http://");
  INFO_PRINT(deviceName);
  INFO_PRINT(".local/things/");
  INFO_PRINTLN(relay.id);
  INFO_PRINTLN("");
}

void setRelay(bool newValue) {
  if (newValue != lastOn) {
    DEBUG_PRINTLN("Adapter will change relay state");
    
    digitalWrite(RELAY_PIN, newValue ? RELAY_ON : RELAY_OFF);
    digitalWrite(LED_PIN, newValue ? LED_ON : LED_OFF);

    DEBUG_PRINT_VAR("Relay state has changed: ", enabled);
  }
  lastOn = newValue;
}

void updateThing() {
  adapter->update();
}

void onOffChanged(ThingPropertyValue newValue) {
  setRelay(newValue.boolean);
}