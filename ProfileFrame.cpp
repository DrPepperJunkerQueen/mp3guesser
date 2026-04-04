#include "ProfileFrame.h"
#include "SelectProfileDialog.h"
#include "GameFrame.h"
#include "BackgroundFrame.h"
#include <wx/textdlg.h>
#include <wx/msgdlg.h>
#include <iostream>
#include "Definitions.h"
#include <wx/busyinfo.h> // Do okna ³adowania

enum
{
    ID_CreateProfileBtn = wxID_HIGHEST + 1,
    ID_SelectProfileBtn,
    ID_RescanBtn // <-- NOWE ID
};

wxBEGIN_EVENT_TABLE(ProfileFrame, BackgroundFrame)
EVT_BUTTON(ID_CreateProfileBtn, ProfileFrame::OnCreateProfile)
EVT_BUTTON(ID_SelectProfileBtn, ProfileFrame::OnSelectProfile)
EVT_BUTTON(ID_RescanBtn, ProfileFrame::OnRescanLibrary) // <-- NOWE POWI¥ZANIE
wxEND_EVENT_TABLE()

// Zmieñ konstruktor
ProfileFrame::ProfileFrame(SongLibrary& songLib)
    : BackgroundFrame(NULL, wxID_ANY, "MP3 Guesser - Profile Selection"),
    m_selectedProfile(Profile()),
    m_songLibRef(songLib) // <-- Zainicjuj referencjê
{
    wxButton* createBtn = new wxButton(m_panel, ID_CreateProfileBtn, "Create Profile");
    wxButton* selectBtn = new wxButton(m_panel, ID_SelectProfileBtn, "Select Profile");
    wxButton* rescanBtn = new wxButton(m_panel, ID_RescanBtn, "Rescan Library"); // <-- NOWY PRZYCISK

    wxSize buttonSize = wxSize(250, 60);
    createBtn->SetMinSize(buttonSize);
    selectBtn->SetMinSize(buttonSize);
    rescanBtn->SetMinSize(buttonSize); // <-- Ustaw rozmiar

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddStretchSpacer(1);
    sizer->Add(createBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    sizer->Add(selectBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    sizer->Add(rescanBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10); // <-- Dodaj do sizera
    sizer->AddStretchSpacer(1);

    m_panel->SetSizer(sizer);
    m_panel->Layout();
}

// --- DODAJ NOW¥ FUNKCJÊ ---
void ProfileFrame::OnRescanLibrary(wxCommandEvent& event)
{
    // Zapytaj u¿ytkownika, czy jest pewien
    if (wxMessageBox("Rescanning the library may take several minutes. Are you sure?",
        "Confirm Rescan",
        wxYES_NO | wxICON_QUESTION, this) == wxNO)
    {
        return; // U¿ytkownik anulowa³
    }

    { // Blok dla wxBusyInfo
        wxBusyInfo info("Re-scanning music library, please wait...");
        wxYield();

        // U¿yj referencji, aby wywo³aæ skanowanie
        m_songLibRef.ScanAndSave(MUSIC_LIBRARY_FOLDER);
    }

    wxMessageBox("Library rescan complete! Found " +
        std::to_string(m_songLibRef.GetSongCount()) + " songs.",
        "Success", wxOK | wxICON_INFORMATION, this);
}

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

        GameFrame* game = new GameFrame(newProfile, m_songLibRef); // <-- Przeka¿ referencjê
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
            // ZMIANA TUTAJ: Musimy przekazaæ bibliotekê do GameFrame
            GameFrame* game = new GameFrame(chosenProfile, m_songLibRef); // <-- Przeka¿ referencjê
            game->Show();
            this->Close();
        }
        else
        {
            wxMessageBox("No profile selected.", "Info", wxOK | wxICON_INFORMATION);
        }
    }
}