#include "Physics.h"
#include "VectorHelper.h"
#include"SpriteObject.h"



Physics::Physics()
	: SpriteObject()
	, physics(PhysicsType::FORWARD_EULER)
	, m_colliding(false)
	, m_CollisionScale(1, 1)
	, m_CollisionOffset(0, 0)
	, collisionType(CollisionType::AABB)
	, m_twoFramesOldPos(GetPosition())
	, m_velocity()
	, m_acceleration(0, 0)
	, GRAVITY(1000)
{

}

void Physics::PhysicsSelect(PhysicsType physics, sf::Time _frameTime)
{
	const float DRAG_MULT = 10.0f;

	switch (physics)
	{
	case PhysicsType::FORWARD_EULER:
	{
		SetPosition(GetPosition() + m_velocity * _frameTime.asSeconds());

		m_velocity = m_velocity + m_acceleration * _frameTime.asSeconds();

		m_velocity.x = m_velocity.x - m_velocity.x * DRAG_MULT * _frameTime.asSeconds();


		UpdateAcceleration();
	}
	break;

	case PhysicsType::BACKWARD_EULER:
	{
		UpdateAcceleration();

		m_velocity = m_velocity + m_acceleration * _frameTime.asSeconds();

		m_velocity.x = m_velocity.x - m_velocity.x * DRAG_MULT * _frameTime.asSeconds();

		SetPosition(GetPosition() + m_velocity * _frameTime.asSeconds());

	}
	break;

	case PhysicsType::SYMPLECTIC_EULER:
	{
		m_velocity = m_velocity + m_acceleration * _frameTime.asSeconds();

		m_velocity.x = m_velocity.x - m_velocity.x * DRAG_MULT * _frameTime.asSeconds();


		SetPosition(GetPosition() + m_velocity * _frameTime.asSeconds());


		UpdateAcceleration();

	}
	break;

	case PhysicsType::INCORRECT_VERLET: //position verlet
	{
		UpdateAcceleration();

		sf::Vector2f lastFramePos = GetPosition();

		//current frame pos
		GetPosition() = 2.0f * lastFramePos - m_twoFramesOldPos + m_acceleration * _frameTime.asSeconds() * _frameTime.asSeconds();

		//two frames ago (for next frame)

		m_twoFramesOldPos = lastFramePos;
	}
	break;

	case PhysicsType::VELOCITY_VERLET:
	{

		//get half frame velocity using

		//previous frame's acceleration 
		sf::Vector2f halfFrameVel = m_velocity + m_acceleration * _frameTime.asSeconds() / 2.0f;

		//get new frame position using hjalf frame velocity
		SetPosition(GetPosition() + halfFrameVel * _frameTime.asSeconds());


		//update acceleration
		UpdateAcceleration();


		//get new frames velocity using half frame velocity and updated acceleration
		m_velocity = halfFrameVel + m_acceleration * _frameTime.asSeconds();

		m_velocity = m_velocity - m_velocity * DRAG_MULT * _frameTime.asSeconds();

	}
	break;
	default:
		break;
	}

	//m_sprite.setPosition(GetPosition());
}

bool Physics::CheckCollision(Physics other)
{
	if (!m_isAlive || !other.m_isAlive)
	{
		return false;
	}

	switch (collisionType)
	{
	case CollisionType::CIRCLE:
	{
		if (other.collisionType == CollisionType::CIRCLE)
		{
			//get the cector representing the discplacement between two circles
			sf::Vector2f displacement = GetCollsionCentre() - other.GetCollsionCentre();

			//get the magnitude of that vector which is how far apart the circle centres
			float squareDistance = VectorHelper::SquareMagnitude(displacement);
			//compare that to the combinded radii of the two circles

			float combinedRadii = GetCircleColliderRadius() + other.GetCircleColliderRadius();

			return squareDistance <= combinedRadii * combinedRadii;
		}

		else
		{

			//handles circle colliding with AABB
			sf::Vector2f nearestPointToCircle = GetCollsionCentre(); //actual circle centre to begin

			sf::FloatRect otherAABB = other.GetAABB();

			//clamp circle centre to min max of AABB
			nearestPointToCircle.x = fmaxf(otherAABB.left, fminf(nearestPointToCircle.x, otherAABB.left + otherAABB.width));
			nearestPointToCircle.y = fmaxf(otherAABB.top, fminf(nearestPointToCircle.y, otherAABB.top + otherAABB.height));

			sf::Vector2f displacement = nearestPointToCircle - GetCollsionCentre();

			float squareDistance = VectorHelper::SquareMagnitude(displacement);
			float circRadius = GetCircleColliderRadius();

			return squareDistance <= circRadius * circRadius;

		}

	}
	break;
	case CollisionType::AABB:
	{
		if (other.collisionType == CollisionType::AABB)

			return GetAABB().intersects(other.GetAABB());

		else
		{
			//handles rectangle (AABB) colliding with a circle.


			sf::Vector2f nearestPointToCircle = other.GetCollsionCentre(); //actual circle centre to begin

			sf::FloatRect thisAABB = GetAABB();

			//clamp circle centre to min max of AABB
			nearestPointToCircle.x = fmaxf(thisAABB.left, fminf(nearestPointToCircle.x, thisAABB.left + thisAABB.width));
			nearestPointToCircle.y = fmaxf(thisAABB.top, fminf(nearestPointToCircle.y, thisAABB.top + thisAABB.height));

			sf::Vector2f displacement = nearestPointToCircle - other.GetCollsionCentre();

			float squareDistance = VectorHelper::SquareMagnitude(displacement);
			float circRadius = other.GetCircleColliderRadius();

			return squareDistance <= circRadius * circRadius;
		}
	}
	break;
	default:
		return false;
		break;
	}

}

