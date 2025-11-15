#pragma once
#include <string>
#include <iostream>
#include  <vector>

using namespace std;

class Profile
{
public:
	// Konstruktor: tworzy nowy profil z podan¹ nazw¹.
	// Wyniki domyœlnie ustawia na 0.
	Profile(const string& name);
    Profile();
    void createProfile();

    // G³ówna funkcja logiczna.
    // Dodaje wynik z ostatniej gry do sumy punktów
    // i automatycznie sprawdza, czy to nowy highscore.
    void addToTotal(int newScore);

    // --- Gettery (funkcje do odczytu danych) ---
	int getID() const;
    string getName() const;
    int getHighscore() const;
    long long getTotalPoints() const;

    // --- Settery ---
	void setID(int id);
    void setName(string name);
    void setHighScore(int highScore);
    void setTotalPoints(long long totalPoints);

	void readProfile(int id);
    static vector<Profile> readAllProfiles();
	void updateProfileStats();
private:
    int _id;
	string _name;
	int _highScore;
	long long _totalPoints;
};


