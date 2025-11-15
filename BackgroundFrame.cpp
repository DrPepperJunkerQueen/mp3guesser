#include "BackgroundFrame.h"

BackgroundFrame::BackgroundFrame(wxWindow* parent, wxWindowID id, const wxString& title)
    : wxFrame(parent, id, title)
{
    // Ta logika jest teraz w jednym, centralnym miejscu
    wxImage::AddHandler(new wxPNGHandler);
    if (!m_backgroundBitmap.LoadFile(BACKGROUND_FILEPATH, wxBITMAP_TYPE_PNG))
    {
        // Ten b³¹d jest teraz wspólny dla wszystkich okien
        wxLogError("Can't load background file.");
    }

    m_panel = new wxPanel(this, wxID_ANY);
    m_panel->SetBackgroundStyle(wxBG_STYLE_PAINT);

    // U¿ywamy Bind zamiast Event Table, bo jest czystsze dla klasy bazowej
    m_panel->Bind(wxEVT_PAINT, &BackgroundFrame::OnPaint, this);

    // Domyœlne ustawienia okna
    this->SetClientSize(1280, 720);
    this->Center();
}

void BackgroundFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(m_panel);
    if (m_backgroundBitmap.IsOk())
    {
        dc.DrawBitmap(m_backgroundBitmap, 0, 0, false);
    }
    event.Skip(); // Przeka¿ dalej, aby narysowaæ przyciski!
}