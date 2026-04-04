#pragma once
#include <vector>
#include <string>
#include "Song.h"

class SongLibrary
{
public:
    SongLibrary();

    // Skanuje dysk, filtruje I ZAPISUJE do cache
    void ScanAndSave(const std::string& folderPath);
    // Wczytuje piosenki z cache. Zwraca 'false', jeœli cache nie istnieje.
    bool LoadCache();

    int GetSongCount() const;
    Song GetRandomSong() const;

private:
    std::vector<Song> m_songs;

    // --- NOWE FUNKCJE POMOCNICZE ---

    // Oczyszcza tytu³ z zawartoœci nawiasów () i []
    std::string ParseTitle(const std::string& rawTitle) const;

    // Parsuje string artysty na artystów g³ównych i goœcinnych
    void ParseArtists(const std::string& rawArtist,
        std::vector<std::string>& outMainArtists,
        std::vector<std::string>& outFeatures) const;

    // Sprawdza, czy tytu³ (ju¿ oczyszczony) zawiera "skit"
    bool ContainsSkit(const std::string& cleanedTitle) const;
};