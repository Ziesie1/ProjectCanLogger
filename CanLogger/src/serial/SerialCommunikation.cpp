#include "serial/SerialCommunikation.hpp"

SerialCommunikation utilities::scom {}; // Dient zur geordneten seriellen Kommunikation im Projekt

SerialCommunikation::SerialCommunikation()
{ }

SerialCommunikation::SerialCommunikation(HardwareSerial& serial)
    :serial {&serial}
{ }

void SerialCommunikation::workWith(HardwareSerial& serial)
{
    this->serial = &serial;
}

void SerialCommunikation::print(String const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->print(message);
}

void SerialCommunikation::println(String const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->println(message);
}

SerialCommunikation const& SerialCommunikation::operator<<(String const& message) const
{
    if(this->serial == nullptr)
    {
        return *this;
    }
    this->serial->print(message);
    return *this;
}

SerialCommunikation const& SerialCommunikation::operator<<(char const& message) const
{
    if(this->serial == nullptr)
    {
        return *this;
    }
    this->serial->print(message);
    return *this;
}

void SerialCommunikation::printError(String const& message) const
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

void SerialCommunikation::printDebug(String const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    if(this->debugMode)
    {
        this->serial->print(SerialCommunikation::DEBUG_CHAR);
        this->serial->print(' ');
        this->serial->println(message);
    }
}

void SerialCommunikation::showDebugMessages(bool mode)
{
    this->debugMode = mode;
}
