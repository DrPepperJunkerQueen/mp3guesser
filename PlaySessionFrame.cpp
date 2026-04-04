#include "PlaySessionFrame.h"
#include <wx/msgdlg.h>
#include <wx/choicdlg.h> // Do okienka wyboru przy samozatwierdzaniu
#include <iomanip>       // Do formatowania czasu
#include <sstream>
#include <cmath>         // Do ceil()
#include "Definitions.h"
#include "GameFrame.h"
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/statline.h>

// Funkcja pomocnicza: zmienia na małe litery i usuwa zbędne spacje
std::string NormalizeString(std::string str) {
	// KROK 1: Zamień słowa/znaki na ich odpowiedniki (PRZED normalizacją)
	// Funkcja pomocnicza do zamiany wszystkich wystąpień
	auto replaceAll = [](std::string& s, const std::string& from, const std::string& to) {
		size_t pos = 0;
		while ((pos = s.find(from, pos)) != std::string::npos) {
			s.replace(pos, from.length(), to);
			pos += to.length();
		}
		};

	// Zamień różne formy "&" na "and"
	replaceAll(str, " & ", " and ");  // " & " → " and "
	replaceAll(str, "&", "and");      // "&" → "and" (bez spacji)

	// KROK 2: Normalizacja znaków specjalnych i interpunkcji
	std::string result = "";
	for (unsigned char c : str) {
		// Mapowanie znaków specjalnych na fonetyczne odpowiedniki
		if (c == '$') {
			result += 's';
			continue;
		}
		else if (c == '@') {
			result += 'a';
			continue;
		}
		else if (c == '!' || c == '|') {
			result += 'i';
			continue;
		}
		// SPACJA - zachowujemy ją!
		else if (c == ' ') {
			result += ' ';
			continue;
		}
		// std::ispunct sprawdza czy znak jest interpunkcyjny
		// Jeśli JEST interpunkcją -> ignorujemy go (continue)
		else if (std::ispunct(c)) {
			continue;
		}
		// Jeśli NIE JEST interpunkcją -> zamieniamy na małą literę i dodajemy
		result += std::tolower(c);
	}

	// KROK 3: Trim (usuwanie spacji z początku i końca) + usuń wielokrotne spacje
	// Najpierw usuń wielokrotne spacje (zamień na pojedyncze)
	auto removeMultipleSpaces = [](std::string s) {
		std::string cleaned = "";
		bool lastWasSpace = false;
		for (char c : s) {
			if (c == ' ') {
				if (!lastWasSpace) {
					cleaned += c;
					lastWasSpace = true;
				}
			}
			else {
				cleaned += c;
				lastWasSpace = false;
			}
		}
		return cleaned;
		};

	result = removeMultipleSpaces(result);

	// Teraz Trim (usuwanie spacji z początku i końca)
	size_t first = result.find_first_not_of(" \t\n\r");
	if (std::string::npos == first) return "";
	size_t last = result.find_last_not_of(" \t\n\r");
	return result.substr(first, (last - first + 1));
}
std::string RemoveParentheses(std::string str) {
	std::string result = "";
	int ignoreLevel = 0;
	for (char c : str) {
		if (c == '(' || c == '[') ignoreLevel++;
		else if (c == ')' || c == ']') {
			if (ignoreLevel > 0) ignoreLevel--;
		}
		else if (ignoreLevel == 0) {
			result += c;
		}
	}
	return result;
}

// Funkcja usuwająca "the " z początku stringa (zakłada, że string jest już mały)
std::string RemoveLeadingThe(std::string str) {
	// Sprawdź, czy string jest dłuższy niż 4 znaki i zaczyna się od "the "
	// (musi być spacja, żeby nie ucięło np. "Them" albo "Theory")
	if (str.length() > 4 && str.substr(0, 4) == "the ") {
		return str.substr(4); // Zwróć wszystko PO "the "
	}
	return str;
}

enum {
	ID_PlayStop = 100,
	ID_Submit,
	ID_Override,
	ID_Next,
	ID_Timer
};

