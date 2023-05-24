#pragma once
#include "SpriteObject.h"
#include "Physics.h"


class Platform :
    public Physics
{
public:
    Platform(sf::Vector2f newPosition);
    
};
