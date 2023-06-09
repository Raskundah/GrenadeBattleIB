#include "Player.h"
#include "AssetManager.h"
#include "Physics.h"
#include "Grenade.h";
#include "LevelScreen.h"
#include "VectorHelper.h"
#include "Platform.h"

Player::Player(int playerNumber, LevelScreen* level)
	: Physics()
	, m_playerNumber(playerNumber)
	, pips()
	, level(level)
	, shootCooldown(sf::seconds(1.0f))
	, canJump(true)
	, lifes(3)
	
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

	collisionType = CollisionType::AABB;

}
void Player::Update(sf::Time _frameTime)
{
	Physics::Update(_frameTime);
	

	//physics = PhysicsType::FORWARD_EULER;

	pipVelocity = sf::Vector2f(sf::Joystick::getAxisPosition(m_playerNumber, sf::Joystick::U) *20, sf::Joystick::getAxisPosition(m_playerNumber, sf::Joystick::V) *20); 

	//pipVelocity = VectorHelper::Normalise(pipVelocity);

	//PhysicsSelect(physics, _frameTime);

	if ( abs(sf::Joystick::getAxisPosition(m_playerNumber, sf::Joystick::Z) < -30 ))
	{
		Shoot();
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
	

	if (dynamic_cast<Platform*>(&other) != nullptr)
	{
		sf::Vector2f depth = GetCollisionDepth(other);

		sf::Vector2f newPos = GetPosition();

		if (abs(depth.x) < abs(depth.y))
		{
			// move in x direction
			newPos.x += depth.x * 1.5f;

			m_velocity.x = 0;
			m_acceleration.x = 0;
		}

		else
		{
			//move in y
			newPos.y += depth.y * 1.5f;

			m_velocity.y = 0;
			m_acceleration.y = 0;


			/*if collided from above
			if (depth.y < 0)
			{
				m_velocity.y = -24;
			}

			*/
		}
		SetPosition(newPos);

		canJump = true;
	}


	if (dynamic_cast<Grenade*>(&other) != nullptr && dynamic_cast<Grenade*>(&other)->GetPlayerID() != m_playerNumber)
	{		
		lifes -= 1;
	}
}

sf::Vector2f Player::GetPipPosition(float fakeTime) // This function is used to handle the prediction of grenade paths
{	
	sf::Vector2f gravity(0, GRAVITY);

		sf::Vector2f pipPos(gravity * fakeTime * fakeTime * 0.5f + pipVelocity * fakeTime + GetPosition());
		
		//pipPos = VectorHelper::Normalise(pipPos);

		return pipPos;
}

void Player::UpdateAcceleration()
{
	Physics::UpdateAcceleration();
	
	if (sf::Joystick::isConnected(m_playerNumber)) // handles joystick input.
	{

		float axisX = sf::Joystick::getAxisPosition(m_playerNumber, sf::Joystick::X);
		//float axisY = sf::Joystick::getAxisPosition(playerOneController, sf::Joystick::Y);

		float deadzone = 35;

		if (abs(axisX) > deadzone)
			m_acceleration.x = ACCEL * axisX / 100.0f;
		/*if (abs(axisY) > deadzone)
			m_acceleration.y = axisY / 100.0f;*/
	}

	if(sf::Joystick::isButtonPressed(m_playerNumber, 0) && canJump)
	{
		m_velocity.y -= GRAVITY * 0.25f;
		canJump = false;
	}
}

int Player::GetLives()
{
	return lifes;
}

void Player::SetLives(int _lives)
{
	lifes += +_lives;
}

void Player::ResetLives()
{
	lifes = 3;
}

int Player::GetPlayerID()
{
	return m_playerNumber;
}

void Player::Shoot()
{
	if (shootCooldownTimer.getElapsedTime() >= shootCooldown)
	{
		level->ShootGrenade(GetPosition(), pipVelocity , m_playerNumber);
		shootCooldownTimer.restart();		
	}
}