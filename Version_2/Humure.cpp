#include "Humure.h"

extern char ssid[], password[];

Humure::Humure(uint8_t led_pin, uint8_t dht_pin, uint8_t dht_type, int server_port) :
  server(server_port), dht(dht_pin, dht_type)
{
  this->setup();
}

void Humure::readSensor()
{
  this->h = dht.readHumidity();
  this->t = dht.readTemperature();
  
  unsigned long vtmp = 0;
  #define VCC_READINGS 8
  for ( uint i=0; i < VCC_READINGS; i++)
    vtmp += ESP.getVcc();
  this->v = vtmp / VCC_READINGS;

  if ( isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.print("temperature:");
    Serial.print(t);
    Serial.print(" humidity:");
    Serial.print(h);
    Serial.print(" voltage:");
    Serial.print(v);
    Serial.println();
  }
}

void Humure::setup()
{  
  pinMode(this->led, OUTPUT);
  digitalWrite(this->led, LOW);
  
  Serial.begin(74880);
  Serial.println("");
  Serial.println("Booting up Humure Sensor Node");
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("  - Trying to connect to WiFi: ");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }
  Serial.println(" OK");
  Serial.print("  - Connected to ");
  Serial.println(ssid);
  Serial.print("  - IP address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS responder
  if ( MDNS.begin("esp8266") ){
    Serial.println ( "  - MDNS responder started" );
  }
  
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  // Werbserver Handling
//  this.server.on ( "/", handleRoot );
//  this.server.on ( "/inline", []() {
//    this.server.send ( 200, "text/plain", "this works as well" );
//  } );
//  this.server.onNotFound ( handleNotFound );
  this->server.begin();
  Serial.println ("  - HTTP server started");
  
  // Starting DHT
  this->dht.begin();

  // signaling boot successfull
  digitalWrite(this->led, HIGH);

  Serial.println ("Ready to process requests!");
  Serial.println ("");
}

void Humure::jsonValue(WiFiClient client, String str_value, String str_name)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  if ( str_value.length() > 0 )
  {
    client.print("{\"");
    client.print(str_name);
    client.print("\": ");
    client.print(str_value);
    client.println("}");
  } 
}

void Humure::loop()
{
  this->server.handleClient();
}

