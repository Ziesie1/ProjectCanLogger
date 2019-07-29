#include "utilities/SerialCommunication.hpp"

template<typename T>
void SerialCommunication::print(T const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->print(message);
}

template<typename T>
void SerialCommunication::println(T const& message) const
{
    if(this->serial == nullptr)
    {
        return;
    }
    this->serial->println(message);
}

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