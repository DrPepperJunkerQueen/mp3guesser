#include "Profile.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Definitions.h"

Profile::Profile(const std::string& name)
{
	_id = 0;
	_name = name;
	_highScore = 0;
	_totalPoints = 0;
}

Profile::Profile()
{
	_id = -1;
	_name = "";
	_highScore = 0;
	_totalPoints = 0;
}

void Profile::createProfile()
{
	_highScore = 0;
	_totalPoints = 0;
	// Generowanie unikalnego ID
	ifstream iFile(PROFILE_DATA_FILEPATH);
	if (!iFile.is_open())
	{
		cerr << "Can't open profile data file." << endl;
		return;
	}

	if (iFile.tellg() != 0 || iFile.peek() != std::ifstream::traits_type::eof())
	{
		string line;
		int maxID = -1;
		while (getline(iFile, line))
		{
			stringstream stream(line);
			string drop = "";
			if (getline(stream, drop, ';'))
			{
				int id = stoi(drop);
				maxID = max(maxID, id);
			}
		}
		_id = maxID + 1;
	}
	else
	{
		_id = 0;
	}
	iFile.close();
	ofstream oFile(PROFILE_DATA_FILEPATH, ios::app);
	oFile << _id << ";" << _name << ";" << _highScore << ";" << _totalPoints << endl;
	oFile.close();
}

void Profile::addToTotal(int newScore)
{
	_totalPoints += newScore;
	if (newScore > _highScore)
	{
		_highScore = newScore;
	}
}

int Profile::getID() const
{
	return _id;
}
string Profile::getName() const
{
	return _name;
}
int Profile::getHighscore() const
{
	return _highScore;
}
long long Profile::getTotalPoints() const
{
	return _totalPoints;
}

void Profile::setID(int id)
{
	_id = id;
}
void Profile::setName(string name)
{
	_name = name;
}
void Profile::setHighScore(int highScore)
{
	_highScore = highScore;
}
void Profile::setTotalPoints(long long totalPoints)
{
	_totalPoints = totalPoints;
}

// Implementacja funkcji wczytuj¹cej profil z bazy danych lub pliku
void Profile::readProfile(int id)
{
	_id = id;
	ifstream file(PROFILE_DATA_FILEPATH);
	if (!file.is_open())
	{
		cerr << "Can't open profile data file." << endl;
		return;
	}
	string line;
	while (getline(file, line))
	{

		stringstream stream(line);
		string drop = "";
		getline(stream, drop, ';');
		if (drop == to_string(id))
		{
			for (int i = 0; getline(stream, drop, ';'); i++)
			{
				switch (i)
				{
				case 0:
					_name = drop;
					break;
				case 1:
					_highScore = stoi(drop);
					break;
				case 2:
					_totalPoints = stoll(drop);
					file.close();
					return;
				}
			}
		}
	}
}

vector<Profile> Profile::readAllProfiles()
{
	vector<Profile> profiles;
	ifstream file(PROFILE_DATA_FILEPATH);
	if (!file.is_open())
	{
		cerr << "Can't open profile data file." << endl;
		return profiles;
	}
	string line;
	while (getline(file, line))
	{
		stringstream stream(line);
		string drop = "";
		int id = 0;
		string name = "";
		int highScore = 0;
		long long totalPoints = 0;
		getline(stream, drop, ';');
		id = stoi(drop);
		getline(stream, drop, ';');
		name = drop;
		getline(stream, drop, ';');
		highScore = stoi(drop);
		getline(stream, drop, ';');
		totalPoints = stoll(drop);
		Profile profile(name);
		profile.setID(id);
		profile.setHighScore(highScore);
		profile.setTotalPoints(totalPoints);
		profiles.push_back(profile);
	}
	file.close();
	return profiles;
}

void Profile::updateProfileStats()
{
	ofstream oFile(PROFILE_DATA_FILEPATH);
	if (!oFile.is_open())
	{
		cerr << "Can't open profile data file." << endl;
		return;
	}
	vector<Profile> profiles = Profile::readAllProfiles();
	for (const Profile& profile : profiles)
	{
		if (profile.getID() == _id)
		{
			oFile << _id << ";" << _name << ";" << _highScore << ";" << _totalPoints << endl;
		}
		else
		{
			oFile << profile.getID() << ";" << profile.getName() << ";" << profile.getHighscore() << ";" << profile.getTotalPoints() << endl;
		}
	}
	oFile.close();
}