PlaySessionFrame::PlaySessionFrame(Profile userProfile, SongLibrary& songLib, int gameMode)
	: BackgroundFrame(NULL, wxID_ANY, "MP3 Guesser - Game On!"),
	m_userProfile(userProfile),
	m_songLibRef(songLib),
	m_gameMode(gameMode),
	m_currentRound(0), // Zaczniemy od inkrementacji do 1
	m_totalScore(0),
	m_currentSong("", "", {}, {}, 0), // Pusta piosenka na start
	m_isPlaying(false),
	m_elapsedTime(0.0f),
	m_guessedTitle(false),
	m_guessedMainArtist(false),
	m_guessedFeatArtist(false)
{
	m_scoreBeforeRound = m_totalScore;

	// --- UI: GÓRA (Info) ---
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* topInfoSizer = new wxBoxSizer(wxHORIZONTAL);
	m_lblRound = new wxStaticText(m_panel, wxID_ANY, "Round: 1 / 20");
	m_lblScore = new wxStaticText(m_panel, wxID_ANY, "Score: 0");

	// Powiększ czcionki
	wxFont bigFont = m_lblRound->GetFont();
	bigFont.MakeBold().Scale(1.5);
	m_lblRound->SetFont(bigFont);
	m_lblScore->SetFont(bigFont);

	topInfoSizer->Add(m_lblRound, 1, wxALIGN_CENTER | wxALL, 20);
	topInfoSizer->Add(m_lblScore, 1, wxALIGN_CENTER | wxALL, 20);

	mainSizer->Add(topInfoSizer, 0, wxEXPAND);

	// --- UI: ŚRODEK (Kontrola i Timer) ---
	m_lblTimer = new wxStaticText(m_panel, wxID_ANY, "00.00 s");
	m_lblTimer->SetFont(bigFont.Scale(1.5)); // Jeszcze większe

	m_btnPlayStop = new wxButton(m_panel, ID_PlayStop, "PLAY MUSIC");
	m_btnPlayStop->SetMinSize(wxSize(200, 60));

	mainSizer->Add(m_lblTimer, 0, wxALIGN_CENTER | wxTOP, 20);
	mainSizer->Add(m_btnPlayStop, 0, wxALIGN_CENTER | wxALL, 20);

	// --- UI: POLA TEKSTOWE ---
	wxFlexGridSizer* inputSizer = new wxFlexGridSizer(2, 2, 10, 10);
	inputSizer->AddGrowableCol(1, 1); // Druga kolumna się rozciąga

	inputSizer->Add(new wxStaticText(m_panel, wxID_ANY, "Song Title:"), 0, wxALIGN_CENTER_VERTICAL);
	m_txtTitleInput = new wxTextCtrl(m_panel, wxID_ANY);
	inputSizer->Add(m_txtTitleInput, 1, wxEXPAND);

	inputSizer->Add(new wxStaticText(m_panel, wxID_ANY, "Artist:"), 0, wxALIGN_CENTER_VERTICAL);
	m_txtArtistInput = new wxTextCtrl(m_panel, wxID_ANY);
	inputSizer->Add(m_txtArtistInput, 1, wxEXPAND);

	mainSizer->Add(inputSizer, 0, wxEXPAND | wxALL, 40);

	// --- UI: PRZYCISK ZATWIERDŹ ---
	m_btnSubmit = new wxButton(m_panel, ID_Submit, "GUESS!");
	m_btnSubmit->SetMinSize(wxSize(200, 50));
	m_btnSubmit->SetBackgroundColour(wxColour(100, 255, 100)); // Zielonkawy
	mainSizer->Add(m_btnSubmit, 0, wxALIGN_CENTER | wxBOTTOM, 20);

	// --- UI: WYNIKI (Ukryte na początku) ---
	m_pnlResults = new wxPanel(m_panel, wxID_ANY);
	wxBoxSizer* resultsSizer = new wxBoxSizer(wxVERTICAL);

	m_lblCorrectTitle = new wxStaticText(m_pnlResults, wxID_ANY, "Title: ???");
	m_lblCorrectArtist = new wxStaticText(m_pnlResults, wxID_ANY, "Artist: ???");

	wxFont resFont = m_lblCorrectTitle->GetFont();
	resFont.MakeBold();
	m_lblCorrectTitle->SetFont(resFont);
	m_lblCorrectTitle->SetForegroundColour(*wxBLUE);

	resultsSizer->Add(m_lblCorrectTitle, 0, wxALIGN_CENTER | wxBOTTOM, 5);
	resultsSizer->Add(m_lblCorrectArtist, 0, wxALIGN_CENTER | wxBOTTOM, 10);

	wxBoxSizer* actionBtnSizer = new wxBoxSizer(wxHORIZONTAL);
	m_btnOverride = new wxButton(m_pnlResults, ID_Override, "I was right! (Override)");
	m_btnNext = new wxButton(m_pnlResults, ID_Next, "Next Round >>");

	actionBtnSizer->Add(m_btnOverride, 0, wxRIGHT, 20);
	actionBtnSizer->Add(m_btnNext, 0);

	resultsSizer->Add(actionBtnSizer, 0, wxALIGN_CENTER);

	m_pnlResults->SetSizer(resultsSizer);
	mainSizer->Add(m_pnlResults, 0, wxEXPAND | wxALL, 10);

	// Ukryj panel wyników na start
	m_pnlResults->Hide();

	m_panel->SetSizer(mainSizer);
	m_panel->Layout();

	// --- INITIALIZATION ---
	m_timer.Bind(wxEVT_TIMER, &PlaySessionFrame::OnTimerTick, this);
	m_btnPlayStop->Bind(wxEVT_BUTTON, &PlaySessionFrame::OnPlayStop, this);
	m_btnSubmit->Bind(wxEVT_BUTTON, &PlaySessionFrame::OnSubmit, this);
	m_btnOverride->Bind(wxEVT_BUTTON, &PlaySessionFrame::OnOverride, this);
	m_btnNext->Bind(wxEVT_BUTTON, &PlaySessionFrame::OnNextRound, this);

	// Rozpocznij pierwszą rundę!
	StartRound();
}

