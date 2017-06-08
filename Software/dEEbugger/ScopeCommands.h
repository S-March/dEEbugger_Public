#include <Arduino.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include "I2CCommands.h"
#include "miniDB.h"

void scopeInit(void);
void scopeHandler(WebSocketsServer &WEBSOCKETOBJECT);
void ADCInit(void);
void setADCChannel(int CHANNEL);
int ADCRead(void);
void ADCHandler(void);
