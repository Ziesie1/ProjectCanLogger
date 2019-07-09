#ifndef SERIAL_COMMUNKATION_HPP
#define SERIAL_COMMUNKATION_HPP

#include <Arduino.h>
#include <string.h>

class SerialCommunication 
{
    static constexpr char DEBUG_CHAR = '*';

    HardwareSerial* serial;
    bool debugMode = false; // false = keine Debugnachrichten werden ausgegeben
    
public:
    SerialCommunication();
    SerialCommunication(HardwareSerial& serial);
    
    void workWith(HardwareSerial& serial);
    void print(String const& message) const ;
    void println(String const& message) const;
    SerialCommunication const& operator<<(String const& message) const;
    SerialCommunication const& operator<<(char const& message) const;
    // Ausgabe einer Fehlermeldung
    void printError(String const& message) const;
    // Ausgabe von Debugnachrichten
    void printDebug(String const& message) const;
    void showDebugMessages(bool mode);

};


namespace utilities
{
    constexpr char endz = '\n';
    extern SerialCommunication scom;
}



#endif