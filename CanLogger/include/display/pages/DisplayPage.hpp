#ifndef DISPLAY_PAGE_HPP
#define DISPLAY_PAGE_HPP


class DisplayPage
{

protected:
public:
    virtual void loop() = 0;
    virtual void startView();
    virtual void loadStartView() = 0;
    virtual ~DisplayPage(){}
};


#endif