PlaySessionFrame::~PlaySessionFrame()
{
	m_music.stop(); // Zatrzymaj muzykę przy zamknięciu
}

void PlaySessionFrame::StartRound()
{
	// 1. Logika rund
	m_currentRound++;
	if (m_gameMode != -1) // Jeśli nie infinite
	{
		m_lblRound->SetLabel("Round: " + std::to_string(m_currentRound) + " / " + std::to_string(m_gameMode));
	}
	else {
		m_lblRound->SetLabel("Round: " + std::to_string(m_currentRound) + " / ∞");
	}

	// 2. Reset zmiennych
	m_elapsedTime = 0.0f;
	m_isPlaying = false;
	m_guessedTitle = false;
	m_guessedMainArtist = false;
	m_guessedFeatArtist = false;

	// 3. Wylosuj nową piosenkę
	m_currentSong = m_songLibRef.GetRandomSong();

	// 4. Załaduj muzykę (ale nie graj jeszcze)
	if (!m_music.openFromFile(m_currentSong.filePath)) {
		wxMessageBox("Error loading song file!", "Error", wxOK | wxICON_ERROR);
		// Tutaj można by wylosować inną, ale na razie zostawmy
	}
	// Zacznij od losowego momentu (żeby nie zawsze od początku)
	// Ale upewnij się, że zostanie 20 sekund do końca
	int maxStart = m_currentSong.duration - 25;
	if (maxStart > 0) {
		float startOffset = (float)(rand() % maxStart);
		m_music.setPlayingOffset(sf::seconds(startOffset));
	}

	// 5. Reset UI
	m_lblTimer->SetLabel("00.00 s");
	m_btnPlayStop->SetLabel("PLAY MUSIC");
	m_txtTitleInput->Clear();
	m_txtTitleInput->Enable(true);
	m_txtArtistInput->Clear();
	m_txtArtistInput->Enable(true);
	m_btnSubmit->Enable(true);
	m_btnPlayStop->Enable(true);

	// Ukryj wyniki, pokaż przycisk Play
	m_pnlResults->Hide();
	m_btnPlayStop->Show();
	m_panel->Layout();
}

