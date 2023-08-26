#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include "arduinoSerial.h"




ArduinoSerial UNOSerial;

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
  const String ssid(UNOSerial.readLine());
  const String pswd(UNOSerial.readLine());

  /*
  // Connect to WiFi Network
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2000);
    UNOSerial.println("Connecting to WiFi...");  // For debugging
  }

  // Confirmation message to serial
  Serial.print("Connected to WiFi (");
  Serial.print(ssid);
  UNOSerial.println(")");
  */
  UNOSerial.confirm();
}

void connectWebServer()
{
  // Read WebSocket server pathway
  const String serverHost(UNOSerial.readLine());
  const String serverPort(UNOSerial.readLine());
  const String serverURL(UNOSerial.readLine());

  /*
  // Initialize WebSocket connection
  wsClient.begin(serverHost, serverPort.toInt(), serverURL);
  wsClient.onEvent(webSocketEventHandler);
  wsClient.setReconnectInterval(5000);

  // For Debugging
  Serial.print("Connecting to Webserver (");
  Serial.print(serverHost + ':');
  Serial.print(serverPort);
  Serial.print(serverURL);
  UNOSerial.println(")");
  */
  UNOSerial.confirm();    // REMOVE WHEN REIMPLEMENTING WS
}

void propagateMetricMeta()
{
  // Read metric meta information
  String metricsCountStr(UNOSerial.readLine());
  String measureDelayStr(UNOSerial.readLine());

  // Set the global variables, then propagate the metrics count
  metricsCount = metricsCountStr.toInt();
  measureDelay = measureDelayStr.toInt();
    // wsClient.sendTXT(metricsCountStr);
  UNOSerial.confirm();
}

void propagateMetricInfo()
{
  for (int i = 0; i < metricsCount; ++i)
  {
    // Read each metric name and units
    String metricName(UNOSerial.readLine());
    String metricUnit(UNOSerial.readLine());

    // Propagate the information
      // wsClient.sendTXT(metricName);
      // wsClient.sendTXT(metricUnit);
  }
  UNOSerial.confirm();
}




/* Data Transmission Functions */

void updateMeasurements()
{
  UNOSerial.confirm();  // Request Arduino for updated measurements

  for (int i = 0; i < metricsCount; ++i)
  {
    String data(UNOSerial.readLine());
      // wsClient.sendTXT(data);
  }
    // wsClient.sendTXT("");
}






/* WS Event Functions */

void webSocketDisconnected()
{
  UNOSerial.println("WebSocket disconnected...");
  sendDataEnabled = false;
}

void webSocketConnected()
{
  static bool firstConnection = true;
  UNOSerial.println("WebSocket connected!");
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
  UNOSerial.println("WS --> ");
  UNOSerial.println((char*) payload);
  // Prevent sending data from ESP until all data received
  // Convention: empty data packet signifies end of data stream
  sendDataEnabled = (length == 0);
  if (sendDataEnabled) UNOSerial.println("");
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
      UNOSerial.println("ERROR: Unexpected message type from web server...");
    
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
  // wsClient.loop();

  // if (wsClient.isConnected())
  // {
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
  // }
}
