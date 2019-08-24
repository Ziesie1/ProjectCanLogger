#include "display/pages/DisplayPage.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

/*
    It reset the hardware encoder and button. Shortly afterward the page will be loaded.
    input:  ---
    return: ---
*/
void DisplayPage::startView()
{
    getEncoderPos();
    setEncoderPos(0);
    wasEncoderButtonPressed();
    wasSingleTasterPressed();

    this->loadStartView();
}
