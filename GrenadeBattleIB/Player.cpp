#include "Player.h"
#include "AssetManager.h"
#include "Physics.h"


Player::Player(int playerNumber)
	: Physics()
	, m_playerNumber(playerNumber)

	
{
	if (!playerNumber)
	{
		m_sprite.setTexture(AssetManager::RequestTexture("Assets/player_1_stand.png"));
	}

	if (playerNumber)
	{
		m_sprite.setTexture(AssetManager::RequestTexture("Assets/player_2_stand.png"));
	}

	m_CollisionOffset = sf::Vector2f(0, 0);
	m_CollisionScale = sf::Vector2f(1.0f, 1.0f);

	collisionType = CollisionType::CIRCLE;

	
}

void Player::Update(sf::Time _frameTime)
{
	physics = PhysicsType::FORWARD_EULER;

	PhysicsSelect(physics, _frameTime);

}

void Player::HandleCollision(Physics& other)
{
	Physics::HandleCollision(other);
}

