#include "Grenade.h"
#include "AssetManager.h"



Grenade::Grenade(int _whichPlayer)
	: Physics()
	, whichPlayer(_whichPlayer)
	
{
	m_sprite.setTexture(AssetManager::RequestTexture("Assets/grenade.png"));
	m_sprite.setScale(sf::Vector2f(2.0f, 2.0f));
}



void Grenade::FireGrenade(sf::Vector2f fireVel)
{
	m_velocity = fireVel;
}

void Grenade::UpdatePosition(sf::Time _frameTime)
{
	Physics::Update(_frameTime);
}