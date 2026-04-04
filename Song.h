// Plik: Song.h
#pragma once
#include <string>
#include <vector>

struct Song
{
    std::string filePath;
    std::string title; // Oczyszczony tytu³ (bez nawiasów)
    std::vector<std::string> artists;  // Wektor dla artystów g³ównych
    std::vector<std::string> features; // Wektor dla artystów goœcinnych
    int duration;           // Czas trwania w sekundach

    // Zaktualizowany konstruktor
    Song(std::string path, std::string t,
        std::vector<std::string> a, std::vector<std::string> f, int d)
        : filePath(path), title(t), artists(a), features(f), duration(d) {
    }
};