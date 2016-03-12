/*
 * DHT22-basierter Temepratur- und Feuchtigkeitssensor
 * IoT via ESP8255 über Arduino https://github.com/esp8266/Arduino/
 */


#include "Humure.h"

ADC_MODE(ADC_VCC);

#define DHT_PIN 2
#define DHT_TYPE DHT22
#define LED_PIN 0

#include "wifi_password.h"

void setup() {}
void loop() {
  Humure sensorNode = Humure(LED_PIN, DHT_PIN, DHT_TYPE);

  while(true) {
  sensorNode.loop(); 
  }
}

//#define DHTPIN 2
//#define DHTTYPE DHT22
//DHT dht( DHTPIN, DHTTYPE );
//float h, t;
//unsigned long v;
//
//ADC_MODE(ADC_VCC);
//
//#define LED 0
//byte pin_status = 1; // low 
//
//const char* ssid     = "Humure";
//const char* password = "Railsgirls";
//
//ESP8266WebServer server ( 80 );
//
//void setup(void)
//{  
//  pinMode(LED, OUTPUT);
//  digitalWrite(LED, LOW);
//  
//  Serial.begin(74880);
//  
//  // Connect to WiFi network
//  WiFi.begin(ssid, password);
//  Serial.println("");  
//  
//  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  // Set up mDNS responder:
//  // - first argument is the domain name, in this example
//  //   the fully-qualified domain name is "esp8266.local"
//  // - second argument is the IP address to advertise
//  //   we send our IP address on the WiFi network
//  if (!MDNS.begin("esp8266")) {
//    Serial.println("Error setting up MDNS responder!");
//    while(1) { 
//      delay(100);
//    }
//  }
//  Serial.println("mDNS responder started");
//  
//  // Add service to MDNS-SD
//  MDNS.addService("http", "tcp", 80);
//
//  // Werbserver Handling
//  server.on ( "/", handleRoot );
//  server.on ( "/inline", []() {
//    server.send ( 200, "text/plain", "this works as well" );
//  } );
//  server.onNotFound ( handleNotFound );
//  server.begin();
//  Serial.println ( "HTTP server started" );
//  
//  // Starting DHT
//  dht.begin();
//
//  // signaling boot successfull
//  digitalWrite(LED, HIGH);
//}
//
//void readSensor()
//{
//  h = dht.readHumidity();
//  t = dht.readTemperature();
//  
//  unsigned long vtmp = 0;
//  #define VCC_READINGS 8
//  for ( uint i=0; i < VCC_READINGS; i++)
//    vtmp += ESP.getVcc();
//  v = vtmp / VCC_READINGS;
//
//  if ( isnan(h) || isnan(t) ) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  } else {
//    Serial.print("temperature:");
//    Serial.print(t);
//    Serial.print(" humidity:");
//    Serial.print(h);
//    Serial.print(" voltage:");
//    Serial.print(v);
//    Serial.println();
//  }
//}
//
//void headerJSON(WiFiClient client, String value = "")
//{
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: application/json");
//  client.println("Connection: close");
//  client.println();
//
//  if ( value.length() > 0 )
//  {
//    client.print("{\"value\": ");
//    client.print(value);
//    client.println("}");
//  } 
//}
//
//void sendStatusJSON(WiFiClient client)
//{
//
//  client.print("{");
//  client.print("\"temperature\": ");
//  client.print((float)t,2);
//  client.print(",\"humidity\": ");
//  client.print((float)h,2);
//  client.print(",\"voltage\": ");
//  client.print(v);
//  client.println("}");
//}
//
//void sendLED(WiFiClient client)
//{
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: application/json");
//  client.println("Connection: close");  // the connection will be closed after completion of the response
//  client.println();
//  client.print("{ \"led\"=");
//  // pin pulls down for led to turn on
//  client.print( ! pin_status);
//  client.println(" }");
//}
//
//void sendFullPage(WiFiClient client)
//{
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: text/html");
//  client.println("Connection: close");  // the connection will be closed after completion of the response
//  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
//  client.println();
//  client.println("<!DOCTYPE html>");
//  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
//  client.println("<head>\n<meta charset='UTF-8'>");
//  client.println("<title>ESP8266 Temperature & Humidity DHT11 Sensor</title>");
//  client.println("</head>\n<body>");
//  client.println("<H2>ESP8266 & DHT11 Sensor</H2>");
//  client.println("<H3>Humidity / Temperature</H3>");
//  client.println("<pre>");
//  client.print("Humidity (%)         : ");
//  client.println((float)h, 2);
//  client.print("Temperature (°C)  : ");
//  client.println((float)t, 2);
//  client.println("</pre>");
//  client.print("</body>\n</html>");
//}
//
//void loop(void)
//{
//  // Check if a client has connected
//  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }
//  Serial.println("");
//  Serial.println("New client");
//
//  // Wait for data from client to become available
//  while(client.connected() && !client.available()){
//    delay(1);
//  }
//  
//  // Read the first line of HTTP request
//  String req = client.readStringUntil('\r');
//  
//  // First line of HTTP request looks like "GET /path HTTP/1.1"
//  // Retrieve the "/path" part by finding the spaces
//  int addr_start = req.indexOf(' ');
//  int addr_end = req.indexOf(' ', addr_start + 1);
//  if (addr_start == -1 || addr_end == -1) {
//    Serial.print("Invalid request: ");
//    Serial.println(req);
//    return;
//  }
//
//  bool req_get;
//  if ( req.startsWith("GET") ) {
//    req_get = true;
//  } else if ( req.startsWith("POST") ) {
//    req_get = false;
//  } else {
//    Serial.print("Invalid request: ");
//    Serial.println(req);
//    return;
//  }
//  req = req.substring(addr_start + 1, addr_end);
//  Serial.print("Request: ");
//  Serial.println(req);
//  client.flush();
//
//  if ( req_get ) {
//    if ( req == "/" ) {
//        readSensor();
//        sendFullPage(client);
//    } else 
//    if ( req == "/status.json" ) {
//      readSensor();
//      sendStatusJSON(client);
//    } else
//    if ( req ==  "/led") {
//      sendLED(client);
//    } else {
//      client.print("HTTP/1.1 404 Not Found\r\n\r\n");
//    }
//  } else {
//    if ( req == "/led/on" ) {
//        pin_status = 0;
//        sendLED(client);
//    } else
//    if ( req =="/led/off" ) {
//        pin_status = 1;
//        sendLED(client);
//    } else {
//        client.print("HTTP/1.1 404 Not Found\r\n\r\n");
//    }
//     digitalWrite(LED, pin_status);
//  }
//  Serial.println("Done with client");
//}

