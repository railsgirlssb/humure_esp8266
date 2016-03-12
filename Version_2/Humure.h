#ifndef HUMURE_H
#define HUMURE_H

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

class Humure
{
  public:
    Humure(uint8_t led_pin, uint8_t dht_pin, uint8_t dht_type = DHT22, int server_port = 80);

    void loop();
  
  private:
    void readSensor();
    void setup();
    void jsonValue(WiFiClient client, String str_value = "", String str_name = "value");

    // DHT sensor (temperature, humidity)
    DHT dht;
    float h, t;

    // voltage
    unsigned long v;

    // webserver
    ESP8266WebServer server;

    // status led
    uint8_t led;
    uint8_t led_status;
};

#endif
