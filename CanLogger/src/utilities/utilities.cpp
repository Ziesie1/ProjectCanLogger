#include "display/screenBuffer.hpp"
#include "can/Canmsg.hpp"
#include <Arduino.h>

/* 
    function that processes the last message of the "Canmsg_bufferCanRecMessages"
*/
void processCanMessage(void)
{
  Canmsg_bufferCanRecPointer--;
  Canmsg curMsg = std::move(Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer]);
  
  //screenBuffer:
  sortCanMessageIntoBuffer(curMsg);

  //SD-Card:

  //loopback:
  /*
  Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer].Send();
  */

  //Serial:
  /*
  Serial.print("Empfangene Nachricht: ");
  Serial.println(static_cast<String>(Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer]));
  */
}

/* 
    function to add zeros to a String that every String will have the same ammount of chars
    Input:  s         - reference to the String that should be modified
            val       - value that will be inserted into the String s after this function is called
            maxVal    - maximum possible value the type of val can reach
            numberSys - number System in which the value should be displayed (e.g. 10(dec), 16(hex))
                        possible values: 0 - 255, HEX, DEC, OCT, BIN
*/
void AddZerosToString(String& s,uint64_t const val,uint64_t const maxVal, uint8_t const numberSys)
{
  for(uint64_t i=numberSys; i<=maxVal; i*=numberSys)
  {
    if(val < i)
    {
      s+="0";
    }
  }
}
