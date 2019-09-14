#include "display/screenBuffer.hpp"
#include <Arduino.h>

/* 
    function to add zeros to a String that every String will have the same ammount of chars
    Input:  s         - reference to the String that should be modified
            val       - value that will be inserted into the String s after this function is called
            maxVal    - maximum possible value the type of val can reach
            numberSys - number System in which the value should be displayed (e.g. 10(dec), 16(hex))
                        possible values: 0 - 255, HEX, DEC, OCT, BIN
*/
void AddZerosToString(String& s,uint32_t const val,uint32_t const maxVal, uint8_t const numberSys)
{
    uint32_t copyVal = val;
    uint32_t copyMaxVal = maxVal;
    uint32_t stringLength = 0;
    uint32_t maxStringLength = 0;
    if(copyVal == 0)
    {
        stringLength = 1;
    }
    for(uint32_t i=0; i<8; i++)
    {
        if(copyVal > 0)
        {
            copyVal = (copyVal >> 4);
            stringLength++;
        }
        if(copyMaxVal > 0)
        {
            copyMaxVal = (copyMaxVal >> 4);
            maxStringLength++;
        }
    }
    for(uint32_t i=stringLength; i<maxStringLength; i++)
    {
        s+="0";
    }
}

/* 
    function to add zeros to a String that every String will have the same ammount of chars
    Input:  s           - reference to the String that should be modified
            maxSymbols  - maximum possible ammount the String can reach
*/
void AddBlanksToString(String& s, uint32_t const maxSymbols)
{
    for(uint32_t i=s.length(); i<maxSymbols; i++)
    {
        s += " ";
    }
}

