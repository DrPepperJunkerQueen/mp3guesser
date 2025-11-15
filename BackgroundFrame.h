#pragma once
#include <wx/wx.h>
#include "Definitions.h" // Potrzebuje œcie¿ki do t³a

class BackgroundFrame : public wxFrame
{
public:
    // Konstruktor
    BackgroundFrame(wxWindow* parent, wxWindowID id, const wxString& title);

protected:
    // Te zmienne bêd¹ dostêpne dla klas-dzieci (ProfileFrame i GameFrame)
    wxPanel* m_panel;
    wxBitmap m_backgroundBitmap;

private:
    // Ta funkcja bêdzie obs³ugiwaæ t³o dla wszystkich klas-dzieci
    void OnPaint(wxPaintEvent& event);
};
