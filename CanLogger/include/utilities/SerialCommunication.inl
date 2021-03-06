#include "utilities/SerialCommunication.hpp"

/*
    Normal output message - without LF&CR
*/
template<typename T>
void SerialCommunication::print(T const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->print(message);
}

/*
    Normal output message - with LF&CR
*/
template<typename T>
void SerialCommunication::println(T const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->println(message);
}

/*
    Normal output message - without LF&CR
*/
template<typename T>
SerialCommunication const& SerialCommunication::operator<<(T const& message) const
{
    if(this->serial == nullptr)
    {
        return *this;
    }
    this->serial->print(message);
    return *this;
}