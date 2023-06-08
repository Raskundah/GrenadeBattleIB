#pragma once
#include "Physics.h"
class Grenade :
    public Physics
{
public:

    Grenade(int _whichPlayer);
    void FireGrenade(sf::Vector2f fireVel);
    void HandleCollision(Physics& other);
    
private:
    int whichPlayer;

};

