// Plik: App.cpp
#include "App.h"
#include "ProfileFrame.h"
#include "Definitions.h"
#include <wx/image.h>
#include <wx/busyinfo.h>
#include <wx/msgdlg.h>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxInitAllImageHandlers();

    // --- NOWA LOGIKA £ADOWANIA ---

    bool cacheLoaded = false;
    { // Blok dla wxBusyInfo
        wxBusyInfo info("Loading song library from cache...");
        wxYield();

        // 1. Spróbuj wczytaæ z cache
        cacheLoaded = m_songLibrary.LoadCache();
    } // wxBusyInfo znika

    // 2. Jeœli cache nie istnieje lub jest pusty...
    if (!cacheLoaded || m_songLibrary.GetSongCount() == 0)
    {
        // 3. Poka¿ okno z informacj¹ (zgodnie z Twoj¹ proœb¹)
        wxMessageBox(
            "Select music library folder in settings and rescan music library",
            "Song Library Empty",
            wxOK | wxICON_INFORMATION
        );
        // NIE WYCHODZIMY Z PROGRAMU.
        // Kontynuujemy, aby u¿ytkownik móg³ klikn¹æ "Rescan Library".
    }

    // 4. Stwórz ProfileFrame (z pe³n¹ LUB pust¹ bibliotek¹)
    ProfileFrame* frame = new ProfileFrame(m_songLibrary);

    // 5. Poka¿ okno
    frame->Show(true);

    // 6. Zawsze zwracaj 'true', aby aplikacja siê uruchomi³a
    return true;
}