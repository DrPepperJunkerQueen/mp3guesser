#pragma once

#include <wx/wx.h>
#include <wx/listbox.h> // Potrzebujemy do³¹czenia dla listy
#include <vector>
#include "Profile.h" // Musi wiedzieæ, czym jest klasa Profile

class SelectProfileDialog : public wxDialog
{
public:
    // Konstruktor: przyjmuje wskaŸnik do okna-rodzica
    SelectProfileDialog(wxWindow* parent);

    // Zwróci skopiowany obiekt wybranego profilu
    // (Zwróci "pusty" profil, jeœli nic nie wybrano)
    Profile getSelectedProfile() const;

private:
    // Funkcja wywo³ywana, gdy u¿ytkownik kliknie "OK"
    void OnOK(wxCommandEvent& event);

    // --- Zmienne cz³onkowskie ---

    // WskaŸnik na nasz¹ listê w interfejsie
    wxListBox* m_listBox;

    // Przechowujemy tu wszystkie wczytane profile
    std::vector<Profile> m_profiles;

    // Przechowujemy tu profil, który u¿ytkownik wybra³
    Profile m_selectedProfile;
};