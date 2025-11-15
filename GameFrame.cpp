#include "GameFrame.h"
#include "ProfileFrame.h"
#include <wx/msgdlg.h>

// ... (enum z ID_PlayBtn - bez zmian) ...
enum {
        ID_PlayBtn = 1,
        ID_MainMenuBtn
     };

GameFrame::GameFrame(const Profile& userProfile)
    : BackgroundFrame(NULL, wxID_ANY, "MP3 Guesser - Let's Play!"),
    m_userProfile(userProfile)
{
    SetTitle("MP3 Guesser - Welcome, " + m_userProfile.getName() + "!");

    // ZNIKN¥£ KOD TWORZENIA PANELU!
    // U¿ywamy m_panel odziedziczonego z BackgroundFrame.

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
    // Ustawienia rozmiaru i centrowania s¹ ju¿ w klasie bazowej

    playBtn->Bind(wxEVT_BUTTON, &GameFrame::OnPlay, this);
    mainMenuBtn->Bind(wxEVT_BUTTON, &GameFrame::OnMainMenu, this);

    m_panel->Layout();
}

void GameFrame::OnPlay(wxCommandEvent& event)
{
    wxMessageBox("Here the game will start!", "TO DO", wxOK | wxICON_INFORMATION);
}

void GameFrame::OnMainMenu(wxCommandEvent& event)
{
    ProfileFrame* menu = new ProfileFrame();

    // 2. Poka¿ je
    menu->Show();

    // 3. Zamknij (zniszcz) obecne okno (GameFrame)
    this->Close();
}