#pragma once 
#include <wx/wx.h>
#include "SongLibrary.h"

class App : public wxApp
{
private:
    SongLibrary m_songLibrary;
public:
    // Ta funkcja jest wywo³ywana na starcie aplikacji
    virtual bool OnInit();
};