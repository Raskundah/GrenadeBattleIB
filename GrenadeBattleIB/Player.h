#pragma once
#include "SpriteObject.h"
#include "Physics.h"
class Player :
    public Physics
{
public:
    Player(int playerNumber);

    void Update(sf::Time _frameTime) override;

    void HandleCollision(Physics& other) override;




protected:

   
    int m_playerNumber;

private:

};