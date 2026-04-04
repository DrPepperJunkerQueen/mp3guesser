#include "GameFrame.h"
#include "ProfileFrame.h"    // Potrzebny do OnMainMenu
#include "ModeSelectFrame.h" // Potrzebny do OnPlay
#include <wx/msgdlg.h>

enum {
    ID_PlayBtn = 1,
    ID_MainMenuBtn
};

// ZMIANA: Zaktualizowany konstruktor i lista inicjalizacyjna
GameFrame::GameFrame(const Profile& userProfile, SongLibrary& songLib)
    : BackgroundFrame(NULL, wxID_ANY, "MP3 Guesser - Let's Play!"),
    m_userProfile(userProfile),
    m_songLibRef(songLib) // <-- ZAPISZ REFERENCJÊ
{
    SetTitle("MP3 Guesser - Welcome, " + m_userProfile.getName() + "!");

    // Reszta konstruktora (tworzenie przycisków, sizer, layout)
    // pozostaje BEZ ZMIAN
    wxButton* playBtn = new wxButton(m_panel, ID_PlayBtn, "Play");
    wxButton* myProfileBtn = new wxButton(m_panel, wxID_ANY, "My Profile");
    wxButton* mainMenuBtn = new wxButton(m_panel, ID_MainMenuBtn, "Main Menu");

    wxSize buttonSize = wxSize(250, 60);
    playBtn->SetMinSize(buttonSize);
    myProfileBtn->SetMinSize(buttonSize);
    mainMenuBtn->SetMinSize(buttonSize);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer(1);
    sizer->Add(playBtn, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(myProfileBtn, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(mainMenuBtn, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->AddStretchSpacer(1);

    m_panel->SetSizer(sizer);

    playBtn->Bind(wxEVT_BUTTON, &GameFrame::OnPlay, this);
    mainMenuBtn->Bind(wxEVT_BUTTON, &GameFrame::OnMainMenu, this);

    m_panel->Layout();
}

// ZMIANA: Funkcja OnPlay musi teraz przekazaæ DALEJ bibliotekê
void GameFrame::OnPlay(wxCommandEvent& event)
{
    // SprawdŸ, czy biblioteka ma jakiekolwiek piosenki, ZANIM przejdziesz dalej
    if (m_songLibRef.GetSongCount() == 0)
    {
        wxMessageBox(
            "Your song library is empty.\n\n"
            "Please return to the main menu (Profile Selection) and use 'Rescan Library'.",
            "No Songs to Play",
            wxOK | wxICON_WARNING
        );
        return; // ZATRZYMAJ - nie otwieraj okna wyboru trybu
    }

    // Przeka¿ profil ORAZ bibliotekê piosenek
    ModeSelectFrame* modeSelect = new ModeSelectFrame(m_userProfile, m_songLibRef);
    modeSelect->Show();
    this->Close();
}

// ZMIANA: Funkcja OnMainMenu te¿ musi przekazaæ bibliotekê
void GameFrame::OnMainMenu(wxCommandEvent& event)
{
    // Konstruktor ProfileFrame teraz te¿ wymaga biblioteki
    ProfileFrame* menu = new ProfileFrame(m_songLibRef);
    menu->Show();
    this->Close();
}