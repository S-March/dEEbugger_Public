#include "miniDB.h"

int msTimer;
bool dataLogFlag;
bool uartScopeFlag;
String uartScopeData;
String adcScopeData;
String lastAdcScopeData;
String channelMode1;
String channelMode2;

/////////////////
//TIMER VARIABLE
/////////////////
void setMsTimer(int TIME)
{
	msTimer = TIME;
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
void addADCScopeData(String ADCSCOPEDATA)
{
  adcScopeData += " ";
  adcScopeData += ADCSCOPEDATA;
}
String getADCScopeData(void)
{
  return adcScopeData;
}
void setLastADCScopeData(String LASTADCSCOPEDATA)
{
  lastAdcScopeData = LASTADCSCOPEDATA;
}
String getLastADCScopeData(void)
{
  return lastAdcScopeData;
}
void clearADCScopeData(void)
{
  adcScopeData = "";
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
