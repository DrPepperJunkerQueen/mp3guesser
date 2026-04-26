# mp3guesser

Gra muzyczna typu "zgadnij melodię", stworzona w C++ przy użyciu nowoczesnych bibliotek multimedialnych i interfejsu graficznego.

## 🎵 O projekcie

**mp3guesser** to aplikacja desktopowa, która pozwala użytkownikom testować swoją znajomość biblioteki muzycznej. Program odtwarza fragmenty utworów MP3, a zadaniem gracza jest poprawne odgadnięcie tytułu lub wykonawcy.

### Główne cechy:
- **System profili:** Tworzenie i wybór profilu użytkownika.
- **Statystyki:** Śledzenie najwyższego wyniku (highscore) oraz sumy zdobytych punktów.
- **Obsługa metadanych:** Wykorzystanie TagLib do poprawnego odczytu informacji o utworach.
- **Wysokiej jakości dźwięk:** Silnik audio oparty na bibliotece SFML.
- **Natywny interfejs:** GUI zbudowane za pomocą wxWidgets.

## 🛠️ Technologie

- **Język:** C++17 (lub nowszy)
- **GUI:** [wxWidgets](https://www.wxwidgets.org/)
- **Audio:** [SFML](https://www.sfml-dev.org/)
- **Tagi:** [TagLib](https://taglib.org/)
- **Zarządzanie pakietami:** [vcpkg](https://vcpkg.io/) (tryb manifestu)

## 🚀 Instalacja i kompilacja

Projekt korzysta z `vcpkg` w trybie manifestu, co oznacza, że wszystkie zależności są pobierane i kompilowane automatycznie przy pierwszym otwarciu w Visual Studio.

### Wymagania:
- Windows 10/11
- Visual Studio 2022 (z zainstalowanym obciążeniem "Programowanie aplikacji klasycznych w języku C++")
- Zainstalowany `vcpkg`

### Kroki:
1. Sklonuj repozytorium:
   ```bash
   git clone [https://github.com/twoj-uzytkownik/mp3guesser.git](https://github.com/twoj-uzytkownik/mp3guesser.git)
   cd mp3guesser
   ```
2. Upewnij się, że plik `vcpkg.json` znajduje się w głównym folderze projektu.
3. Otwórz plik `mp3guesser.sln` w Visual Studio 2022.
4. Visual Studio powinno automatycznie wykryć plik manifestu i rozpocząć pobieranie bibliotek (może to potrwać kilka-kilkanaście minut przy pierwszym uruchomieniu).
5. Upewnij się, że w ustawieniach projektu wybrany jest standard **C++17** lub **C++20** oraz podsystem **Windows (/SUBSYSTEM:WINDOWS)**.
6. Skompiluj projekt (F7) i uruchom go.

## 📁 Struktura plików

- `App.h / App.cpp`: Punkt wejścia aplikacji i inicjalizacja wxWidgets.
- `ProfileFrame.h / ProfileFrame.cpp`: Główne okno wyboru i tworzenia profilu.
- `Profile.h / Profile.cpp`: Logika zarządzania danymi użytkownika (wyniki, nazwa).
- `vcpkg.json`: Lista zależności projektu.

## 📝 Licencja

Projekt stworzony w celach edukacyjnych.
