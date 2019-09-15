#ifndef SETTING_PAGE_HPP
#define SETTING_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"
#include "display/elements/Table.hpp"
#include "display/elements/Kopfzeile.hpp"
//#include "display/elements/Button.hpp"
//#include "display/elements/SelectionButton.hpp"
#include "display/elements/SpeedSelectionButton.hpp"
#include "display/elements/SilentSelectionButton.hpp"


class SettingPage : public DisplayPage
{
protected:
    ILI9341& display;
    Kopfzeile* header;
    const uint16_t DISPLAY_X = 240;
    const uint16_t DISPLAY_Y = 320;
    const long COLOR_BLACK_SETTING_WRITING = BLACK;
    const long COLOR_WHITE_BACKGROUND_SETTING = WHITE;
    const String TRANSFER_RATE = "Uebertragungsrate:";
    const String SILENT_MODE = "Silent-Mode:";
    const uint16_t TRANSFER_RATE_SILENT_MODE_POS_X = 2;
    const uint16_t TRANSFER_RATE_POS_Y = 32;
    const uint16_t SILENT_MODE_POS_Y = 60;
    const byte BUTTON_WIDTH = 100;
    const byte BUTTON_HIGH = 32;
    const byte BUTTON_SPECIAL_WIDTH = 90;
    const byte BUTTON_SPECIAL_HIGH = 20;
    const byte BUTTON_SPECIAL_POS_X = 147;
    const uint16_t BUTTON_POS_Y = 280;
    const String BUTTON_DISCARD_TEXT = "Verwerfen";
    const String BUTTON_STORE_TEXT = "Speichern";
    const unsigned long COLOR_BUTTON_DEFAULT = Table::COLOR_BLUE_BACKGROUND_HEADER;
    const unsigned long COLOR_BUTTON_TEXT = WHITE;
    Button* buttonDiscard;
    Button* buttonStore;
    SpeedSelectionButton* speedSelect;
    SilentSelectionButton* silentSelect;

    int lastEncoderPos = 0; // Position des Drehimpulsgebers

public:
    SettingPage(ILI9341& display);
    void loop() override;
    void startView() override;
    void reloadView() override;
    void closeView() override;
    ~SettingPage();
    void setElements();
    
};

#endif