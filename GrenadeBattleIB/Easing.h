#pragma once
#include <SFML/Graphics.hpp>

namespace Easing {

	sf::Vector2f Linear(sf::Vector2f begin, sf::Vector2f change, float duration, float time);
	sf::Vector2f QuadEaseIn(sf::Vector2f begin, sf::Vector2f change, float duration, float time);

}
