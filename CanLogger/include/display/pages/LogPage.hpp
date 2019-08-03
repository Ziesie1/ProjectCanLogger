#ifndef LOG_PAGE_HPP
#define LOG_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"
#include "display/elements/Button.hpp"

class LogPage : public DisplayPage
{

protected:
    ILI9341& display;
    bool statusSD;
    
public:
    LogPage(ILI9341& display, bool statusSD);
    void loop() override;
    void startView() override;
    ~LogPage();
};


#endif