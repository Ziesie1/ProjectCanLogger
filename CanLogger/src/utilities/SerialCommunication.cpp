#include "utilities/SerialCommunication.hpp"

const String utilities::endz = "\r\n"; // LF & CR
SerialCommunication utilities::scom {}; // be of use for the projekt to manage the seriel communikation

SerialCommunication::SerialCommunication()
{ }

SerialCommunication::SerialCommunication(HardwareSerial& serial)
    :serial {&serial}
{ }

/*
    This method set the HardwareSerial port, which the objekt shuold work with.
*/
void SerialCommunication::workWith(HardwareSerial& serial)
{
    this->serial = &serial;
}

/*
    Print eror-messages, which are importent for user and developer.
    The Message will be highlighted.
*/
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
    this->serial->println();
}

/*
    Print debug-messages, which are intersting for the developer.
    The Message will be highlighted.
*/
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

/*
    mode=true : activate debug messages.
    mode=false: deactivate debug messages.
*/
void SerialCommunication::setDebugMode(bool mode)
{
    this->debugMode = mode;
}

/*
    return=true : debug messages will be shown.
    return=false: debug messages will be discarded.
*/
bool SerialCommunication::isDebugMode()
{
    return this->debugMode;
}