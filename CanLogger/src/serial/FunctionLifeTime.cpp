#include <Arduino.h>
#include "serial/FunctionLifeTime.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

FunctionLifeTime::FunctionLifeTime()
{
    this->time = millis();
}

FunctionLifeTime::FunctionLifeTime(String const& functionName)
    :function {functionName}
{
    this->time = millis();
}

FunctionLifeTime::~FunctionLifeTime()
{
    uint32_t diff = millis() - this->time;
    String str = this->function;
    str += ": in ";
    str += diff;
    str += " ms";
    scom.printDebug(str);
}

