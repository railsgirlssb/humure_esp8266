/*
 * DHT22-basierter Temepratur- und Feuchtigkeitssensor
 * IoT via ESP8255 über Arduino https://github.com/esp8266/Arduino/
 */

#include "Humure.h"

ADC_MODE(ADC_VCC);

#define DHT_PIN 0
#define DHT_TYPE DHT22
#define LED_PIN 2

#include "wifi_password.h"

void setup() {}
void loop() {
  Humure sensorNode = Humure(LED_PIN, DHT_PIN, DHT_TYPE);

  while(true) {
  sensorNode.loop(); 
  }
}

