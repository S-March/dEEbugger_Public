#include <Arduino.h>

//TIMER VARIABLE
void setMsTimer(int TIME);
int getMsTimer(void);

//DATA LOG FLAG
void setDataLog(bool LOG);
bool getDataLog(void);

//UART SCOPE DATA
void setUartScopeFlag(bool UARTSCOPEFLAG);
bool getUartScopeFlag(void);
void setUartScopeData(String UARTSCOPEDATA);
String getUartScopeData(void);
void clearUartScopeData(void);

//ADC SCOPE DATA
void addADCScopeData1(String ADCSCOPEDATA);
void addADCScopeData2(String ADCSCOPEDATA);
String getADCScopeData1(void);
String getADCScopeData2(void);
void clearADCScopeData1(void);
void clearADCScopeData2(void);

//SCOPE PARAMETERS
void setChannelMode1(String MODE);
void setChannelMode2(String MODE);
String getChanneMode1(void);
String getChanneMode2(void);
