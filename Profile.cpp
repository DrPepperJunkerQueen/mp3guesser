#include "Profile.h"
#include <algorithm>

Profile::Profile(const std::string& name)
    : _name(name),
    _highScore(0),
    _totalPoints(0)
{
    
}
void Profile::addToTotal(int newScore)
{
    _totalPoints += newScore;
    if (newScore > _highScore)
    {
        _highScore = newScore;
    }
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

void Profile::setName(string name)
{
    _name = name;
}
void Profile::setHighScore(int highScore)
{
    _highScore = highScore;
}
