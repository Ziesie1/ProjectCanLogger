#ifndef XFILE_HPP
#define XFILE_HPP

#include <SdFat.h>
#include <string.h>

class XFile : public SdFile
{

public:
    // Datei wird mit Text beschrieben/ueberschrieben
    bool writeStr(String const& text);
    // Datei wird Text angehängt
    bool appendStr(String const& text);
};

#endif