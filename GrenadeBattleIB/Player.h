#pragma once
#include "SpriteObject.h"
#include "Physics.h"

class Player :
    public Physics
{
public:
    Player(int playerNumber);

    void Update(sf::Time _frameTime) override;
    void Draw(sf::RenderTarget& _target) override;

    void HandleCollision(Physics& other) override;

    sf::Vector2f GetPipPosition(float fakeTime);

protected:

    sf::Vector2f pipVelocity;

    int m_playerNumber;

    private:

        std::vector<sf::Sprite> pips;
};