#include "sd/XFile.hpp"
#include "serial/SerialCommunication.hpp"
#include "serial/FunctionLifeTime.hpp"

using namespace utilities;


XFile::XFile(SdFat& sdCard)
    :sdCard{sdCard}
{ }


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

void XFile::setFilePath(String const& filePath)
{
    this->filePath = filePath;
}

void XFile::setFileName(String const& fileName)
{
    this->fileName = fileName;
}

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

String const& XFile::getFilePath() const
{
    return this->filePath;
}

String const& XFile::getFileName() const
{
    return this->fileName;
}

String XFile::getTotalFilePath() const
{
    return this->filePath + "/" +this->fileName;
}