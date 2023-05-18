#pragma once
#include "SFML/Graphics.hpp"
class EndPanel
{
public:

	EndPanel(sf::RenderWindow* newWindow);

	virtual void Update(sf::Time _frameTime);
	virtual void Draw(sf::RenderTarget& _target);

	void SetPosition(sf::Vector2f newPosition);
	void StartAnimation();

private:
	sf::Sprite m_background;
	sf::Text m_title;
	sf::Text m_message;
	sf::Vector2f m_position;
	sf::RenderWindow* m_window;

	bool m_animatingIn;
	sf::Clock m_animationClock;
};

