#include "Platform.h"
#include "AssetManager.h"

Platform::Platform(sf::Vector2f newPosition)
	: SpriteObject()
{
	m_sprite.setTexture(AssetManager::RequestTexture("Assets/tile.png"));

	m_CollisionOffset = sf::Vector2f(0, 0);
	m_CollisionScale = sf::Vector2f(1.0f, 1.0f);

	SetPosition(newPosition);
}

