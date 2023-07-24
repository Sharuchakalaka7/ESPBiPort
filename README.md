# ESPBiPort

The ESP Wi-Fi Module is the critical component of the "IoT Remote Sensors Project". *This is where the magic happens*.

While the [Arduino UNO](https://github.com/Sharuchakalaka7/ArduinoSensors) handles all measurement related tasks, the ESP is responsible for managing remote communication from the hardware side of the project. This software module relays the MQ sensor data from the Arduino to a remote web server. The ESP controls various parameters of the data flow such as the measurement period and conditional data relay (e.g., to detect gas percentage anomalies).

---

> **Current Version: v0.0.0**

The current version is aligned with the [ArduinoSensors](https://github.com/Sharuchakalaka7/ArduinoSensors) repository v0.0.0, where only the DHT11 Temperature/Humidity sensor is supported through a hard-coded program.

All Versions:
* v0.0.0: Hard-code example with DHT11 sensor
