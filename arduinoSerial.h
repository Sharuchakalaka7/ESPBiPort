#ifndef _ESPBIPORT_ARDUINOSERIAL_H
#define _ESPBIPORT_ARDUINOSERIAL_H

class ArduinoSerial
{
 public:
  ArduinoSerial();
  void begin(const long);

  /* ArduinoSerial decorated interface */
  String readLine(bool printback = true);
  void println(const String);
  void confirm();
};

#endif // _ESPBIPORT_ARDUINOSERIAL_H