# ESPBiPort

### Jump Ahead
--> [Description](#description)  
--> [Installation](#installation)  
--> [Version](#version)  

---




## Description

The ESP Wi-Fi Module is the critical component of the "IoT Remote Sensors Project". *This is where the magic happens*.

While the [Arduino UNO](https://github.com/Sharuchakalaka7/ArduinoSensors) handles all measurement related tasks, the ESP is responsible for managing remote communication from the hardware side of the project. This software module relays the MQ sensor data from the Arduino to a remote web server. The ESP controls various parameters of the data flow such as the measurement period and conditional data relay (e.g., to detect gas percentage anomalies).

The plan for this module includes (i) accomodate data relay for an abstract set of metrics, and (ii) extending to other connectivity solutions such as an ethernet shield.

---




## Installation

#### Hardware Components:
1. Complete the [ArduinoSensors Installation section](https://github.com/Sharuchakalaka7/ArduinoSensors/README.md#installation) first
2. Ensure you can program your ESP module
   - This code was tested with the ESP-01-S module with ESP8266 chip, and a ESP-to-USB connector
   - Use a serial monitor software such as CoolTerm to configure the ESP module
   - Make sure the ESP module is on "UART" mode at runttime instead of "Programming" mode
   - Set ESP's default baud rate to 9600 using serial command: `AT+UART_DEF=9600,8,1,0,0` [^1]
   - Enable both "AP" and "STA" modes in ESP using serial command: `AT+CWMODE=3`
3. Connect ESP to Arduino board after flashing the software [^2]

[^1]: Do ***not*** use the `AT+IPR=9600` command for the ESP - this freezes up the ESP module and you have to reflash the firmware again
[^2]: (Will upload diagram for ESP-01-S later)

#### Software Components:
1. Download and open this repository
2. Switch ESP module to "Programming" mode
3. Upload/Flash the program onto the ESP module
4. Switch back to "UART" mode




---

## Version

> **Current Version: v0.0.0**

The current version is aligned with the [ArduinoSensors](https://github.com/Sharuchakalaka7/ArduinoSensors) repository v0.0.0, where only the DHT11 Temperature/Humidity sensor is supported through a hard-coded program.

All Versions:
* v0.0.0: Hard-code example with DHT11 sensor
