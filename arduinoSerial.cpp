#include <HardwareSerial.h>
#include "arduinoSerial.h"


ArduinoSerial::ArduinoSerial() {}


void ArduinoSerial::begin(const long baudrate)
{
  Serial.begin(baudrate);
}


String ArduinoSerial::readLine(bool printback)
{
  while (!Serial.available());
  String str(Serial.readStringUntil('\n'));
  str.trim();
  if (printback) println(str);
  return str;
}


void ArduinoSerial::println(const String str)
{
  delay(100);
  Serial.println(str);
}


void ArduinoSerial::confirm()
{
  // Convention: Empty string message means confirmation
  println("");
  while (readLine(false).length() != 0)
    println("");
}
