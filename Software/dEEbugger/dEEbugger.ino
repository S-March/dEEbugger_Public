#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <Wire.h>
#include "miniDB.h"
#include "websiteHTML.h"
#include "WebsocketInterpreter.h"

#define APMODE_BOOT_PIN 4

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void handleRoot();
void handleNotFound();
void serialEvent();

boolean bootAPFlag = true;

String inputString = "";
int uartIntData = -1;
boolean stringComplete = false;
String webSocketData = "";

unsigned long oldTime = 0;
unsigned long oldTimeADC = 0;
unsigned long currentTime = 0;

const char *ssid = "dEEbugger";
const char *password = "DEBUGGIN4DAYZ";

MDNSResponder mdns;

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup()
{  
  Serial.begin(115200);
  if(digitalRead(APMODE_BOOT_PIN))
  {
    WiFi.disconnect();
    WiFi.softAP(ssid, password);
    Serial.println();
    Serial.println("Booting in AP mode");
    Serial.println("Go to 192.168.4.1 to access the dEEbugger");
    Serial.println("NOTE: OTA is NOT available in AP mode");
  }
  else
  {
    Serial.println();
    Serial.println("Booting in client mode");
    Serial.println("OTA is available");
    WiFiManager wifiManager;
    wifiManager.autoConnect(ssid,password);
    Serial.println();
    Serial.print("IP address: ");  
    if (!MDNS.begin("dEEbugger"))
    {
      Serial.println("Error setting up MDNS responder!");
      while(1)
      { 
        delay(1000);
      }
    }
    Serial.println("mDNS responder started");
  
    Serial.print("Connect to http://dEEbugger.local or http://");
    Serial.println(WiFi.localIP());
    
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81); 
  }

  ArduinoOTA.setHostname("dEEbugger");
  ArduinoOTA.onStart([]()
  {Serial.println("Start");});
  ArduinoOTA.onEnd([]()
  {Serial.println("\nEnd");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {Serial.printf("Progress: %u%%\r", (progress / (total / 100)));});
  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  Wire.begin(); 

  scopeInit();
  setMsTimer(40);
}

void loop()
{  
  currentTime = millis();
  serialEvent();
  ArduinoOTA.handle();
  webSocket.loop();
  server.handleClient();
  if((currentTime - oldTimeADC)>=5)
  {
    ADCHandler();
    oldTimeADC = currentTime;
  }
  if(webSocketData!="")
  {
    webSocketDataInterpreter(webSocket, webSocketData);
    webSocketData = "";
  }
  if((currentTime - oldTime)>=getMsTimer())
  {
    scopeHandler(webSocket);
    webSocketData = "";
    oldTime = currentTime;
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch(type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\r\n", num);
        break;
    case WStype_CONNECTED:
        {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        }
        break;
    case WStype_TEXT:
         webSocketData = String((const char *)payload);
        break;
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\r\n", num, length);
        hexdump(payload, length);

        // echo data back to browser
        webSocket.sendBIN(num, payload, length);
        break;
    default:
        Serial.printf("Invalid WStype [%d]\r\n", type);
        break;
    }
}

void handleRoot()
{
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      return;
    } else {
      inputString += inChar;
    }
  }
  if (stringComplete) {
    if(getUartScopeFlag())
    {
      uartIntData = inputString.toInt();
      if(uartIntData)
      {
        setUartScopeData(inputString);
        Serial.println(inputString);
      }
      else if(inputString == "0")
      {
        setUartScopeData(inputString);
        Serial.println(inputString);
      }
      else
      {
        String line = "SERIAL UART " + inputString;
        webSocket.broadcastTXT(line);
        Serial.println(line);
      }
    }
    else{
      String line = "SERIAL UART " + inputString;
      webSocket.broadcastTXT(line);
      Serial.println(line);
    }
    inputString = "";
    stringComplete = false;
  }
}
