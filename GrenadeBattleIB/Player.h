#pragma once
#include "SpriteObject.h"
#include "Physics.h"
#include "LevelScreen.h"

class Level;

class Player :
    public Physics
{
public:
    Player(int playerNumber);

    void Update(sf::Time _frameTime) override;
    void Draw(sf::RenderTarget& _target) override;

    void HandleCollision(Physics& other) override;

    sf::Vector2f GetPipPosition(float fakeTime);

    void Shoot(int playerNum);

protected:

    sf::Vector2f pipVelocity;

    int m_playerNumber;

    private:

        LevelScreen* level;

        std::vector<sf::Sprite> pips;

        sf::Time shootCooldown;
        sf::Clock shootCooldownTimer;
        sf::Clock shootCooldowntimerTwo;
        

        
};