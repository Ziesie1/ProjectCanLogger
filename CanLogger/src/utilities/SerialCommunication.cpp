#include "utilities/SerialCommunication.hpp"

const String utilities::endz = "\n\r"; // Zeilenumbruch mit CR
SerialCommunication utilities::scom {}; // Dient zur geordneten seriellen Kommunikation im Projekt

SerialCommunication::SerialCommunication()
{ }

SerialCommunication::SerialCommunication(HardwareSerial& serial)
    :serial {&serial}
{ }

void SerialCommunication::workWith(HardwareSerial& serial)
{
    this->serial = &serial;
}

void SerialCommunication::printError(String const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->println("######## Fehler ########");
    this->serial->print("#-> ");
    for(unsigned int i = 0; i < message.length(); i++)
    {
        this->serial->print(message[i]);
        if(message[i] == '\n' && i + 1 < message.length())
        {
            this->serial->print("#-> ");
        }
    }
    this->serial->println('\n');
    
}

void SerialCommunication::printDebug(String const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    if(this->debugMode)
    {
        this->serial->print(SerialCommunication::DEBUG_CHAR);
        this->serial->print(' ');
        this->serial->println(message);
    }
}

void SerialCommunication::showDebugMessages(bool mode)
{
    this->debugMode = mode;
}