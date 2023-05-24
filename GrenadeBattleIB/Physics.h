#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteObject.h"


enum class CollisionType
{
	CIRCLE,
	AABB
};

enum class PhysicsType {
	FORWARD_EULER,
	BACKWARD_EULER,
	SYMPLECTIC_EULER,
	INCORRECT_VERLET,
	VELOCITY_VERLET
};


class Physics :
	public SpriteObject

{
public:
	Physics();

	void PhysicsSelect(PhysicsType physics);

	bool CheckCollision(Physics other);
	void SetColliding(bool newColliding);

	sf::Vector2f GetCollisionDepth(Physics other);
	virtual void HandleCollision(Physics& other);

	void SetAlive(bool _alive);


	sf::Vector2f m_CollisionOffset;
	sf::Vector2f m_CollisionScale;
	CollisionType collisionType;

protected:

	PhysicsType physics;

private:

	sf::Vector2f GetCollsionCentre();
	float GetCircleColliderRadius();
	sf::FloatRect GetAABB();

	void UpdateAcceleration();

	bool m_colliding;




	sf::Vector2f m_twoFramesOldPos;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;

	sf::Time _frameTime;
	sf::Clock _clock;
};


