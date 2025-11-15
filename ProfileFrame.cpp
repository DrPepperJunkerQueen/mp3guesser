#include "ProfileFrame.h"
#include "SelectProfileDialog.h"
#include "GameFrame.h"
#include "BackgroundFrame.h"
#include <wx/textdlg.h>
#include <wx/msgdlg.h>
#include <iostream>

// ... (enum z ID przycisków - bez zmian) ...
enum
{
    ID_CreateProfileBtn = wxID_HIGHEST + 1,
    ID_SelectProfileBtn
};

// Z Event Table ZNIKA EVT_PAINT
wxBEGIN_EVENT_TABLE(ProfileFrame, BackgroundFrame) // <-- ZMIANA TUTAJ
EVT_BUTTON(ID_CreateProfileBtn, ProfileFrame::OnCreateProfile)
EVT_BUTTON(ID_SelectProfileBtn, ProfileFrame::OnSelectProfile)
wxEND_EVENT_TABLE()

// Konstruktor jest teraz malutki
ProfileFrame::ProfileFrame()
    : BackgroundFrame(NULL, wxID_ANY, "MP3 Guesser - Profile Selection"), // <-- ZMIANA TUTAJ
    m_selectedProfile(Profile()) // U¿yj konstruktora domyœlnego
{
    // ZNIKN¥£ CA£Y KOD T£A I PANELU!
    // Ju¿ jest w BackgroundFrame.
    // m_panel jest teraz chronion¹ zmienn¹ odziedziczon¹ z rodzica.

    wxButton* createBtn = new wxButton(m_panel, ID_CreateProfileBtn, "Create Profile");
    wxButton* selectBtn = new wxButton(m_panel, ID_SelectProfileBtn, "Select Profile");

    wxSize buttonSize = wxSize(250, 60);
    createBtn->SetMinSize(buttonSize);
    selectBtn->SetMinSize(buttonSize);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer(1);
    sizer->Add(createBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    sizer->Add(selectBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    sizer->AddStretchSpacer(1);

    m_panel->SetSizer(sizer);
    // Ustawienia rozmiaru i centrowania s¹ ju¿ w klasie bazowej\

    m_panel->Layout();
}

// ZNIKNÊ£A funkcja OnPaint! Jest w klasie bazowej.

// ... (Funkcje OnCreateProfile i OnSelectProfile - bez zmian) ...
// (Pamiêtaj, ¿eby dodaæ #include "SelectProfileDialog.h" i "GameFrame.h")
void ProfileFrame::OnCreateProfile(wxCommandEvent& event)
{
    wxTextEntryDialog dialog(this, "Enter your profile name:", "Create Profile");
    if (dialog.ShowModal() == wxID_OK)
    {
        string name = dialog.GetValue().ToStdString();
        if (name.empty())
        {
            wxMessageBox("Name cannot be empty!", "Error", wxOK | wxICON_ERROR);
            return;
        }
        Profile newProfile(name);
        newProfile.createProfile();

        // PrzejdŸ od razu do gry
        GameFrame* game = new GameFrame(newProfile);
        game->Show();
        this->Close();
    }
}

void ProfileFrame::OnSelectProfile(wxCommandEvent& event)
{
    SelectProfileDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {
        Profile chosenProfile = dialog.getSelectedProfile();
        if (chosenProfile.getID() != -1)
        {
            GameFrame* game = new GameFrame(chosenProfile);
            game->Show();
            this->Close();
        }
        else
        {
            wxMessageBox("No profile selected.", "Info", wxOK | wxICON_INFORMATION);
        }
    }
}