#include "SongLibrary.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include "Definitions.h"

// Nagłówki TagLib
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

// === FUNKCJE DO BEZPIECZNEJ KONWERSJI UNICODE ===

std::string RemoveDiacritics(const std::string& input)
{
    std::string result;
    result.reserve(input.length());

    for (size_t i = 0; i < input.length(); )
    {
        unsigned char c = static_cast<unsigned char>(input[i]);

        // Zwykły ASCII (1 bajt)
        if (c < 128) {
            // Akceptuj tylko drukowalne znaki ASCII i spację
            if (c >= 32 && c <= 126) {
                result += c;
            }
            i++;
        }
        // UTF-8: 2 bajty (większość europejskich znaków)
        else if (c >= 0xC0 && c < 0xE0 && i + 1 < input.length()) {
            unsigned char c1 = static_cast<unsigned char>(input[i]);
            unsigned char c2 = static_cast<unsigned char>(input[i + 1]);

            // Polskie znaki
            if (c1 == 0xC4 && c2 == 0x85) result += 'a';  // ą
            else if (c1 == 0xC4 && c2 == 0x84) result += 'A';  // Ą
            else if (c1 == 0xC4 && c2 == 0x87) result += 'c';  // ć
            else if (c1 == 0xC4 && c2 == 0x86) result += 'C';  // Ć
            else if (c1 == 0xC4 && c2 == 0x99) result += 'e';  // ę
            else if (c1 == 0xC4 && c2 == 0x98) result += 'E';  // Ę
            else if (c1 == 0xC5 && c2 == 0x82) result += 'l';  // ł
            else if (c1 == 0xC5 && c2 == 0x81) result += 'L';  // Ł
            else if (c1 == 0xC5 && c2 == 0x84) result += 'n';  // ń
            else if (c1 == 0xC5 && c2 == 0x83) result += 'N';  // Ń
            else if (c1 == 0xC3 && c2 == 0xB3) result += 'o';  // ó
            else if (c1 == 0xC3 && c2 == 0x93) result += 'O';  // Ó
            else if (c1 == 0xC5 && c2 == 0x9B) result += 's';  // ś
            else if (c1 == 0xC5 && c2 == 0x9A) result += 'S';  // Ś
            else if (c1 == 0xC5 && c2 == 0xBA) result += 'z';  // ź
            else if (c1 == 0xC5 && c2 == 0xB9) result += 'Z';  // Ź
            else if (c1 == 0xC5 && c2 == 0xBC) result += 'z';  // ż
            else if (c1 == 0xC5 && c2 == 0xBB) result += 'Z';  // Ż

            // Niemieckie umlauts
            else if (c1 == 0xC3 && c2 == 0xA4) result += 'a';  // ä
            else if (c1 == 0xC3 && c2 == 0x84) result += 'A';  // Ä
            else if (c1 == 0xC3 && c2 == 0xB6) result += 'o';  // ö
            else if (c1 == 0xC3 && c2 == 0x96) result += 'O';  // Ö
            else if (c1 == 0xC3 && c2 == 0xBC) result += 'u';  // ü
            else if (c1 == 0xC3 && c2 == 0x9C) result += 'U';  // Ü
            else if (c1 == 0xC3 && c2 == 0x9F) result += "ss"; // ß

            // Francuskie znaki
            else if (c1 == 0xC3 && c2 == 0xA0) result += 'a';  // à
            else if (c1 == 0xC3 && c2 == 0xA2) result += 'a';  // â
            else if (c1 == 0xC3 && c2 == 0xA1) result += 'a';  // á
            else if (c1 == 0xC3 && c2 == 0xA8) result += 'e';  // è
            else if (c1 == 0xC3 && c2 == 0xA9) result += 'e';  // é
            else if (c1 == 0xC3 && c2 == 0xAA) result += 'e';  // ê
            else if (c1 == 0xC3 && c2 == 0xAB) result += 'e';  // ë
            else if (c1 == 0xC3 && c2 == 0xAE) result += 'i';  // î
            else if (c1 == 0xC3 && c2 == 0xAF) result += 'i';  // ï
            else if (c1 == 0xC3 && c2 == 0xB4) result += 'o';  // ô
            else if (c1 == 0xC3 && c2 == 0xB9) result += 'u';  // ù
            else if (c1 == 0xC3 && c2 == 0xBB) result += 'u';  // û
            else if (c1 == 0xC3 && c2 == 0xA7) result += 'c';  // ç
            else if (c1 == 0xC3 && c2 == 0x87) result += 'C';  // Ç

            // Hiszpańskie
            else if (c1 == 0xC3 && c2 == 0xB1) result += 'n';  // ñ
            else if (c1 == 0xC3 && c2 == 0x91) result += 'N';  // Ñ

            // Skandynawskie
            else if (c1 == 0xC3 && c2 == 0xA5) result += 'a';  // å
            else if (c1 == 0xC3 && c2 == 0x85) result += 'A';  // Å
            else if (c1 == 0xC3 && c2 == 0xA6) result += "ae"; // æ
            else if (c1 == 0xC3 && c2 == 0x86) result += "AE"; // Æ
            else if (c1 == 0xC3 && c2 == 0xB8) result += 'o';  // ø
            else if (c1 == 0xC3 && c2 == 0x98) result += 'O';  // Ø

            // Inne
			else if (c1 == 0xC2 && c2 == 0xA5) result += 'Y';  // ¥
            

            i += 2;
        }
        // UTF-8: 3 bajty lub więcej - ignoruj (emotikony itp.)
        else if (c >= 0xE0 && c < 0xF0) {
            i += 3;
        }
        else if (c >= 0xF0) {
            i += 4;
        }
        else {
            i++;
        }
    }

    return result;
}

