#include "display/DisplayPageManager.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

void clearButtonState();

DisplayPageManager::DisplayPageManager()
{ }

DisplayPageManager::~DisplayPageManager()
{
    if(this->pageArray)
    {
        // löschen aller Pages
        for(int i = 0; i < this->pageArraySize; i++)
        {
            if(this->pageArray[i])
            {
                delete this->pageArray[i];
            }
            this->pageArray[i] = nullptr;
        }
        delete[] this->pageArray; // array löschen
    }
    this->pageArraySize = 0;
}

/*
    Loop the current Page.
*/
void DisplayPageManager::loop()
{
    if(!this->pageArray)
    {
        return;
    }
    if(this->pageArray[this->pageArraySize-1])
    {
        this->pageArray[this->pageArraySize-1]->loop();
    }
}

/*
    Add a Page to the Array.
    Show the Startview of the Page.
*/
void DisplayPageManager::openNewPage(DisplayPage* page)
{
    clearButtonState();

    if(!this->pageArray)
    {
        // noch keine Page vorhanden
        this->pageArray = new DisplayPage*[1];
        this->pageArray[0] = page;
        this->pageArraySize = 1;
        page->startView();
        return;
    }
    
    // Array kopieren
    DisplayPage* copyArray[this->pageArraySize];
    for(int i = 0; i < this->pageArraySize; i++)
    {
        copyArray[i] = this->pageArray[i];
    }

    // altes Array löschen
    delete[] this->pageArray;

    // neus Array erstellen
    this->pageArray = new DisplayPage*[this->pageArraySize+1];
    for(int i = 0; i < this->pageArraySize; i++)
    {
        this->pageArray[i] = copyArray[i];
    }
    this->pageArray[this->pageArraySize] = page;
    this->pageArraySize++;

    this->pageArray[this->pageArraySize-1]->pauseView(); // alte Page pausieren
    page->startView(); // neue Page aufrufen
}

/*
    Delete the current Page and show the Page before.
*/
bool DisplayPageManager::deleteOpenPage()
{
    if(!this->pageArray)
    {
        return false;
    }

    clearButtonState();

    if(this->pageArray[this->pageArraySize-1])
    {
        this->pageArray[this->pageArraySize-1]->closeView(); // alte Page auf das schließen vorbereiten
        delete this->pageArray[this->pageArraySize-1]; // seite löschen
        this->pageArray[this->pageArraySize-1] = nullptr;
    }

    this->pageArraySize--;
    if(this->pageArraySize == 0)
    {
        delete[] this->pageArray;
        this->pageArray = nullptr;
    }
    else
    {
        this->pageArray[this->pageArraySize-1]->reloadView(); // alte Page zum neuladen aufrufen
    }
    
    return true;
}

/*
    Clear alle states of the Buttons.
*/
void clearButtonState()
{
    resetEncoderStates();
    wasSingleTasterPressed();
}