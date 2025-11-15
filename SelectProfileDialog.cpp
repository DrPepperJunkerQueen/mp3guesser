#include "SelectProfileDialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

SelectProfileDialog::SelectProfileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Select Profile", wxDefaultPosition, wxSize(300, 400)),
    m_selectedProfile() // U¿yj konstruktora domyœlnego (ustawi ID na -1)
{
    // --- G£ÓWNA LOGIKA ---
    // 1. Za³aduj wszystkie profile U¯YWAJ¥C FUNKCJI STATYCZNEJ
    // Zak³adam, ¿e doda³eœ 'static' do readAllProfiles() w Profile.h i .cpp
    m_profiles = Profile::readAllProfiles();

    // --- BUDOWANIE INTERFEJSU ---
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // 1. Etykieta na górze
    mainSizer->Add(
        new wxStaticText(this, wxID_ANY, "Please select a profile:"),
        0, // Proporcja 0 (nie roœnie)
        wxALL, // Margines z ka¿dej strony
        10   // Rozmiar marginesu
    );

    // 2. Lista profili
    m_listBox = new wxListBox(this, wxID_ANY);
    mainSizer->Add(
        m_listBox,
        1, // Proporcja 1 (roœnie, aby wype³niæ miejsce)
        wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, // Rozci¹gnij i dodaj marginesy
        10
    );

    // 3. Wype³nij listê nazwami profili
    if (m_profiles.empty())
    {
        m_listBox->Append("No profiles found. Please create one.");
        m_listBox->Enable(false); // Wyszarz listê
    }
    else
    {
        for (const Profile& profile : m_profiles)
        {
            // Dodaj nazwê profilu do listy
            m_listBox->Append(profile.getName());
        }
    }

    // 4. Przyciski OK i Anuluj
    // wxStdDialogButtonSizer automatycznie tworzy przyciski OK/Anuluj
    wxSizer* buttonSizer = CreateStdDialogButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(mainSizer); // Ustaw sizer i dopasuj rozmiar okna
    CenterOnParent(); // Wyœrodkuj wzglêdem okna ProfileFrame

    // --- PODPIÊCIE LOGIKI ---
    // ZnajdŸ przycisk OK (który zosta³ stworzony przez CreateStdDialogButtonSizer)
    // i podepnij pod niego nasz¹ funkcjê OnOK
    wxButton* okButton = (wxButton*)FindWindowById(wxID_OK);
    okButton->Bind(wxEVT_BUTTON, &SelectProfileDialog::OnOK, this);
}

// Ta funkcja jest wywo³ywana, gdy u¿ytkownik kliknie "OK"
void SelectProfileDialog::OnOK(wxCommandEvent& event)
{
    // Pobierz numer (indeks) zaznaczonej pozycji
    int selectionIndex = m_listBox->GetSelection();

    // SprawdŸ, czy cokolwiek jest zaznaczone I czy lista profili nie jest pusta
    if (selectionIndex != wxNOT_FOUND && !m_profiles.empty())
    {
        // Skopiuj wybrany profil (o tym indeksie) do zmiennej m_selectedProfile
        m_selectedProfile = m_profiles[selectionIndex];
    }

    // Pozwól dialogowi siê normalnie zamkn¹æ
    event.Skip();
}

// Zwraca skopiowany obiekt profilu
Profile SelectProfileDialog::getSelectedProfile() const
{
    return m_selectedProfile;
}