void Physics::SetColliding(bool newColliding) //boolean logic for setting if an object is colliding to trigger behaviour.
{
	m_colliding = newColliding;

}

sf::Vector2f Physics::GetCollisionDepth(Physics other) //calculates how much a collision box or circle has overlapped with another collider.
{
	sf::FloatRect thisAABB = GetAABB();
	sf::FloatRect otheAABB = other.GetAABB();

	sf::Vector2f thisCentre = GetCollsionCentre();
	sf::Vector2f otherCentre = other.GetCollsionCentre();

	sf::Vector2f minDistance;
	minDistance.x = thisAABB.width * 0.5f + otheAABB.width * 0.5f;
	minDistance.y = thisAABB.height * 0.5f + otheAABB.height * 0.5f;

	sf::Vector2f actualDistance = otherCentre - thisCentre;

	if (actualDistance.x < 0)
		minDistance.x = -minDistance.x;
	if (actualDistance.y < 0)
		minDistance.y = -minDistance.y;


	return actualDistance - minDistance;
}

void Physics::HandleCollision(Physics& other)
{
	// const float JUMPSPEED = 100;

	sf::Vector2f depth = GetCollisionDepth(other);

	sf::Vector2f newPos = GetPosition();

	if (abs(depth.x) < abs(depth.y))
	{
		// move in x direction
		newPos.x += depth.x*1.1f;

		m_velocity.x = 0;
		m_acceleration.x = 0;
	}

	else
	{
		//move in y
		newPos.y += depth.y*1.1f;

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
}





void Physics::SetAlive(bool _alive) //this allows a physics element to present whether it is alive or dead for collision and rendering purposes.
{
	m_isAlive = _alive;

}

sf::Vector2f Physics::GetCollsionCentre() // this function is designed to get a circle collider's centre and place that centre position at the centre of the sprite.
{

	sf::Vector2f centre = m_position;

	sf::FloatRect bounds = m_sprite.getGlobalBounds();
	centre.x += bounds.width * 0.5f;
	centre.y += bounds.height * 0.5f;

	centre.x += m_CollisionOffset.x;
	centre.y += m_CollisionOffset.y;

	return centre;
}

float Physics::GetCircleColliderRadius() //calculates the radius of a cirlce collision.
{
	sf::FloatRect bounds = m_sprite.getGlobalBounds();

	bounds.width = bounds.width * m_CollisionScale.x;
	bounds.height = bounds.height * m_CollisionScale.y;


	if (bounds.width > bounds.height)
	{
		return bounds.width * 0.5f;
	}
	else
		return bounds.height * 0.5f;
}

sf::FloatRect Physics::GetAABB()
{
	//this establishes the bounds of the object requiring a collision box, then multiplies it by the given scalar on a per case basisd.
	// it also centres the box by getting the centre and aligning the left and top to that centre. scalar vector is found when the non abstract object is called.

	// the reason we take the left bound and the top bound and multiply them by 0.5 and subtract the heigh and width is this gives us the centre of the sprite.

	sf::FloatRect bounds = m_sprite.getGlobalBounds();

	bounds.width = bounds.width * m_CollisionScale.x;
	bounds.height = bounds.height * m_CollisionScale.y;

	sf::Vector2f centre = GetCollsionCentre();

	bounds.left = centre.x - bounds.width * 0.5f;
	bounds.top = centre.y - bounds.height * 0.5f;


	return bounds;
}

void Physics::UpdateAcceleration() //used to update the object's acceleration as part of movement code.
{
	const float ACCEL = 10000;
	m_acceleration.x = 0;
	//m_acceleration.y = GRAVITY;

	int playerOneController = 0;
	int playerTwoController = 1;

	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_acceleration.x = -ACCEL;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_acceleration.x = ACCEL;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_acceleration.y = -ACCEL;
	}
	*/
	if (sf::Joystick::isConnected(playerOneController)) // handles joystick input.
	{
		

		float axisX = sf::Joystick::getAxisPosition(playerOneController, sf::Joystick::X);
		//float axisY = sf::Joystick::getAxisPosition(playerOneController, sf::Joystick::Y);

		float deadzone = 25;

		if (abs(axisX) > deadzone)
			m_acceleration.x = ACCEL*axisX / 100.0f;
		/*if (abs(axisY) > deadzone)
			m_acceleration.y = axisY / 100.0f;*/
	}
}




