#ifndef XFILE_HPP
#define XFILE_HPP

#include <SdFat.h>
#include <string.h>

/*
    Ein Objekt der Klasse XFile verwaltet eine Datei welche auf der SD-Karte gespeichert wird.
    Die Klasse bietet verschiedene Funktionen zur Verwaltung der Datei.
*/
class XFile
{
public:
    static const String LINE_FEED;
    
protected:
    SdFat& sdCard;
    File orgFile;
    String filePath = "";
    String fileName = "";
    
public:
    XFile(SdFat& sdCard);
    // bool writeStr(String const& text);
    // bool writeStrLn(String const& text);
    bool open();
    bool close();
    bool appendStr(String const& text);
    bool appendStrLn(String const& text);
    bool clear();
    void setFilePath(String const& filePath);
    void setFileName(String const& fileName);
    bool exists();

    String const& getFilePath() const;
    String const& getFileName() const;
    String getTotalFilePath() const;
    
};

#endif