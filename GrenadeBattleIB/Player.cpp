#include "Player.h"
#include "AssetManager.h"
#include "Physics.h"
#include "Grenade.h";

#include "LevelScreen.h"

Player::Player(int playerNumber, LevelScreen* level)
	: Physics()
	, m_playerNumber(playerNumber)
	, pips()
	, level(level)
	, shootCooldown(sf::seconds(2.5f))
	
{
	for (int i = 0; i < 11; ++i)
	{
		pips.push_back(sf::Sprite());
		pips[i].setTexture((AssetManager::RequestTexture("Assets/pip.png")));
	
	}

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
	Physics::Update(_frameTime);

	physics = PhysicsType::FORWARD_EULER;

	 pipVelocity = sf::Vector2f(sf::Joystick::getAxisPosition(m_playerNumber, sf::Joystick::U) *10, sf::Joystick::getAxisPosition(m_playerNumber, sf::Joystick::V) *10 );

	 

	PhysicsSelect(physics, _frameTime);

	if (sf::Joystick::isButtonPressed(m_playerNumber, 0) )
	{
		Shoot(m_playerNumber);
	}

}

void Player::Draw(sf::RenderTarget& _target)
{
	Physics::Draw(_target);

	float faketime = 0;

	for (int i = 0; i < pips.size(); ++i)
	{
		pips[i].setPosition(GetPipPosition(faketime));
		_target.draw(pips[i]);
		faketime += 0.05f;
	}
}

void Player::HandleCollision(Physics& other)
{
	Physics::HandleCollision(other);
}

sf::Vector2f Player::GetPipPosition(float fakeTime) // This function is used to handle the prediction of grenade paths
{	
	sf::Vector2f gravity(0, GRAVITY);

		sf::Vector2f pipPos(gravity * fakeTime * fakeTime * 0.5f + pipVelocity * fakeTime + GetPosition());

		return pipPos;
}

void Player::Shoot(int playerNum)
{
	m_playerNumber = playerNum;

	if (m_playerNumber && shootCooldownTimer.getElapsedTime() >= shootCooldown)
	{
		level->ShootGrenade(m_position, m_velocity , 1);
		shootCooldownTimer.restart();
	}

	if (!m_playerNumber && shootCooldowntimerTwo.getElapsedTime() >= shootCooldown)
	{
		level->ShootGrenade(m_position, m_velocity , 0);
		shootCooldowntimerTwo.restart();
	}
}