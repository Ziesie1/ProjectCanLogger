#ifndef XFILE_HPP
#define XFILE_HPP

#include <SdFat.h>
#include <string.h>

class XFile
{
    SdFat& sdCard;
    File orgFile;
    String filePath = "";
    String fileName = "";
    
public:
    XFile(SdFat& sdCard);
    // Datei wird mit Text beschrieben/ueberschrieben
    bool writeStr(String const& text);
    // Datei wird Text angehängt
    bool appendStr(String const& text);
    // Dateipfad angeben
    void setFilePath(String const& filePath);
    // Dateinamen setzen
    void setFileName(String const& fileName);
    // Prüft ob die Datei bereits existiert
    bool exists();
    // Gibt den Dateipfad zurück
    String const& getFilePath() const;
    // Gibt den Dateinamen zurück
    String const& getFileName() const;
    // Gibt den gesamten Dateipfad zurück
    String getTotalFilePath() const;
    
};

#endif