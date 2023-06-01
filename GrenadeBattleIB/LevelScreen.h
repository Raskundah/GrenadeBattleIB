#pragma once
#include "Screen.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "EndPanel.h"
#include "Physics.h"
#include "Grenade.h"

class Game;
class Platform;

class LevelScreen :
    public Screen
{

public:
    LevelScreen(Game* newGamePointer);

    void Update(sf::Time frameTime) override;
    void Draw(sf::RenderTarget& target) override;

    void TriggerEndState(bool _win);
    void ShootGrenade(sf::Vector2f position, sf::Vector2f velocity, int playerID);


private:

    Player playerOne;
    Player playerTwo;

    EndPanel endPanel;
    bool gameRunning;

    std::vector<Platform*> platforms;
    std::vector<Grenade> grenades;
    
};

