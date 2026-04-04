#pragma once
#include "BackgroundFrame.h" // Dziedziczy z t³a
#include "Profile.h"
#include "SongLibrary.h"

class ModeSelectFrame : public BackgroundFrame
{
public:
    // Musi przyj¹æ profil, aby móc go przekazaæ z powrotem
    ModeSelectFrame(const Profile& userProfile, SongLibrary& songLib);

private:
    Profile m_userProfile; // Przechowuje profil gracza
    SongLibrary& m_songLibRef;

    // Funkcje dla przycisków
    void OnPlay20Rounds(wxCommandEvent& event);
    void OnPlayInfinite(wxCommandEvent& event);
    void OnBack(wxCommandEvent& event); // Przycisk powrotu
};