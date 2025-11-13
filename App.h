#pragma once 
#include <wx/wx.h>

class App : public wxApp
{
public:
    // Ta funkcja jest wywo³ywana na starcie aplikacji
    virtual bool OnInit();
};