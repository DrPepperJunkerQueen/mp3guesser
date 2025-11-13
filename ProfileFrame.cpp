#include "ProfileFrame.h"
#include "Definitions.h"

ProfileFrame::ProfileFrame()
    : wxFrame(NULL, wxID_ANY, "MP3 Guesser - Profile Selection")
{
    // --- KROK 1: Wczytaj obrazek t³a ---
    // Obrazek "tlo.png" musi znajdowaæ siê w tym samym folderze co plik .exe
    // (czyli np. w Twoim folderze x64/Debug)
    wxImage::AddHandler(new wxPNGHandler); // Mówimy, ¿e bêdziemy ³adowaæ PNG
    if (!m_backgroundBitmap.LoadFile(BACKGROUND_FILEPATH, wxBITMAP_TYPE_PNG))
    {
        // Jeœli siê nie uda, wypisz b³¹d (zobaczysz go w oknie "Output" w VS)
        wxLogError("Can't load background file.");
    }


    // --- KROK 2: Zmodyfikuj tworzenie panelu ---

    // U¿ywamy teraz zmiennej m_panel z pliku .h
    m_panel = new wxPanel(this, wxID_ANY);

    // Mówimy panelowi, ¿e bêdziemy go malowaæ rêcznie
    m_panel->SetBackgroundStyle(wxBG_STYLE_PAINT);


    // --- KROK 3: Reszta kodu (przyciski i sizer) ---
    // Wa¿ne: Zmieñ "panel->" na "m_panel->"

    // --- Tworzenie przycisków ---
    wxButton* createBtn = new wxButton(m_panel, wxID_ANY, "Create Profile");
    wxButton* selectBtn = new wxButton(m_panel, wxID_ANY, "Select Profile");

    // --- Ustawianie KONKRETNEGO rozmiaru ---
    wxSize buttonSize = wxSize(250, 60); // Twój wybrany rozmiar
    createBtn->SetMinSize(buttonSize);
    selectBtn->SetMinSize(buttonSize);

    // --- Tworzenie sizera ---
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // 1. Dodaj "pusty" rozci¹gliwy odstêp POWY¯EJ przycisków
    //    Zajmie on ca³¹ woln¹ przestrzeñ na górze
    sizer->AddStretchSpacer(1);

    // 2. Dodaj przyciski (z proporcj¹ 0 i centrowaniem w poziomie)
    sizer->Add(createBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    sizer->Add(selectBtn, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    // 3. Dodaj drugi "pusty" odstêp PONI¯EJ przycisków
    //    Zajmie on ca³¹ woln¹ przestrzeñ na dole
    sizer->AddStretchSpacer(1);

    // --- Ustawienie sizera dla panelu ---
    m_panel->SetSizer(sizer);

    this->SetClientSize(1280, 720);
    this->Center();

    // --- KROK 4: Podepnij zdarzenie rysowania ---
    // Mówimy: "Gdy system bêdzie chcia³ narysowaæ m_panel,
    // zamiast tego wywo³aj funkcjê ProfileFrame::OnPaint"
    m_panel->Bind(wxEVT_PAINT, &ProfileFrame::OnPaint, this);
}

// --- KROK 5: Dodaj now¹ funkcjê OnPaint ---

void ProfileFrame::OnPaint(wxPaintEvent& event)
{
    // SprawdŸ, czy bitmapa (obrazek) zosta³a poprawnie za³adowana
    if (m_backgroundBitmap.IsOk())
    {
        // Stwórz "kontekst rysowania" dla naszego panelu
        wxPaintDC dc(m_panel);

        // Narysuj bitmapê (obrazek) w rogu panelu (0, 0)
        dc.DrawBitmap(m_backgroundBitmap, 0, 0, false);
    }
    // Wa¿ne: Nie wywo³uj event.Skip() - chcemy w pe³ni przej¹æ kontrolê nad rysowaniem
}