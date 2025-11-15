#pragma once
#include <wx/wx.h>
#include "BackgroundFrame.h"
#include "Profile.h"

// Deklaracja klasy naszego okna
class ProfileFrame : public BackgroundFrame
{
private:
    Profile m_selectedProfile;

    // Deklaracje funkcji obs³uguj¹cych klikniêcia
    void OnCreateProfile(wxCommandEvent& event);
    void OnSelectProfile(wxCommandEvent& event);

    // Makro do ³¹czenia zdarzeñ
    wxDECLARE_EVENT_TABLE();
public:
    // Mamy tylko konstruktor
    ProfileFrame();

    // Nic wiêcej! ¯adnych prywatnych funkcji, ¿adnych makr.
};