std::string SafeTagToString(const TagLib::String& tagString)
{
    if (tagString.isEmpty()) return "";
    try {
        return tagString.toCString(true); // Próba UTF-8
    }
    catch (...) {
        return "";
    }
}

// === FUNKCJE POMOCNICZE DLA STRINGÓW ===

std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void SplitString(const std::string& s, char delimiter, std::vector<std::string>& outVector)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter))
    {
        std::string trimmedItem = Trim(item);
        if (!trimmedItem.empty()) outVector.push_back(trimmedItem);
    }
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string Join(const std::vector<std::string>& vec, const std::string& delim)
{
    std::string result = "";
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i < vec.size() - 1) result += delim;
    }
    return result;
}

// === LOGIKA BIBLIOTEKI ===

bool SongLibrary::ContainsSkit(const std::string& cleanedTitle) const
{
    return ToLower(cleanedTitle).find("skit") != std::string::npos;
}

std::string SongLibrary::ParseTitle(const std::string& rawTitle) const
{
    // ZMIANA: Ta funkcja teraz służy tylko do oczyszczania ze śmieci, 
    // ale ZACHOWUJE nawiasy, bo tak ustaliliśmy.
    // Wcześniejsza logika usuwania nawiasów została przeniesiona do PlaySessionFrame::EndRound
    return Trim(rawTitle);
}

void SongLibrary::ParseArtists(const std::string& rawArtist,
    std::vector<std::string>& outMainArtists,
    std::vector<std::string>& outFeatures) const
{
    std::string mainArtistsStr = rawArtist;
    std::string featuresStr = "";

    size_t featPos = rawArtist.find(" Feat. ");
    if (featPos != std::string::npos)
    {
        mainArtistsStr = rawArtist.substr(0, featPos);
        featuresStr = rawArtist.substr(featPos + 7);
    }

    SplitString(mainArtistsStr, '&', outMainArtists);
    featuresStr = ReplaceAll(featuresStr, " & ", ",");
    SplitString(featuresStr, ',', outFeatures);
}

SongLibrary::SongLibrary() {
    srand((unsigned int)time(NULL));
}

int SongLibrary::GetSongCount() const {
    return m_songs.size();
}

Song SongLibrary::GetRandomSong() const {
    if (m_songs.empty()) return Song("", "", {}, {}, 0);
    int randomIndex = rand() % m_songs.size();
    return m_songs[randomIndex];
}

