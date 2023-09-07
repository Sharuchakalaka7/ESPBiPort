#include "arduinoSerial.h"


ArduinoSerial::ArduinoSerial() {}


void ArduinoSerial::begin(const long baudrate)
{
  Serial.begin(baudrate);
}


String ArduinoSerial::readLine(bool printback)
{
  delay(SERIAL_DELAY);
  while (!Serial.available());
  String str(Serial.readStringUntil('\n'));
  str.trim();
  if (printback)
  {
    delay(SERIAL_DELAY);
    println(str);
  }
  return str;
}


void ArduinoSerial::print(const String str)
{
  delay(SERIAL_DELAY);
  Serial.print(str);
}


void ArduinoSerial::println(const String str)
{
  print(str);
  Serial.print("\n");
}


void ArduinoSerial::confirm()
{
  // Convention: Empty string message means confirmation
  String response("");
  println(response);
  do {
    response = readLine(false);
  } while (response.length() > 0);
}


ArduinoSerial &operator<<(ArduinoSerial &serial, const String str)
{
  serial.print(str);
  return serial;
}


ArduinoSerial &operator<<(ArduinoSerial &serial, const double num)
{
  serial.print(String(num));
  return serial;
}


ArduinoSerial &operator>>(ArduinoSerial &serial, String &str)
{
  str = serial.readLine();
  return serial;
}


ArduinoSerial &operator>>(ArduinoSerial &serial, int &num)
{
  num = serial.readLine().toInt();
  return serial;
}


ArduinoSerial &operator>>(ArduinoSerial &serial, double &num)
{
  num = serial.readLine().toDouble();
  return serial;
}


ArduinoSerial &getUNOSerial()
{
  static ArduinoSerial serial;
  return serial;
}
