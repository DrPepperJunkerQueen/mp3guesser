#pragma once
#include "BackgroundFrame.h" // Dziedziczymy po BackgroundFrame
#include "Profile.h"
#include "SongLibrary.h"

class GameFrame : public BackgroundFrame // <-- ZMIANA TUTAJ
{
public:
    GameFrame(const Profile& userProfile, SongLibrary& songLib);

private:
    Profile m_userProfile;
    SongLibrary& m_songLibRef;
    void OnPlay(wxCommandEvent& event);
    void OnMainMenu(wxCommandEvent& event);
};