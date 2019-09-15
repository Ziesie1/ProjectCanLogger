#include "sd/XFile.hpp"
#include "utilities/SerialCommunication.hpp"
#include "utilities/FunctionLifeTime.hpp"

using namespace utilities;

const String XFile::LINE_FEED = "\r\n";

XFile::XFile(SdFat& sdCard)
    :sdCard{sdCard}
{ }

// // override file with string
// bool XFile::writeStr(String const& text)
// {
//     //FUNCTION_TIME_X("bool XFile::writeStr(String const& text)")
//     bool status = true;
//     this->orgFile = this->sdCard.open(this->getTotalFilePath().c_str(), O_RDWR | O_CREAT | O_TRUNC);
//     if(this->orgFile)
//     {
//         this->orgFile.print(text.c_str()); // Schreibt Char Array in die Datei
//     }else{
//         status = false;
//     }
//     this->orgFile.close();
//     return status;
// }

// // override fFile with string, with CR&LF
// bool XFile::writeStrLn(String const& text)
// {
//     String text_LF = text + XFile::LINE_FEED;
//     return this->writeStr(text_LF);
// }

/*
    Open/Create the file on SD-Card.
    Before appendStr() must be called.
    input: ---
    return: true - succesful
            false - failure
*/
bool XFile::open()
{
    this->orgFile = this->sdCard.open(this->getTotalFilePath().c_str(), O_RDWR | O_CREAT); // 9-10 ms
    if(!this->orgFile)
    {
        return false;
    }
    return true;
}

/*
    Write the last bytes and close the file. 
    input: ---
    return: true - succesful
            false - failure
*/
bool XFile::close()
{
    return this->orgFile.close();
}

/*
    Append a string on the current file.
    Open() must be called before.
    input:  text - the text to write into the file
    return: true - succesful
            false - failure
*/
bool XFile::appendStr(String const& text)
{
    //FUNCTION_TIME_X("XFile::appendStr(String const& text)") // 5 ms
    bool status = true;

    if(this->orgFile)
    {
        this->orgFile.seekEnd(0); // 0 ms
        this->orgFile.print(text.c_str()); // 4-5 ms
    }else{
        status = false;
    }
    
    return status;
}

/*
    Append a string on the current file, with LF&CR.
    Open() must be called before.
    input:  text - the text to write into the file
    return: true - succesful
            false - failure
*/
bool XFile::appendStrLn(String const& text)
{
    String text_LF = text + XFile::LINE_FEED;
    return this->appendStr(text_LF);
}

/*
    Clear the hole file. Afterwards the file is empty.
    input:  
    return: true - succesful
            false - failure
*/
bool XFile::clear()
{
    return this->orgFile.remove();
}

/*
    Set the filepath for the file.
    input:  filePath - filepath like "/folder/.."
    return:  
*/
void XFile::setFilePath(String const& filePath)
{
    this->filePath = filePath;
}

/*
    Set the filename for the file.
    input:  fileName - filename like "file.txt"
    return:  
*/
void XFile::setFileName(String const& fileName)
{
    this->fileName = fileName;
}

/*
    Check if the file exists.
    input:  
    return: true - file exists
            false - file dosen't exists
*/
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

/*
    Return the filepath of the file.
    input:
    return: String - the current filepath 
*/
String const& XFile::getFilePath() const
{
    return this->filePath;
}

/*
    Return the filename of the file.
    input:
    return: String - the current filename 
*/
String const& XFile::getFileName() const
{
    return this->fileName;
}

/*
    Return the total filepath of the file.
    input:
    return: String - the current total filepath 
*/
String XFile::getTotalFilePath() const
{
    return this->filePath + "/" +this->fileName;
}