#pragma once
#include "SpriteObject.h"
class Player :
    public SpriteObject
{
public:
    Player(int playerNumber);


    void Update(sf::Time _frameTime) override;

    void HandleCollision(SpriteObject& other) override;
    
protected:
    int m_playerNumber;

private:

    void UpdateAcceleration();

    sf::Vector2f m_twoFramesOldPos;
    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;
};

