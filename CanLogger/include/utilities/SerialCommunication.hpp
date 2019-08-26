#ifndef SERIAL_COMMUNKATION_HPP
#define SERIAL_COMMUNKATION_HPP

#include <Arduino.h>
#include <string.h>

/*
    SerialCommunication dient zur geordneten seriellen Kommunikation mit dem PC.
    Es sieht verschiedene Methoden für unterschiedliche Nachrichtentypen vor.
        - normale Nachrichten 
        - debug Nachrichten
        - fehler Nachrichten
*/
class SerialCommunication 
{
    static constexpr char DEBUG_CHAR = '*';

    HardwareSerial* serial;
    bool debugMode = false;
    
public:
    SerialCommunication();
    SerialCommunication(HardwareSerial& serial);
    
    void workWith(HardwareSerial& serial);

    template<typename T>
    void print(T const& message) const ;
    template<typename T>
    void println(T const& message) const;
    template<typename T>
    SerialCommunication const& operator<<(T const& message) const;

    void printError(String const& message) const;
    void printDebug(String const& message) const;
    void setDebugMode(bool mode);
    bool isDebugMode();
};


namespace utilities
{
    extern const String endz;
    extern SerialCommunication scom;
}

#include "utilities/SerialCommunication.inl"


#endif