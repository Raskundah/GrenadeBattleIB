#pragma once
#include "Physics.h"
class Grenade :
    public Physics
{
public:

    Grenade(int _whichPlayer);
    void FireGrenade(sf::Vector2f fireVel);
    void UpdatePosition(sf::Time _frameTime);
    
private:
    int whichPlayer;

};

