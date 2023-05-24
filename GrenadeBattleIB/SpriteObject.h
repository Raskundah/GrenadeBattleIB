#pragma once
#include <SFML/Graphics.hpp>



class SpriteObject
{
public:

	SpriteObject();

	virtual void Update(sf::Time _frameTime);
	virtual void Draw(sf::RenderTarget& _target);

	sf::Vector2f GetPosition();
	

	virtual void SetPosition(sf::Vector2f _newPosition);
	void SetPosition(float m_newX, float m_newY) ;


	sf::Sprite m_sprite;

protected:
	bool m_isAlive;
	sf::Vector2f m_position;
};