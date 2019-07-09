#ifndef SERIAL_COMMUNKATION_HPP
#define SERIAL_COMMUNKATION_HPP

#include <Arduino.h>
#include <string.h>

class SerialCommunikation 
{
    static constexpr char DEBUG_CHAR = '*';

    HardwareSerial* serial;
    bool debugMode = false; // false = keine Debugnachrichten werden ausgegeben
    
public:
    SerialCommunikation();
    SerialCommunikation(HardwareSerial& serial);
    
    void workWith(HardwareSerial& serial);
    void print(String const& message) const ;
    void println(String const& message) const;
    SerialCommunikation const& operator<<(String const& message) const;
    SerialCommunikation const& operator<<(char const& message) const;
    void printError(String const& message) const;
    void printDebug(String const& message) const;
    void showDebugMessages(bool mode);

};


namespace utilities
{
    constexpr char endz = '\n';
    extern SerialCommunikation scom;
}



#endif