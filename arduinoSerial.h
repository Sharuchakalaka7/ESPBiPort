#ifndef _ESPBIPORT_ARDUINOSERIAL_H
#define _ESPBIPORT_ARDUINOSERIAL_H

#include <HardwareSerial.h>
#include <List.hpp>

class ArduinoSerial
{
 public:
  ArduinoSerial();
  void begin(const long);

  /* ArduinoSerial decorated interface */
  String readLine(bool printback = false);
  void print(const String);
  void println(const String);
  void confirm();
 private:
  static const long SERIAL_DELAY = 500;
};

ArduinoSerial &operator<<(ArduinoSerial&, const String);
ArduinoSerial &operator<<(ArduinoSerial&, const double);

ArduinoSerial &operator>>(ArduinoSerial&, String&);
ArduinoSerial &operator>>(ArduinoSerial&, int&);
ArduinoSerial &operator>>(ArduinoSerial&, double&);

ArduinoSerial &getUNOSerial();

#endif // _ESPBIPORT_ARDUINOSERIAL_H
