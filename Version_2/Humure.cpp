#include "Humure.h"

extern const char* ssid;
extern const char* password;

Humure::Humure(uint8_t led_pin, uint8_t dht_pin, uint8_t dht_type, int server_port) :
  server(server_port), dht(dht_pin, dht_type), led(led_pin)
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
  Serial.println("--------------------------------------------------------------------------------");
  Serial.println("Booting up Humure Sensor Node");
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("  - Trying to connect to WiFi '");
  Serial.print(ssid);
  Serial.print("':\n  ");
  
  // Wait for connection
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (counter == 60 )
      Serial.print(" failed\n  ");
    delay(100);
    Serial.print(".");
    counter += 1;
  }
  Serial.println(" connected!");
  Serial.print("  - IP address  : ");
  Serial.println(WiFi.localIP());

  // MAC auslesen
  uint8_t A[6];
  
  WiFi.macAddress(A);
  Serial.printf("  - MAC address : %02x:%02x:%02x:%02x:%02x:%02x\n", A[0], A[1], A[2], A[3], A[4], A[5]);

  // Set up mDNS responder
  if ( MDNS.begin("esp8266") ){
    Serial.println ( "  - MDNS responder started" );
  }
  
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  this->server.begin();
  Serial.println ("  - HTTP server started");
  
  // Starting DHT
  this->dht.begin();

  // signaling boot successfull
  digitalWrite(this->led, HIGH);

  Serial.println ("Ready to process requests!");
  Serial.println("--------------------------------------------------------------------------------");
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
  // Check if a client has connected
  WiFiClient client = this->server.available();
  if (!client) {
    return;
  }
  Serial.println("");
  Serial.println("New client");

  // Wait for data from client to become available
  while(client.connected() && !client.available()){
    delay(1);
  }
  
  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');

  // GET or POST?
  bool req_get;
  if ( req.startsWith("GET") ) {
    req_get = true;
  } else if ( req.startsWith("POST") ) {
    req_get = false;
  } else {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return;
  }
  
  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return;
  }
  req = req.substring(addr_start + 1, addr_end);

  Serial.print("Request ");
  if (req_get)
  {
    Serial.print("GET");
  } else {
    Serial.print("POST");
  }
  Serial.print(" : ");
  Serial.println(req);
  client.flush();

  if ( req_get ) {
    if ( req == "/" ) {
        readSensor();
        sendFullPage(client);
    } else 
    if ( req ==  "/lamp") {
      jsonValue(client, String( ! led_status) );
    } else 
    if ( req ==  "/temperatur") {
      readSensor();
      jsonValue(client, String( this->t) );
    } else 
    if ( req ==  "/humidity") {
      readSensor();
      jsonValue(client, String( this->h) );
    } else {
      client.print("HTTP/1.1 404 Not Found\r\n\r\n");
    }
  } else {
    if ( req == "/lamp/on" ) {
        led_status = 0;
        jsonValue(client, String( ! led_status) );
    } else
    if ( req =="/lamp/off" ) {
        led_status = 1;
        jsonValue(client, String( ! led_status) );
    } else {
        client.print("HTTP/1.1 404 Not Found\r\n\r\n");
    }
     digitalWrite(this->led, led_status);
  }

  Serial.println("Done with client");
}

void Humure::sendFullPage(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head>\n<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Temperature & Humidity DHT11 Sensor</title>");
  client.println("</head>\n<body>");
  client.println("<H2>ESP8266 & DHT11 Sensor</H2>");
  client.println("<H3>Humidity / Temperature</H3>");
  client.println("<pre>");
  client.print("Humidity (%)         : ");
  client.println((float)h, 2);
  client.print("Temperature (°C)  : ");
  client.println((float)t, 2);
  client.println("</pre>");
  client.print("</body>\n</html>");
}
