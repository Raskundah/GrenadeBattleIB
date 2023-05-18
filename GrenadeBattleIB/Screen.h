#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Screen
{

public:

	Screen(Game* newGamePointer);

	virtual void Update(sf::Time frameTime);
	virtual void Draw(sf::RenderTarget& target);

protected:

	Game* gamePointer;

};

