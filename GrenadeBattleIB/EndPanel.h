#pragma once
#include <SFML/Graphics.hpp>


class EndPanel
{

public:

	EndPanel(sf::RenderWindow* newWindow);

	void Update(sf::Time frameTime);
	void Draw(sf::RenderTarget& target);

	void SetPosition(sf::Vector2f newPosition);

	void StartAnimation();
	void ResetPosition();

	void SetString(std::string winText);

private:

	sf::Sprite background;
	sf::Text title;
	sf::Text message;
	sf::Vector2f position;
	sf::RenderWindow* window;

	bool animatingIn;
	sf::Clock animationClock;

};
