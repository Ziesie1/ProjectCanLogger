#ifndef LOG_PAGE_HPP
#define LOG_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"

class LogPage : public DisplayPage
{

protected:
    bool statusSD;
    ILI9341& display;
public:
    LogPage(ILI9341& display, bool statusSD);
    void loop() override;
    void startView() override;
};


#endif