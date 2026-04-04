#pragma once
#include "BackgroundFrame.h"
#include "Profile.h"
#include "SongLibrary.h" // <-- DODAJ INCLUDE

class ProfileFrame : public BackgroundFrame
{
public:
    // Zmieñ konstruktor, aby przyjmowa³ referencjê do biblioteki
    ProfileFrame(SongLibrary& songLib);

private:
    SongLibrary& m_songLibRef; // <-- Przechowaj referencjê
    Profile m_selectedProfile;

    void OnCreateProfile(wxCommandEvent& event);
    void OnSelectProfile(wxCommandEvent& event);
    void OnRescanLibrary(wxCommandEvent& event); // <-- NOWA FUNKCJA

    wxDECLARE_EVENT_TABLE();
};