void PlaySessionFrame::OnPlayStop(wxCommandEvent& event)
{
	if (m_isPlaying) {
		// Zatrzymaj
		m_music.pause();
		m_timer.Stop();
		m_btnPlayStop->SetLabel("RESUME");
	}
	else {
		// Graj
		// Sprawdź limit czasu
		if (m_elapsedTime > ROUND_TIME_LIMIT + ROUND_TIME_MARGIN) return;

		m_music.play();
		m_timer.Start(10); // Aktualizuj co 10ms
		m_btnPlayStop->SetLabel("STOP");
	}
	m_isPlaying = !m_isPlaying;
}

void PlaySessionFrame::OnTimerTick(wxTimerEvent& event)
{
	// Zliczamy czas tylko gdy muzyka gra
	// (Można też użyć m_music.getPlayingOffset(), ale to czas utworu, a nie zgadywania)
	m_elapsedTime += 0.01f; // 10ms = 0.01s

	// Formatowanie wyświetlania
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << m_elapsedTime << " s";
	m_lblTimer->SetLabel(ss.str());

	// Limit 20 sekund
	if (m_elapsedTime > ROUND_TIME_LIMIT + ROUND_TIME_MARGIN) {
		m_music.pause();
		m_timer.Stop();
		m_isPlaying = false;
		m_btnPlayStop->SetLabel("TIME'S UP!");
		m_btnPlayStop->Disable();
	}
}

void PlaySessionFrame::OnSubmit(wxCommandEvent& event)
{
	EndRound();
}

void PlaySessionFrame::EndRound()
{
	// 1. Zatrzymaj wszystko
	m_music.stop();
	m_timer.Stop();
	m_isPlaying = false;

	// 2. Zablokuj wejścia
	m_txtTitleInput->Disable();
	m_txtArtistInput->Disable();
	m_btnSubmit->Disable();
	m_btnPlayStop->Disable();

	// 3. Sprawdź odpowiedzi
	// Normalizuj wejście gracza (małe litery, trim)
	std::string userTitle = NormalizeString(m_txtTitleInput->GetValue().ToStdString());
	std::string userArtist = NormalizeString(m_txtArtistInput->GetValue().ToStdString());

	// Przygotuj poprawny tytuł do porównania:
	// 1. Usuń nawiasy z oryginału
	// 2. Znormalizuj (małe litery, trim)
	std::string realTitleCleaned = RemoveParentheses(m_currentSong.title);
	std::string realTitleNormalized = NormalizeString(realTitleCleaned);

	// Sprawdź tytuł (porównujemy wersje bez nawiasów)
	if (!userTitle.empty() && userTitle == realTitleNormalized) {
		m_guessedTitle = true;
	}

	userArtist = RemoveLeadingThe(userArtist); // <-- NOWOŚĆ

	if (!userArtist.empty()) {
		// Sprawdź głównych artystów
		for (const auto& art : m_currentSong.artists) {
			// Przygotuj artystę z bazy do porównania
			std::string dbArtist = NormalizeString(art);
			dbArtist = RemoveLeadingThe(dbArtist); // <-- NOWOŚĆ

			if (dbArtist == userArtist) {
				m_guessedMainArtist = true;
				break;
			}
		}

		// Jeśli nie główny, sprawdź gościnnych (jeśli jeszcze nie zgadnięto)
		if (!m_guessedMainArtist) {
			for (const auto& feat : m_currentSong.features) {
				// To samo dla featuringów (choć rzadziej mają "The", ale warto)
				std::string dbFeat = NormalizeString(feat);
				dbFeat = RemoveLeadingThe(dbFeat); // <-- NOWOŚĆ

				if (dbFeat == userArtist) {
					m_guessedFeatArtist = true;
					break;
				}
			}
		}
	}

	// 4. Oblicz i dodaj punkty
	int points = CalculatePoints();
	m_totalScore += points;
	m_lblScore->SetLabel("Score: " + std::to_string(m_totalScore) + " (+" + std::to_string(points) + ")");

	// 5. Pokaż poprawne odpowiedzi
	m_lblCorrectTitle->SetLabel("Title: " + m_currentSong.title);

	std::string allArtists = "Artists: ";
	for (const auto& a : m_currentSong.artists) allArtists += a + ", ";
	for (const auto& f : m_currentSong.features) allArtists += f + " (feat), ";
	// Usuń ostatni przecinek
	if (allArtists.size() > 2) allArtists = allArtists.substr(0, allArtists.size() - 2);

	m_lblCorrectArtist->SetLabel(allArtists);

	// 6. Pokaż panel wyników
	m_pnlResults->Show();
	m_btnOverride->Enable(true); // Włącz przycisk korekty
	m_panel->Layout();
}

