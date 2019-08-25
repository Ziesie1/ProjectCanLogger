#ifndef DISPLAY_PAGE_MANAGER_HPP
#define DISPLAY_PAGE_MANAGER_HPP


#include "display/pages/DisplayPage.hpp"

/*
    Die Klasse dient zur Verwaltung verschiedener DisplayPages.
    Dabei übergibt man der Klasse eine DisplayPage und der DisplayPageManager ruft diese auf und löscht sie später. 
    Es gibt eine Page im Fordergrund, wohingegen die anderen Pages auf dem Stack liegen und inaktiv sind.
*/
class DisplayPageManager
{
    DisplayPage** pageArray = nullptr;
    int pageArraySize = 0;

public:
    DisplayPageManager();
    ~DisplayPageManager();
    DisplayPageManager(DisplayPageManager const& pageManager) = delete;
    void operator=(DisplayPageManager const& pageManager) = delete;
    void loop();
    void openNewPage(DisplayPage* page);
    bool deleteOpenPage();
};

#endif