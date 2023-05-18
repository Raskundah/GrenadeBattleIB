#include "Easing.h"

sf::Vector2f Easing::Linear(sf::Vector2f begin, sf::Vector2f change, float duration, float time)
{
	 return (change / duration) * time + begin;
}

sf::Vector2f Easing::QuadEaseIn(sf::Vector2f begin, sf::Vector2f change, float duration, float time)
{
	return (change / (duration * duration)) * (time * time) + begin;
}
