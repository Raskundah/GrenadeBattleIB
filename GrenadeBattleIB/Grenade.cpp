#include "Grenade.h"



Grenade::Grenade(int _whichPlayer)
	: Physics()
	, whichPlayer(_whichPlayer)
{

}



void Grenade::FireGrenade(sf::Vector2f fireVel)
{
	m_velocity = fireVel;
}

void Grenade::UpdatePosition(sf::Time _frameTime)
{
	Physics::Update(_frameTime)
}
