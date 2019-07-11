#include "sd/XFile.hpp"
#include "serial/FunctionLifeTime.hpp"

bool XFile::writeStr(String const& text)
{
    FUNCTION_TIME_X("bool XFile::writeStr(String const& text)") // Fehler
    for(size_t i = 0; i < text.length(); i++)
    {
        if(!SdFile::write(text[i])) // zeichen schreiben
        {
            return false; // Konnte zeichen nicht schreiben
        }
    }
    this->sync();
    return true;
}

bool XFile::appendStr(String const& text)
{
    // Bis zum Ende der Datei springen
    while (this->read() != -1) { }
    return this->writeStr(text);
}
