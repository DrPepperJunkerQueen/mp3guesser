#include "ModeSelectFrame.h"
#include "GameFrame.h" // Musi wiedzieæ, jak wróciæ do GameFrame
#include <wx/msgdlg.h>
#include "PlaySessionFrame.h"

enum {
    ID_Mode20Rounds = 1,
    ID_ModeInfinite,
    ID_ModeBack
};

ModeSelectFrame::ModeSelectFrame(const Profile& userProfile, SongLibrary& songLib)
    : BackgroundFrame(NULL, wxID_ANY, "MP3 Guesser - Select Mode"),
    m_userProfile(userProfile), // Zapisz profil
	m_songLibRef(songLib) // Zapisz referencjê do biblioteki
{
    // Stwórz przyciski
    wxButton* play20Btn = new wxButton(m_panel, ID_Mode20Rounds, "Straight Up 20");
    wxButton* playInfiniteBtn = new wxButton(m_panel, ID_ModeInfinite, wxT("\u221E"));
    wxButton* backBtn = new wxButton(m_panel, ID_ModeBack, "Back");

    wxSize buttonSize = wxSize(250, 60);
    play20Btn->SetMinSize(buttonSize);
    playInfiniteBtn->SetMinSize(buttonSize);
    backBtn->SetMinSize(buttonSize);

    // U³ó¿ je
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer(1);
    sizer->Add(play20Btn, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(playInfiniteBtn, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->Add(backBtn, 0, wxALIGN_CENTER | wxALL, 10);
    sizer->AddStretchSpacer(1);

    m_panel->SetSizer(sizer);

    // Podepnij logikê
    play20Btn->Bind(wxEVT_BUTTON, &ModeSelectFrame::OnPlay20Rounds, this);
    playInfiniteBtn->Bind(wxEVT_BUTTON, &ModeSelectFrame::OnPlayInfinite, this);
    backBtn->Bind(wxEVT_BUTTON, &ModeSelectFrame::OnBack, this);

    m_panel->Layout(); // Przelicz sizer
}

void ModeSelectFrame::OnPlay20Rounds(wxCommandEvent& event)
{
    // Tworzymy now¹ sesjê gry:
    // 1. Przekazujemy profil gracza (m_userProfile)
    // 2. Przekazujemy bibliotekê piosenek (m_songLibRef)
    // 3. Przekazujemy tryb gry: 20 (oznacza 20 rund)
    PlaySessionFrame* gameSession = new PlaySessionFrame(m_userProfile, m_songLibRef, 20);

    // Poka¿ okno gry
    gameSession->Show();

    // Zamknij obecne okno wyboru trybu
    this->Close();
}

void ModeSelectFrame::OnPlayInfinite(wxCommandEvent& event)
{
    wxMessageBox("Starting a new infinite game!", "TO DO", wxOK | wxICON_INFORMATION);
    // TODO: Tutaj otworzysz w³aœciw¹ scenê gry (np. "new PlaySessionFrame(m_userProfile, -1)")
}

void ModeSelectFrame::OnBack(wxCommandEvent& event)
{
    // POPRAWKA: Musimy przekazaæ bibliotekê z powrotem do GameFrame
    GameFrame* gameMenu = new GameFrame(m_userProfile, m_songLibRef);
    gameMenu->Show();
    this->Close(); // Zamknij obecne okno (ModeSelectFrame)
}