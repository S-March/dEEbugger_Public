#include "ScopeCommands.h"
#include <HX711.h>

HX711 scale;


boolean ScalesPresent;
String channelModeOutput1;
String channelModeOutput2;
bool toggledChannelOffFlag1;
bool toggledChannelOffFlag2;
byte ADCAddress = 54;  //adc address on 12c
long offset=64;
long CH1Scale =1 ; //DAG new variables for scaling everything
long CH2Scale =1 ;
float CH1scaleset=2.09;
float CH2scaleset=2.09;

long TAREA=0;
long TAREB=0;
boolean ScalesConnected(void){
    return ScalesPresent;
}
void ScalesInit(byte Data,byte Clock){
   long reading =0;
   pinMode(Data, INPUT_PULLUP);
  Serial.print("Testing for Scales..."); 
  scale.begin(Data, Clock); 
  scale.set_gain(128); // selects A
  reading= scale.read();
  //Serial.println(reading);
 if (reading !=-1) {
    Serial.println("  HX711 PRESENT ");
     SetScalesConnected(1);
    }  
    else {
      Serial.println(" No HX711 Present");
      SetScalesConnected(0);
    }
 
  if (ScalesConnected()) {
    TAREA=0;
    TAREB=0;
    Serial.println("----------------------------");
    Serial.println("Initializing the A scale");
    Serial.print("Initial A and TARE_A: ");
    TAREA =scale.read_average(20);
    Serial.println(TAREA);  
    Serial.println("----------------------------");
    scale.set_gain(32); // select ch b
    Serial.println("Initializing the B scale");
    Serial.print("Initial B and TARE_B ");
    TAREB =scale.read_average(20);;      // print a raw reading of chA with tare = 0
    Serial.println(TAREB);  
     }
}
void SetScalesConnected(boolean set){
  ScalesPresent=set;
}
String readScales(byte Chan){ 
  String Reading;
  int average =1;
  Reading="0";
  if (ScalesConnected()) {
         if (Chan==0){ //CH A
              scale.set_gain(128);
              Reading=scale.read(); //dummy to get ready
              //CH1scaleset);  this value is obtained by calibrating the scale with known weights; see the README for details
              Reading= String (  ((scale.read_average(average)-TAREA)/CH1scaleset) );}
          if (Chan==1){  // CH B
              scale.set_gain(32);
              Reading=scale.read(); //dummy to get ready
              //CH2scaleset);  This value is obtained by calibrating the scale with known weights; see the README for details
              Reading= String (  ((scale.read_average(average)-TAREB)/CH2scaleset));}
                          }  
  return Reading;
}
void scopeInit(void)
{
  
  setChannelMode1(""); // nb this does not set the initial settings, use "SELECTED" IN THE WEBSITE HTML CODES 
  setChannelMode2("");
  toggledChannelOffFlag1 = false;
  toggledChannelOffFlag2 = false;
  channelModeOutput1 = "";
  channelModeOutput2 = "";
  // changed and webserver to defauult to var yPlotScaleFactor = 10;
 
  setUartScopeData("0");
  ADCInit();
  
}
void scopeHandler(WebSocketsServer &WEBSOCKETOBJECT,byte Chan)
{
  //Channel 1
if (Chan==0){ //CH 1
  if( (getChanneMode1()=="4V ADC")||(getChanneMode1()=="64V ADC")||(getChanneMode1()=="INT ADC")||(getChanneMode1()=="DIG")|| (getChanneMode1()=="SCALES") )
  {
    toggledChannelOffFlag1 = false;
    channelModeOutput1 = "SCOPE ADC DATACHANNEL1 ";  // DAG ADDED SPACE TO WORK WITH SCALE FACTOR string IN NEXT LINE
    if( getChanneMode1()=="SCALES"){
      channelModeOutput1 += String ( (((getADCScopeData1().toInt()*4096/64)/CH1Scale)+offset),DEC);    //DAG  to scale, plus offset to lift on screen (4096 =64v)/CH1Scale) order is to ensure INT stays big for longer in the sum to avoid truncation..
    }
    else {channelModeOutput1 += String ( (((getADCScopeData1().toInt()*4096/64)/CH1Scale)),DEC);    //DAG  to scale,(4096 =64v)/CH1Scale) order is to ensure INT stays big for longer in the sum to avoid truncation..
    }
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput1);
    if(getDataLog())
    {
      if (getChanneMode1()!="SCALES"){      Serial.print("CHANNEL1 mV, ");      }
      else{        Serial.print("CHANNEL1 grams, ");        }
        Serial.println((getADCScopeData1().toInt()*1000/CH1Scale)   );    }
        
        
       // clearADCScopeData1();  // DAG just for a test
      }
  if(getChanneMode1()=="UART")     {
    toggledChannelOffFlag1 = false;
    channelModeOutput1 = "SCOPE UART DATACHANNEL1";
    channelModeOutput1 += getUartScopeData();
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput1);
    clearUartScopeData();  }
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
 }
  //Channel 2
  if (Chan==1){
  if( (getChanneMode2()=="4V ADC")||(getChanneMode2()=="64V ADC")||(getChanneMode2()=="INT ADC")||(getChanneMode2()=="DIG") ||(getChanneMode2()=="SCALESB")||(getChanneMode2()=="SCALES")) 
  {
    toggledChannelOffFlag2 = false;
    channelModeOutput2 = "SCOPE ADC DATACHANNEL2 ";
    if(( getChanneMode2()=="SCALES")||(getChanneMode2()=="SCALESB")) {
      channelModeOutput2 += String ( (((getADCScopeData2().toInt()*4096/64)/CH2Scale)+offset),DEC);    //DAG  to scale, plus offset to lift on screen (4096 =64v)/CH1Scale) order is to ensure INT stays big for longer in the sum to avoid truncation..
    }
    else {channelModeOutput2 += String ( (((getADCScopeData2().toInt()*4096/64)/CH2Scale)),DEC);    //DAG  to scale,(4096 =64v)/CH1Scale) order is to ensure INT stays big for longer in the sum to avoid truncation..
    }
        
    WEBSOCKETOBJECT.broadcastTXT(channelModeOutput2);
    if(getDataLog())
    {
      if ((getChanneMode2()=="SCALESB")||(getChanneMode2()=="SCALES")){
      Serial.print("     CHANNEL2 grams, ");
      }
      else{
         Serial.print("CHANNEL2 mV, "); 
        }
        Serial.println((getADCScopeData2().toInt()*1000/CH2Scale)   );
    }
      
   
   // clearADCScopeData2();  // DAG just for a test
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
  }
  
void ADCInit(void)
{
  	byte internalError;
    byte ADCSetupByte = 210;
	  byte ADCConfigByte = 97;
    Wire.beginTransmission(ADCAddress);
    Wire.write(ADCSetupByte);
    Wire.write(ADCConfigByte);
    internalError = Wire.endTransmission();
	if (internalError == 0)
	{
		Serial.println("i2c ADC Initialized");
	}
}
void setADCChannel(int CHANNEL)
{   
    byte internalError, ADCConfigByte;
    //Select correct channel
    switch(CHANNEL)
    {
        case 0:
            ADCConfigByte = 97;
            break;
        case 1:
            ADCConfigByte = 99;
            break;
        default:
            ADCConfigByte = 97;
            break;
    }
    //Send channel selection
    Wire.beginTransmission(ADCAddress);
    Wire.write(ADCConfigByte);
    internalError = Wire.endTransmission();
    if (internalError != 0)
    {
        Serial.println("Error setting ADC channel");
    }
}
int ADCRead(void)
{
	//Read channel
	Wire.requestFrom(ADCAddress, 2); 
    if (Wire.available() > 0)
    { 
        byte ADCResultMSB = Wire.read();
        byte ADCResultLSB = Wire.read();
        uint16_t ADCResult = (((ADCResultMSB<<8)|ADCResultLSB)&0x0FFF);
        return ADCResult;
	}
}
void ADCHandler(boolean Chan)
{

byte DigPin = D_Input;  
if (Chan==0) {
  clearADCScopeData1();
  if(getChanneMode1()=="DIG")
    {
       if  (digitalRead(DigPin)==1) {  
            addADCScopeData1("1") ;}
       else {
        addADCScopeData1("0");
               }
        CH1Scale=1;
    }
  if(getChanneMode1()=="INT ADC")
    {
        addADCScopeData1(String((analogRead(0)) ));  //3.3v ref, output in mv1024 not 2048
        CH1Scale=1024/3.3;
    }
  if(getChanneMode1()=="SCALES")
    {
         addADCScopeData1(readScales(0) ) ;   //readScales() is a string so no need for the String function and includes a check for presence
         CH1Scale=-100000; //DAG NB set in initscales to grammes, 5KG EXPECTED TO READ AS 5 V! 209 and 1000 2090 and 100 20900 and 10 here
     }
    if(getChanneMode1()=="SCALESB")
    {
         addADCScopeData1(readScales(1) ) ;   //readScales() is a string so no need for the String function and includes a check for presence
         CH1Scale=-25000; //DAG NB set in initscales to grammes, 5KG EXPECTED TO READ AS 5 V!
     }
  if(getChanneMode1()=="4V ADC")
    {
        setADCChannel(0);
        addADCScopeData1(String(ADCRead()));
        CH1Scale=2048/4;
    }
  if(getChanneMode1()=="64V ADC")
    {
        setADCChannel(1);
        addADCScopeData1(String(ADCRead()));
        CH1Scale=2048/64;
    }    
}
else{
  clearADCScopeData2();
  if(getChanneMode2()=="DIG")
    {
     // Serial.println(digitalRead(13));  //test
       if  (digitalRead(DigPin)==1){  
        addADCScopeData2("1") ;
        }
       else {
        addADCScopeData2("0");
               }
        CH2Scale=1;
     }
     if(getChanneMode2()=="INT ADC")
    {
        addADCScopeData2(String((analogRead(0)) ));  //3.3v ref, output in mv1024 not 2048
        CH2Scale=1024/3.3;
    }
    if(getChanneMode2()=="SCALES")
    {
         addADCScopeData2(readScales(0) ) ;   //readScales() is a string so no need for the String function and includes a check for presence
         CH2Scale=-100000; //DAG NB set in initscales to grammes, 5KG EXPECTED TO READ AS 5 V!
     }
    if(getChanneMode2()=="SCALESB")
    {
         addADCScopeData2(readScales(1) ) ;   //readScales() is a string so no need for the String function and includes a check for presence
         CH2Scale=-25000; //DAG NB set in initscales to grammes, 5KG EXPECTED TO READ AS 5 V!
     }
    if(getChanneMode2()=="4V ADC")
    {
        setADCChannel(0);
        addADCScopeData2(String(ADCRead()));
        CH2Scale=2048/4;
    }
    if(getChanneMode2()=="64V ADC")
    {
        setADCChannel(1);
        addADCScopeData2(String(ADCRead()));
        CH2Scale=2048/64;
    }
    }
}

