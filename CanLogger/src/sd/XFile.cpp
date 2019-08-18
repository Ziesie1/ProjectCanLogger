#include "sd/XFile.hpp"
#include "utilities/SerialCommunication.hpp"
#include "utilities/FunctionLifeTime.hpp"

using namespace utilities;

const String XFile::LINE_FEED = "\r\n";

XFile::XFile(SdFat& sdCard)
    :sdCard{sdCard}
{ }

// override file with string
bool XFile::writeStr(String const& text)
{
    //FUNCTION_TIME_X("bool XFile::writeStr(String const& text)")
    bool status = true;
    this->orgFile = this->sdCard.open(this->getTotalFilePath().c_str(), O_RDWR | O_CREAT | O_TRUNC);
    if(this->orgFile)
    {
        this->orgFile.print(text.c_str()); // Schreibt Char Array in die Datei
    }else{
        status = false;
    }
    this->orgFile.close();
    return status;
}

// override fFile with string, with CR&LF
bool XFile::writeStrLn(String const& text)
{
    String text_LF = text + XFile::LINE_FEED;
    return this->writeStr(text_LF);
}

// append string on current file
bool XFile::appendStr(String const& text)
{
    //FUNCTION_TIME_X("XFile::appendStr(String const& text)")
    bool status = true;
    this->orgFile = this->sdCard.open(this->getTotalFilePath().c_str(), O_RDWR | O_CREAT);
    if(this->orgFile)
    {
        this->orgFile.seekEnd(0);
        this->orgFile.print(text.c_str());
    }else{
        status = false;
    }
    this->orgFile.close();
    return status;
}

// append string on current file, with CR&LF
bool XFile::appendStrLn(String const& text)
{
    String text_LF = text + XFile::LINE_FEED;
    return this->appendStr(text_LF);
}

// set filepath of the current file
void XFile::setFilePath(String const& filePath)
{
    this->filePath = filePath;
}

// set the filename
void XFile::setFileName(String const& fileName)
{
    this->fileName = fileName;
}

// check if the current file with filepath exists
bool XFile::exists()
{
    bool ret = this->sdCard.chdir(this->filePath.c_str()); // Workingdirectory setzen
    if(ret == false)
    {
        String str = "Das Verzeichnis: ";
        str += this->filePath;
        str += " konnte nicht als Arbeitsverzeichnis gesetzt werden.";
        scom.printError(str);
        return false;
    }
    return this->sdCard.exists(this->getFileName().c_str());
}

// return the current filepath
String const& XFile::getFilePath() const
{
    return this->filePath;
}

// return the current filename
String const& XFile::getFileName() const
{
    return this->fileName;
}

// return the current total filepath
String XFile::getTotalFilePath() const
{
    return this->filePath + "/" +this->fileName;
}