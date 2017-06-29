#include <Arduino.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>

void scanI2CAddressAndRegisters(WebSocketsServer &WEBSOCKETOBJECT);
void scanI2CAddress(WebSocketsServer &WEBSOCKETOBJECT);
void scanI2CRegisters(WebSocketsServer &WEBSOCKETOBJECT, byte ADDRESS);
boolean SinglescanI2CAddress(WebSocketsServer &WEBSOCKETOBJECT,byte address);
