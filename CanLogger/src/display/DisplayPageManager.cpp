#include "display/DisplayPageManager.hpp"

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

    page->startView();
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

    if(this->pageArray[this->pageArraySize-1])
    {
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
        
        this->pageArray[this->pageArraySize-1]->startView();
    }
    
    return true;
}
