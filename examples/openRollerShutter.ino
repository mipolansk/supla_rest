
#include <SuplaRestLib.h>

#define WIFI_SSID "<set your WIFI SSID>"
#define WIFI_PWD "<set your WIFI password>"

#define SUPLA_USER "your supla user"
#define SUPLA_PWD "your supla password"

// replace with your roller id
#define SUPLA_ROLLER_ID 1234

bool opened = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  SuplaRest.begin(SUPLA_USER, SUPLA_PWD);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("WiFI connecting");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    delay(1000);
    Serial.println(".");
  }

  if (!opened) {
    Serial.println("Openning...");
    // Will fail at first time, because iterate() will load access token.
    if (SuplaRest.openRollerShutter(SUPLA_ROLLER_ID)) {
      opened = true;
    }
  }
  
  SuplaRest.iterate();
  delay(500);
}
