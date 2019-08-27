#ifndef DISPLAY_PAGE_HPP
#define DISPLAY_PAGE_HPP

/*
    Von dieser Klasse können verschiedene Pages mit unterschiedlichen Aufgaben erstellt werden.
    Eine DisplayPage wird vom DisplayPageManager verwaltet.
*/
class DisplayPage
{

protected:
public:
    virtual void loop() = 0;
    virtual void startView() = 0;
    virtual void pauseView();
    virtual void reloadView() = 0;
    virtual void closeView();
    virtual ~DisplayPage(){}
};


#endif