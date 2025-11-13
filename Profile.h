#pragma once
#include <string>
#include <iostream>

using namespace std;

class Profile
{
public:
	// Konstruktor: tworzy nowy profil z podan¹ nazw¹.
	// Wyniki domyœlnie ustawia na 0.
	Profile(const std::string& name);

    // G³ówna funkcja logiczna.
    // Dodaje wynik z ostatniej gry do sumy punktów
    // i automatycznie sprawdza, czy to nowy highscore.
    void addToTotal(int newScore);

    // --- Gettery (funkcje do odczytu danych) ---
    string getName() const;
    int getHighscore() const;
    long long getTotalPoints() const;

    // --- Settery ---
    void setName(string name);
    void setHighScore(int highScore);
private:
	string _name;
	int _highScore;
	long long _totalPoints;
};

