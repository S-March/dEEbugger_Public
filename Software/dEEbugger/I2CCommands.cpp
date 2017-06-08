#include "I2CCommands.h"

void scanI2CAddressAndRegisters(WebSocketsServer &WEBSOCKETOBJECT)
{
  byte error, address;
  int nDevices;
  String newDevice = "";
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    if(address!=54){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
     
        if (error == 0)
        {
          newDevice = "SERIAL I2C DEVICE ";
          if (address<16)
          {
            newDevice += "0";
          }
          newDevice += String(address, HEX);
    	    WEBSOCKETOBJECT.broadcastTXT(newDevice);
          nDevices++;
    	    scanI2CRegisters(WEBSOCKETOBJECT, address);
        }
    }
  }
}

void scanI2CAddress(WebSocketsServer &WEBSOCKETOBJECT)
{
  byte error, address;
  int nDevices;
  String newDevice = "";
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {    
    if(address!=54){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
     
        if (error == 0)
        {
          newDevice = "SERIAL I2C DEVICE ";
          if (address<16)
          {
            newDevice += "0";
          }
          newDevice += String(address, HEX);
    	    WEBSOCKETOBJECT.broadcastTXT(newDevice);
          nDevices++;
        }
    }
  }
}

void scanI2CRegisters(WebSocketsServer &WEBSOCKETOBJECT, byte ADDRESS)
{
  byte internalError, internalRegister, readValue;
  String newRegisterRead = "";
  for(internalRegister = 1; internalRegister < 255; internalRegister++ )
  {
    if(ADDRESS!=54){
        Wire.beginTransmission(ADDRESS);
        Wire.write(internalRegister);
        internalError = Wire.endTransmission();
        Wire.requestFrom(ADDRESS, 1); 
        if (Wire.available() > 0)
        { 
          readValue = Wire.read();
          if(readValue>0)
          {
            newRegisterRead = "SERIAL I2C REGISTER ";
            if (internalRegister<16)
            {
              newRegisterRead += "0";
            }
            newRegisterRead += String(internalRegister,HEX);
            newRegisterRead += " ";
              if (readValue<16)
              {
                newRegisterRead += "0";
              }
              newRegisterRead += String(readValue,HEX);
    	  	  WEBSOCKETOBJECT.broadcastTXT(newRegisterRead);
          }
        }
    }
  }
}
