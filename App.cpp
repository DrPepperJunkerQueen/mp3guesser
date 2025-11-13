#include <wx/image.h>
#include "App.h"
#include "ProfileFrame.h"

// To makro zastêpuje funkcjê main() i uruchamia nasz¹ klasê App
wxIMPLEMENT_APP(App);

// Implementacja funkcji OnInit
bool App::OnInit()
{
    wxInitAllImageHandlers();

    // 1. Stwórz now¹ instancjê naszego okna profilu
    ProfileFrame* frame = new ProfileFrame();

    // 2. Poka¿ je
    frame->Show(true);

    // 3. Zwróæ true, aby uruchomiæ pêtlê zdarzeñ
    return true;
}