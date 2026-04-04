#pragma once
#include "BackgroundFrame.h"
#include "Profile.h"
#include "SongLibrary.h"
#include <SFML/Audio.hpp> // <-- Wa¿ne!
#include <wx/timer.h>     // Do liczenia czasu

class PlaySessionFrame : public BackgroundFrame
{
public:
    PlaySessionFrame(Profile userProfile, SongLibrary& songLib, int gameMode);
    ~PlaySessionFrame(); // Destruktor do sprz¹tania

private:
    // --- DANE GRY ---
    Profile m_userProfile;
    SongLibrary& m_songLibRef;
    int m_gameMode; // 20 lub -1
    int m_currentRound;
    long long m_totalScore;

    Song m_currentSong;      // Aktualnie grana piosenka
    bool m_isPlaying;        // Czy muzyka gra?
    float m_elapsedTime;     // Czas w sekundach (z dok³adnoœci¹)

    long long m_scoreBeforeRound; // Przechowuje wynik z pocz¹tku rundy

    // Flagi, co uda³o siê zgadn¹æ w tej rundzie
    bool m_guessedTitle;
    bool m_guessedMainArtist;
    bool m_guessedFeatArtist;

    // --- ELEMENTY UI ---
    wxStaticText* m_lblRound;
    wxStaticText* m_lblScore;
    wxStaticText* m_lblTimer;

    wxButton* m_btnPlayStop;

    wxTextCtrl* m_txtTitleInput;
    wxTextCtrl* m_txtArtistInput;

    wxButton* m_btnSubmit;

    // Elementy widoczne PO zgadywaniu
    wxPanel* m_pnlResults; // Panel grupuj¹cy wyniki
    wxStaticText* m_lblCorrectTitle;
    wxStaticText* m_lblCorrectArtist;
    wxButton* m_btnOverride; // Samozatwierdzanie
    wxButton* m_btnNext;

    // --- NARZÊDZIA ---
    sf::Music m_music;
    wxTimer m_timer;

    // --- FUNKCJE ---
    void StartRound();
    void EndRound(); // Wywo³ywane po zatwierdzeniu
    void UpdateTimerDisplay();
    int CalculatePoints(); // Liczy punkty za tê rundê

    // Event Handlers
    void OnPlayStop(wxCommandEvent& event);
    void OnSubmit(wxCommandEvent& event);
    void OnOverride(wxCommandEvent& event); // Samozatwierdzanie
    void OnNextRound(wxCommandEvent& event);
    void OnTimerTick(wxTimerEvent& event);
};