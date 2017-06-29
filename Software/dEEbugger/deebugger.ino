   #include <Arduino.h>
/// NodeMCU numbering
/*  / these are defined in arduino 
  static const uint8_t D0   = 16;  and Red Led on NodeMcu V2 (not present on NodeMCU v3)
  static const uint8_t D1   = 5;
  static const uint8_t D2   = 4;
  static const uint8_t D3   = 0;
  static const uint8_t D4   = 2;  and Blue Led on SP8266
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 15;
  static const uint8_t D9   = 3;
  static const uint8_t D10  = 1;
  #define BlueLed 2 // NB  same as PIN D4!
*/


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
//#include "miniDB.h" // called from scope commands and websocket interprete
#include "websiteHTML.h"
#include "WebsocketInterpreter.h" 


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

//DAG added
String SettingsData;
boolean PHASE;
// DAG end

const char *ssid = "dEEbugger";
const char *password = "debuggin4dayz";

MDNSResponder mdns;

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
#define APMODE_BOOT_PIN D3  //DAG  press this pin to ground to start in AP mode.. 

void BROADCAST(String MSG); 

void setup()

{   
  Serial.begin(115200);
  pinMode(D_Input ,INPUT_PULLUP);
  pinMode(D4, OUTPUT);
  digitalWrite(D4,0); //DAG Turn on the blue LED 
  delay(1000);

  if(!digitalRead(APMODE_BOOT_PIN))
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
  digitalWrite(D4,1); //DAG led OFF?
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
  setMsTimer(500);  // initial 2Hz lazyflash timer for scope sampling rate timebase 
  SetScalesConnected(0);
  currentTime = millis();
  SettingsData="";
   Serial.println("testing for Connected I2C devices");
    //  scanI2CAddress(webSocket);
     Serial.println(SinglescanI2CAddress(webSocket,60));
      Serial.println(SinglescanI2CAddress(webSocket,50));
  ScalesInit(D5,D6);    
}

void BROADCAST(String MSG){
   webSocket.broadcastTXT(MSG); 
}

void loop()
{   
  //DAG test  Serial.println(hx711.read()/100.0);
  // test ends

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
  if((currentTime - oldTime)>=getMsTimer()) //getmstimer is the calibration ?...
  {
            digitalWrite(D4,PHASE); //DAG LED flashing 
            PHASE=!PHASE;
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
