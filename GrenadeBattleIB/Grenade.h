#pragma once
#include "Physics.h"
class Grenade :
    public Physics
{
public:

    Grenade(int _whichPlayer);
    void FireGrenade(sf::Vector2f fireVel);
    void HandleCollision(Physics& other);

    bool GetmarkedForDeletion();
    void SetMarkedForDeletion(bool value);

    sf::Clock GetClock();
    void ResetClock();
    int GetPlayerID();
    

    
private:
    int whichPlayer;
    bool shouldBeDeleted;
    bool hitPlayer;
    sf::Time expiryTime;
    sf::Clock expiryClock;
};