int PlaySessionFrame::CalculatePoints()
{
	int timePoints = 21 - (int)m_elapsedTime;
	if (timePoints < 1) timePoints = 1;

	// Mnożniki
	int multiplier = 0;
	if (m_guessedTitle) multiplier += TITLE_MULTIPLIER;
	if (m_guessedMainArtist) multiplier += MAIN_ARTIST_MULTIPLIER;
	if (m_guessedFeatArtist) multiplier += FEATURE_ARTIST_MULTIPLIER;

	// Jeśli nic nie zgadł, mnożnik 0 -> 0 punktów
	return timePoints * multiplier;
}

class OverrideDialog : public wxDialog
{
public:
	wxCheckBox* m_checkTitle;
	wxRadioButton* m_radioNoArtist;
	wxRadioButton* m_radioMainArtist;
	wxRadioButton* m_radioFeatArtist;

	OverrideDialog(wxWindow* parent, bool titleGuessed, bool mainGuessed, bool featGuessed)
		: wxDialog(parent, wxID_ANY, "Manual Override", wxDefaultPosition, wxDefaultSize) // wxDefaultSize pozwoli oknu dopasować się samo
	{
		// Główny sizer pionowy
		wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

		// Sizer dla zawartości (żeby zrobić ładny margines dookoła wszystkiego)
		wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

		// --- SEKCJA TYTUŁU ---
		contentSizer->Add(new wxStaticText(this, wxID_ANY, "Select what should be counted as correct:"),
			0, wxBOTTOM, 10);

		m_checkTitle = new wxCheckBox(this, wxID_ANY, "Correct Title (x3)");
		m_checkTitle->SetValue(titleGuessed);
		// wxFontWeightBold dla wyróżnienia
		wxFont boldFont = m_checkTitle->GetFont();
		boldFont.MakeBold();
		m_checkTitle->SetFont(boldFont);

		contentSizer->Add(m_checkTitle, 0, wxLEFT | wxBOTTOM, 5);

		// --- LINIA ODZIELAJĄCA ---
		// Dodajemy ją do mainSizer, żeby rozciągnęła się na całą szerokość (bez marginesów contentSizera)
		mainSizer->Add(contentSizer, 0, wxEXPAND | wxALL, 20);
		mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

		// --- SEKCJA ARTYSTY ---
		wxBoxSizer* artistSizer = new wxBoxSizer(wxVERTICAL);

		wxStaticText* artistLabel = new wxStaticText(this, wxID_ANY, "Artist Type:");
		artistSizer->Add(artistLabel, 0, wxTOP | wxBOTTOM, 10);

		// Radio Buttony
		m_radioNoArtist = new wxRadioButton(this, wxID_ANY, "Wrong Artist (x0)",
			wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
		m_radioMainArtist = new wxRadioButton(this, wxID_ANY, "Main Artist (x2)");
		m_radioFeatArtist = new wxRadioButton(this, wxID_ANY, "Feature Artist (x1)");

		// Ustawienia wartości
		if (mainGuessed) m_radioMainArtist->SetValue(true);
		else if (featGuessed) m_radioFeatArtist->SetValue(true);
		else m_radioNoArtist->SetValue(true);

		// Dodajemy z wcięciem (wxLEFT 20), żeby było ładnie pod etykietą
		artistSizer->Add(m_radioNoArtist, 0, wxLEFT | wxBOTTOM, 8);
		artistSizer->Add(m_radioMainArtist, 0, wxLEFT | wxBOTTOM, 8);
		artistSizer->Add(m_radioFeatArtist, 0, wxLEFT | wxBOTTOM, 8);

		// Dodajemy sekcję artysty do głównego sizera z marginesami
		mainSizer->Add(artistSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 20);

		// --- PRZYCISKI (OK / ANULUJ) ---
		wxSizer* btnSizer = CreateStdDialogButtonSizer(wxOK | wxCANCEL);

		// Kluczowe dla wyglądu przycisków: 
		// wxEXPAND - pozwala sizerowi zarządzać miejscem
		// wxALL, 15 - daje odstęp od dolnej krawędzi i sekcji wyżej
		mainSizer->Add(btnSizer, 0, wxEXPAND | wxALL, 15);

		// --- FINALIZACJA ---
		SetSizer(mainSizer);
		mainSizer->SetSizeHints(this); // Dopasuj minimalny rozmiar okna do zawartości
		Fit(); // Skurcz okno do rozmiaru zawartości (dzięki temu nie będzie pustego miejsca)
		CenterOnParent();
	}
};


// --- POPRAWIONA FUNKCJA OnOverride ---
void PlaySessionFrame::OnOverride(wxCommandEvent& event)
{
	// 1. Stwórz nasze niestandardowe okno
	OverrideDialog dialog(this, m_guessedTitle, m_guessedMainArtist, m_guessedFeatArtist);

	// 2. Pokaż je i czekaj na OK
	if (dialog.ShowModal() == wxID_OK)
	{
		// 3. Zaktualizuj flagi na podstawie wyboru użytkownika
		m_guessedTitle = dialog.m_checkTitle->GetValue();

		// Radio buttony same pilnują, że tylko jeden jest true
		m_guessedMainArtist = dialog.m_radioMainArtist->GetValue();
		m_guessedFeatArtist = dialog.m_radioFeatArtist->GetValue();

		// 4. PRZELICZ PUNKTY OD NOWA
		int newPoints = CalculatePoints();

		// 5. Zaktualizuj wynik całkowity
		// (Przywróć wynik z początku rundy i dodaj nowy)
		m_totalScore = m_scoreBeforeRound + newPoints;

		// 6. Zaktualizuj wyświetlanie ("Score: 123 (+45)")
		m_lblScore->SetLabel("Score: " + std::to_string(m_totalScore) +
			" (+" + std::to_string(newPoints) + ")");

		// Nie blokujemy przycisku - użytkownik może się poprawiać wielokrotnie
	}
}


void PlaySessionFrame::OnNextRound(wxCommandEvent& event)
{
	// Sprawdź czy koniec gry
	if (m_gameMode != -1 && m_currentRound >= m_gameMode)
	{
		// KONIEC GRY
		wxMessageBox("Game Over!\nFinal Score: " + std::to_string(m_totalScore),
			"Congratulations!", wxOK | wxICON_INFORMATION);

		// Zapisz wynik do profilu
		m_userProfile.addToTotal(m_totalScore);
		m_userProfile.updateProfileStats(); // Zapisz do pliku

		GameFrame* menu = new GameFrame(m_userProfile, m_songLibRef);
		menu->Show();

		// Wróć do menu
		this->Close();
		// (Tutaj przydałoby się otworzyć GameFrame, ale this->Close() po prostu zamknie okno.
		//  Można zrobić: new GameFrame(...) -> Show(); Close();)
	}
	else
	{
		StartRound();
	}
}