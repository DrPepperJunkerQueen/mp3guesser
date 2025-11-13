#pragma once
#include <wx/wx.h>

// Deklaracja klasy naszego okna
class ProfileFrame : public wxFrame
{
private:
    wxPanel* m_panel; // Bêdziemy przechowywaæ wskaŸnik do panelu
    wxBitmap m_backgroundBitmap; // Tutaj za³adujemy nasz obrazek t³a

    // Deklaracja funkcji, która bêdzie rysowaæ t³o
    void OnPaint(wxPaintEvent& event);
public:
    // Mamy tylko konstruktor
    ProfileFrame();

    // Nic wiêcej! ¯adnych prywatnych funkcji, ¿adnych makr.
};