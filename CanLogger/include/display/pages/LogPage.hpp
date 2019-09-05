#ifndef LOG_PAGE_HPP
#define LOG_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"
#include "display/elements/Table.hpp"
#include"display/elements/Kopfzeile.hpp"

/*
    This Class creates the displaypage where the "Table" and other iformations are printet on while canlogger is running. 
*/
class LogPage : public DisplayPage
{

protected:
    ILI9341& display;
    bool statusSD;
    Table* logTable = nullptr;
    String headline;
    Kopfzeile* kopfzeile;
   
    
public:
    LogPage(ILI9341& display, bool statusSD, String headline);
    void loop() override;
    void startView() override;
    void reloadView() override;
    void closeView() override;
    ~LogPage();
    
};


#endif