bool SongLibrary::LoadCache()
{
    m_songs.clear();
    std::ifstream file(SONG_CACHE_FILEPATH);

    if (!file.is_open()) {
        std::cout << "Song cache not found." << std::endl;
        return false;
    }

    std::string filePath, title, durationStr, artistsStr, featuresStr, delimiter;
    int duration;
    std::vector<std::string> artists, features;

    while (std::getline(file, filePath) &&
        std::getline(file, title) &&
        std::getline(file, durationStr) &&
        std::getline(file, artistsStr) &&
        std::getline(file, featuresStr) &&
        std::getline(file, delimiter))
    {
        if (delimiter != "===") {
            m_songs.clear();
            return false;
        }
        try {
            duration = std::stoi(durationStr);
            artists.clear(); features.clear();
            SplitString(artistsStr, ',', artists);
            SplitString(featuresStr, ',', features);
            m_songs.emplace_back(filePath, title, artists, features, duration);
        }
        catch (...) {}
    }
    file.close();
    std::cout << "Loaded " << m_songs.size() << " songs from cache." << std::endl;
    return true;
}

void SongLibrary::ScanAndSave(const std::string& folderPath)
{
    m_songs.clear();
    std::cout << "Starting scan of: " << folderPath << std::endl;

    // Ignoruj foldery systemowe/bez uprawnień
    const auto scanOptions = std::filesystem::directory_options::skip_permission_denied;

    // Używamy iteratora, który nie rzuca wyjątków przy błędzie dostępu (opcjonalnie można dodać parametr ec)
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath, scanOptions))
    {
        try // Główny try dla pętli
        {
            try // Wewnętrzny try dla TagLib i konwersji stringów
            {
                if (!entry.is_regular_file()) continue;

                std::wstring wPath = entry.path().wstring();
                TagLib::FileRef f(wPath.c_str());

                if (f.isNull() || !f.audioProperties()) continue;

                int duration = f.audioProperties()->lengthInSeconds();
                std::string rawTitle = "";
                std::string rawArtist = "";

                if (f.tag()) {
                    rawTitle = SafeTagToString(f.tag()->title());
                    rawArtist = SafeTagToString(f.tag()->artist());
                }

                // --- NOWY FILTR: Odrzuć puste lub Unknown ---
                if (rawTitle.empty() || rawArtist.empty() || rawArtist == "Unknown Artist") {
                    continue;
                }

                // Przygotowanie danych (ZACHOWUJEMY NAWIASY)
                std::string finalTitle = Trim(rawTitle);

                std::vector<std::string> mainArtists;
                std::vector<std::string> features;
                ParseArtists(rawArtist, mainArtists, features);

                // Filtry gry
                if (duration < MIN_SONG_DURATION) continue;
                if (ContainsSkit(finalTitle)) continue;

                // --- ZAPISZ PIOSENKĘ ---
                // Używamy .string() (może rzucić wyjątek przy dziwnych znakach, który złapiemy niżej)
                std::string pathString = entry.path().string();

                m_songs.emplace_back(pathString, finalTitle, mainArtists, features, duration);
            }
            catch (const std::exception& e)
            {
                // Bezpieczne logowanie błędu (szerokie znaki)
                std::wcerr << L"Error processing file " << entry.path().wstring()
                    << L": " << e.what() << std::endl;
            }
            catch (...)
            {
                std::wcerr << L"Unknown error processing file: "
                    << entry.path().wstring() << std::endl;
            }
        }
        catch (...)
        {
            // Ostatnia deska ratunku, gdyby sam iterator rzucił błąd
            continue;
        }
    }
    std::cout << "Scan complete. Found " << m_songs.size() << " songs." << std::endl;

    // Zapis do cache
    std::ofstream cacheFile(SONG_CACHE_FILEPATH);
    if (cacheFile.is_open()) {
        for (const Song& song : m_songs)
        {
            cacheFile << song.filePath << "\n";
            cacheFile << song.title << "\n";
            cacheFile << song.duration << "\n";
            cacheFile << Join(song.artists, ",") << "\n";
            cacheFile << Join(song.features, ",") << "\n";
            cacheFile << "===\n";
        }
        cacheFile.close();
    }
}