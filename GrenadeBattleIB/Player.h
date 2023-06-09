#pragma once
#include "SpriteObject.h"
#include "Physics.h"

class LevelScreen;

class Player :
    public Physics
{
public:
    Player(int playerNumber, LevelScreen* level);

    void Update(sf::Time _frameTime) override;
    void Draw(sf::RenderTarget& _target) override;

    void HandleCollision(Physics& other) override;

    sf::Vector2f GetPipPosition(float fakeTime);

    void UpdateAcceleration() override;

    int GetLives();
    void SetLives(int _lives);
    void ResetLives();
    int GetPlayerID();

    void Shoot();

protected:

    sf::Vector2f pipVelocity;

    int m_playerNumber;

    private:

        bool canJump;

        int lifes;

        LevelScreen* level;

        std::vector<sf::Sprite> pips;

        sf::Time shootCooldown;
        sf::Clock shootCooldownTimer;
        sf::Clock shootCooldowntimerTwo;
};