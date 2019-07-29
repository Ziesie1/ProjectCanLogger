#ifndef DISPLAY_PAGE_MANAGER_HPP
#define DISPLAY_PAGE_MANAGER_HPP


#include "display/pages/DisplayPage.hpp"

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