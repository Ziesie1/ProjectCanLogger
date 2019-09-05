#ifndef SETTING_PAGE_HPP
#define SETTING_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"
#include "display/elements/Table.hpp"
#include"display/elements/Kopfzeile.hpp"

class SettingPage : public DisplayPage
{
    protected:
    ILI9341& display;
    String kopfzeile;
    Kopfzeile* header;

    public:
    SettingPage(ILI9341& display,  String headline);
    void loop() override;
    void startView() override;
    void reloadView() override;
    void closeView() override;
    ~SettingPage();
    
};

#endif