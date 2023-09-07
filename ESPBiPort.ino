#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include "arduinoSerial.h"




ArduinoSerial &UNOSerial = getUNOSerial();
const String endl("\n");
const String placeholder("Breakpoint!\n");

// Define config variables
bool sendDataEnabled = false;
int bufferPeriod = 10000;  // default 10 seconds delay

// Declare meta data variables for sensor metrics
int metricsCount = 0;
int measureDelay = 0;

// Define other constants and objects
WebSocketsClient wsClient;

// Data propogation state to the Web Server
enum SendState
{
  META, INFO, DATA
};




/* Initial Configuration Functions */

void connectNetwork()
{
  // Read Wi-Fi network credentials
  String ssid, pswd;
  UNOSerial >> ssid >> pswd;
  UNOSerial << placeholder;

  // Connect to WiFi Network
  WiFi.begin(ssid, pswd);
  do {
    UNOSerial << "Connecting to WiFi..." << endl;  // For debugging
    delay(2000);
  } while (WiFi.status() != WL_CONNECTED);

  // Confirmation message to serial
  UNOSerial << "Connected to WiFi (" << WiFi.localIP().toString() << ")" << endl;
  UNOSerial.confirm();
}

void connectWebServer()
{
  // Read WebSocket server pathway
  String host, port, url;
  UNOSerial >> host >> port >> url;
  UNOSerial << placeholder;

  // Initialize WebSocket connection
  wsClient.begin(host, port.toInt(), url);
  wsClient.onEvent(webSocketEventHandler);
  wsClient.setReconnectInterval(5000);

  // For Debugging
  UNOSerial << "Connecting to Web server (ws://" << host << port << url << ")" << endl;
  // UNOSerial.confirm();    // REMOVE WHEN REIMPLEMENTING WS
}

void propagateMetricMeta()
{
  // Read metric meta information
  String metricsCountStr, measureDelayStr;
  UNOSerial >> metricsCountStr >> measureDelayStr;

  // Set the global variables, then propagate the metrics count
  metricsCount = metricsCountStr.toInt();
  measureDelay = measureDelayStr.toInt();
  wsClient.sendTXT(metricsCountStr);
  UNOSerial.confirm();
}

void propagateMetricInfo()
{
  for (int i = 0; i < metricsCount; ++i)
  {
    // Read each metric name and units
    String name, unit;
    UNOSerial >> name >> unit;

    // Propagate the information
    wsClient.sendTXT(name);
    wsClient.sendTXT(unit);
  }
  UNOSerial.confirm();
}




/* Data Transmission Functions */

void updateMeasurements()
{
  UNOSerial.confirm();  // Request Arduino for updated measurements
  String data;

  for (int i = 0; i < metricsCount; ++i)
  {
    // String data(UNOSerial.readLine());
    UNOSerial >> data;
    wsClient.sendTXT(data);
  }
  wsClient.sendTXT("");
}






/* WS Event Functions */

void webSocketDisconnected()
{
  UNOSerial << "WebSocket disconnected..." << endl;
  sendDataEnabled = false;
}

void webSocketConnected()
{
  static bool firstConnection = true;
  UNOSerial << "WebSocket connected!" << endl;
  sendDataEnabled = true;
  if (firstConnection)
  {
    firstConnection = false;
    UNOSerial.confirm();    // the second confirmation for web server connection stage at Arduino
  }
}

void webSocketText(uint8_t *payload, size_t length)
{
  // Print response to debug Serial
  UNOSerial << "WS --> " << ((char*) payload) << endl;
  // Prevent sending data from ESP until all data received
  // Convention: empty data packet signifies end of data stream
  sendDataEnabled = (length == 0);
  if (sendDataEnabled) UNOSerial << endl;
}


void webSocketEventHandler(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {

    case WStype_DISCONNECTED:
      webSocketDisconnected();
      break;
    
    case WStype_CONNECTED:
      webSocketConnected();
      break;
    
    case WStype_TEXT:
      webSocketText(payload, length);
      break;
    
    default:
      // ERROR STATE - unexpected ws message type from server
      UNOSerial << "ERROR: Unexpected message type from web server..." << endl;
    
  }
}




/* Setup and Loop Functions */

void setup()
{
  UNOSerial.begin(115200);   // Communication with Arduino (and for debugging)
  connectNetwork();
  connectWebServer();
}


void loop()
{
  static SendState state = META;
  wsClient.loop();

  if (wsClient.isConnected())
  {
    switch (state)
    {
      case META:
        propagateMetricMeta();
        state = INFO;
        break;
      case INFO:
        propagateMetricInfo();
        state = DATA;
        break;
      case DATA:
        updateMeasurements();
        delay(bufferPeriod-measureDelay);
        break;
    }
  }
}
