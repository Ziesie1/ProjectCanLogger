#include "display/pages/DisplayPage.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

/*
    Will be called if the PageManager open another Page.
    During this time, the Page is paused.
    input: 
    return: 
*/
void DisplayPage::pauseView()
{ }

/*
    Will be called if the PageManager delete the Page.
    input: 
    return: 
*/
void DisplayPage::closeView()
{ }