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
    // Ausgabe einer normalen Nachricht - ohne Zeilenumbruch
    template<typename T>
    void print(T const& message) const ;
    // Ausgabe einer normalen Nachricht - mit Zeilenumbruch
    template<typename T>
    void println(T const& message) const;
    // Ausgabe einer normalen Nachricht - ohne Zeilenumbruch
    template<typename T>
    SerialCommunication const& operator<<(T const& message) const;
    // Ausgabe einer Fehlermeldung
    void printError(String const& message) const;
    // Ausgabe von Debugnachrichten
    void printDebug(String const& message) const;
    // Aktivierung/Deaktivierung von Debugnachrichten
    void showDebugMessages(bool mode);
};


namespace utilities
{
    extern const String endz;
    extern SerialCommunication scom;
}

#include "utilities/SerialCommunication.inl"


#endif