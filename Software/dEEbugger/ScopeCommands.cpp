#include "ScopeCommands.h"

String channelModeOutput1;
String channelModeOutput2;
bool toggledChannelOffFlag1;
bool toggledChannelOffFlag2;

void scopeInit(void)
{
  setChannelMode1("OFF");
  setChannelMode1("OFF");
  toggledChannelOffFlag1 = false;
  toggledChannelOffFlag2 = false;
  channelModeOutput1 = "";
  channelModeOutput2 = "";
  setUartScopeData("0");
}
void scopeHandler(WebSocketsServer &WEBSOCKETOBJECT)
{
  //Channel 1
  if(getChanneMode1()=="ADC")
  {
    toggledChannelOffFlag1 = false;
    channelModeOutput1 = "SCOPE ADC DATACHANNEL1";
    channelModeOutput1 += getADCScopeData();    
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput1);
    if(getDataLog())
    {
      Serial.print("CHANNEL1, ");
      Serial.println(getADCScopeData());
    }
    clearADCScopeData();
  }
  if(getChanneMode1()=="UART")
  {
    toggledChannelOffFlag1 = false;
    channelModeOutput1 = "SCOPE UART DATACHANNEL1";
    channelModeOutput1 += getUartScopeData();
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput1);
    clearUartScopeData();
  }
  if(getChanneMode1()=="OFF")
  {
    if(!toggledChannelOffFlag1)
    {
      toggledChannelOffFlag1 = true;
      channelModeOutput1 = "SCOPE OFF DATACHANNEL1";
      channelModeOutput1 += " 0";
      WEBSOCKETOBJECT.broadcastTXT(channelModeOutput1);
    }
  }
  //Channel 2
  if(getChanneMode2()=="ADC")
  {
    toggledChannelOffFlag2 = false;
    channelModeOutput2 = "SCOPE ADC DATACHANNEL2";
    channelModeOutput2 += getADCScopeData();    
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput2);
    if(getDataLog())
    {
      Serial.print("CHANNEL2, ");
      Serial.println(getADCScopeData());
    }
    clearADCScopeData();
  }
  if(getChanneMode2()=="UART")
  {
    toggledChannelOffFlag2 = false;
    channelModeOutput2 = "SCOPE UART DATACHANNEL2";
    channelModeOutput2 += getUartScopeData();
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput2);
    clearUartScopeData();
  }
  if(getChanneMode2()=="OFF")
  {
    if(!toggledChannelOffFlag2)
    {
      toggledChannelOffFlag2 = true;
      channelModeOutput2 = "SCOPE OFF DATACHANNEL2";
      channelModeOutput2 += " 0";
      WEBSOCKETOBJECT.broadcastTXT(channelModeOutput2);
    }
  }
}

