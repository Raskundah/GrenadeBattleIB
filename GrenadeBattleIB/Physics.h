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

	sf::FloatRect GetAABB();


	void PhysicsSelect(PhysicsType physics, sf::Time _frameTime);
	void Update(sf::Time _frameTime);

	bool CheckCollision(Physics other);
	void SetColliding(bool newColliding);

	sf::Vector2f GetCollisionDepth(Physics other);
	virtual void HandleCollision(Physics& other);

	virtual void UpdateAcceleration();


	void SetAlive(bool _alive);


	sf::Vector2f m_CollisionOffset;
	sf::Vector2f m_CollisionScale;
	CollisionType collisionType;

protected:

	PhysicsType physics;
	const float GRAVITY, ACCEL;
	sf::Vector2f m_velocity;
	sf::Vector2f m_twoFramesOldPos;
	sf::Vector2f m_acceleration;
	bool applyDrag;

private:

	sf::Vector2f GetCollsionCentre();
	float GetCircleColliderRadius();


	bool m_colliding;


	
	
};


