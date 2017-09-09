#ifndef COMMON_H
#define COMMON_H

#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>


const char *cc_uri = "https://github.com/moddevices/cc-arduino-lib/tree/master/examples/ESP8266_ControlChain_Complex";
const int numExpression = 1;
const int numButtons = 5;
const int numRotary = 5;
const int expressionPedalId = numButtons + numRotary;
const int numActuators = numExpression + numButtons + numRotary;

const char *ssid = "ControlChain";
const char *password = "ControlChain";
const bool setupAccessPoint = true;

const int oscIncomingPort = 8000;
const int oscOutgoingPort = 9000;
  
SoftwareSerial debug(14, 13, false, 256);

char * getActuatorName(unsigned int idx);

#endif
