#include "miniDB.h"

int msTimer;
bool dataLogFlag;
bool dataTAREFlag;
bool uartScopeFlag;
String uartScopeData;
String adcScopeData1;
String adcScopeData2;
String channelMode1;
String channelMode2;

/////////////////
//TIMER VARIABLE
/////////////////
void setMsTimer(int TIME)
{
  String  MSGdata ;
	msTimer = TIME; 
  Serial.print(" Setting msTimer to:");
  Serial.print(msTimer);
  Serial.println("ms");

   MSGdata = "SCOPE SETTINGS SPS ";  // 
   MSGdata += String ( (1000/getMsTimer()),DEC);    //Samples per second are 1000/ mstimer.
    BROADCAST(MSGdata);
    Serial.print("...updated msTimer , sending:");
    Serial.println(MSGdata);
}
int getMsTimer(void)
{
	return msTimer;
}

/////////////////
//DATA LOG FLAG
/////////////////
void setDataLog(bool LOG)
{
  dataLogFlag = LOG;
}
bool getDataLog(void)
{
  return dataLogFlag;
}
/////////////
//Scales Flags
//////////////
void setTARE(bool LOG)
{
  dataTAREFlag = LOG;
}
bool getTARE(void)
{
  return dataTAREFlag;
}

/////////////////
//UART SCOPE DATA
/////////////////
void setUartScopeFlag(bool UARTSCOPEFLAG)
{
  uartScopeFlag = UARTSCOPEFLAG;
}
bool getUartScopeFlag(void)
{
  return uartScopeFlag;
}
void setUartScopeData(String UARTSCOPEDATA)
{
  uartScopeData += " ";
  uartScopeData += UARTSCOPEDATA;
}
String getUartScopeData(void)
{
  return uartScopeData;
}
void clearUartScopeData(void)
{
  uartScopeData = "";
}

/////////////////
//ADC SCOPE DATA
/////////////////
void addADCScopeData1(String ADCSCOPEDATA)
{
  adcScopeData1 += " ";
  adcScopeData1 += ADCSCOPEDATA;
}
String getADCScopeData1(void)
{
  return adcScopeData1;
}
void addADCScopeData2(String ADCSCOPEDATA)
{
  adcScopeData2 += " ";
  adcScopeData2 += ADCSCOPEDATA;
}
String getADCScopeData2(void)
{
  return adcScopeData2;
}
void clearADCScopeData1(void)
{
  adcScopeData1 = "";
}
void clearADCScopeData2(void)
{
  adcScopeData2 = "";
}

/////////////////
//SCOPE PARAMETERS
/////////////////
void setChannelMode1(String MODE)
{
  channelMode1 = "";
  channelMode1 = MODE;
  if(channelMode1 == "UART")
  {
    setUartScopeFlag(true);    
  }
  else
  {
    setUartScopeFlag(false);
  }
}
void setChannelMode2(String MODE)
{
  channelMode2 = "";
  channelMode2 = MODE;
  if(channelMode2 == "UART")
  {
    setUartScopeFlag(true);    
  }
  else
  {
    setUartScopeFlag(false);
  }
}
String getChanneMode1(void)
{
  return channelMode1;
}
String getChanneMode2(void)
{
  return channelMode2;
}
