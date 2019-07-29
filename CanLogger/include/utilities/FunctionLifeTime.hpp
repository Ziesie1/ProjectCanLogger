#ifndef FUNCTION_LIFE_TIME_HPP
#define FUNCTION_LIFE_TIME_HPP

#include <Arduino.h>
#include <string.h>

class FunctionLifeTime
{
    String function;
    uint32_t time;

public:
    FunctionLifeTime(String const& functionName);
    ~FunctionLifeTime();
};

#define FUNCTION_TIME_X(x) FunctionLifeTime functionlifetime{x};

#endif