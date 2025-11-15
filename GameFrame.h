#pragma once
#include "BackgroundFrame.h" // Dziedziczymy po BackgroundFrame
#include "Profile.h"

class GameFrame : public BackgroundFrame // <-- ZMIANA TUTAJ
{
public:
    GameFrame(const Profile& userProfile);

private:
    Profile m_userProfile;
    void OnPlay(wxCommandEvent& event);
    void OnMainMenu(wxCommandEvent& event);
};