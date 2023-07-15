#include "ESP8266WiFi.h"
#include <WebSocketsClient.h>




// Define WiFi connection constants
const char* ssid = "Ahmed-Home";
const char* password = "alhamdulillah";

// Define WebSocket server address
const char* serverIP = "192.168.86.43";
const uint16_t serverPort = 8080;
const char* serverURL = "/ws";

// Define config variables
bool doSendData = false;
unsigned long bufferPeriod = 5000;  // default 5 seconds delay

// Define sensor data variables
float temperature, humidity;

// Define other constants and objects
const long baudRate = 9600;
WebSocketsClient webSocketC;




void setup()
{
  // Initialize serial (to communicate with UNO)
  Serial.begin(baudRate);

  // Connect to WiFi Network
  WiFi.begin(ssid, password);

  // Wait for initial connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Confirmation message to serial
  Serial.print("Connected to WiFi (");
  Serial.print(ssid);
  Serial.println(")");

  // Connect to remote WebSocket server 
  webSocketC.begin(serverIP, serverPort, serverURL);
  webSocketC.onEvent(webSocketEvent);
}




void loop()
{
  // Run WebSocketClient loop (with callback function)
  webSocketC.loop();

  if (doSendData)
  {
    // Refresh data from Arduino
    // TEMPORARY CODE: WITH RANDOM/COMPLIANT INPUT
    Serial.println("GET");

    while (!Serial.available());
    String temperatureStr(Serial.readStringUntil('\n'));   // ((float) random(2000, 3500)) / 100.0;

    while (!Serial.available());
    String humidityStr(Serial.readStringUntil('\n'));      // ((float) random(5000, 8000)) / 100.0;

    // Notify debug serial of data to be sent
    Serial.println(temperatureStr);
    Serial.println(humidityStr);
    Serial.println("");

    // Send data if authorized and callback function status is alright
    webSocketC.sendTXT(temperatureStr);
    webSocketC.sendTXT(humidityStr);
    webSocketC.sendTXT("");

    delay(bufferPeriod);
  }
}




void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {

    case WStype_DISCONNECTED:
      Serial.println("WebSocket disconnected...");
      doSendData = false;
      break;
    
    case WStype_CONNECTED:
      Serial.println("WebSocket connected!");
      doSendData = true;
      break;
    
    case WStype_TEXT:
      // Print response to debug Serial
      Serial.print("Received message: ");
      Serial.println((char*) payload);
      // Prevent sending data from ESP until all data received
      // Convention: empty data packet signifies end of data stream
      doSendData = (length == 0);
      if (doSendData) Serial.println("");
      break;
    
    default:
      // ERROR STATE - unexpected ws message from server
      Serial.println("Default message...");
    
  }
}
