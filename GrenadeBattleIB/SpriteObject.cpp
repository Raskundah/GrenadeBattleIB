#include "SpriteObject.h"
#include "VectorHelper.h"
#include <algorithm>

SpriteObject::SpriteObject()
	: m_sprite()
	, m_position(0, 0)
	, m_colliding(false)
	, m_CollisionScale(1, 1)
	, m_CollisionOffset(0, 0)
	, collisionType(CollisionType::AABB)
	, m_isAlive(true)
{

}

void SpriteObject::Update(sf::Time _frameTime)
{
}

void SpriteObject::Draw(sf::RenderTarget& _target)
{
	if (!m_isAlive)
		return;

	_target.draw(m_sprite);

	bool drawCollider = true;
	


	if (drawCollider)
	{
		switch (collisionType)
		{
		case CollisionType::CIRCLE:
		{
			sf::CircleShape circle;

			float circleRadius = GetCircleColliderRadius();

			sf::Vector2f shapePosition = GetCollsionCentre();
			shapePosition.x -= circleRadius;
			shapePosition.y -= circleRadius;

			circle.setPosition(shapePosition);
			circle.setRadius(GetCircleColliderRadius());
			sf::Color collisionColor = sf::Color::Green;

			if (m_colliding)
				collisionColor = sf::Color::Red;

			collisionColor.a = 100;
			circle.setFillColor(collisionColor);
			_target.draw(circle);
		}
			break;
		case CollisionType::AABB:
		{
			sf::RectangleShape rectangle;
			sf::FloatRect bounds = GetAABB();
			rectangle.setPosition(bounds.left, bounds.top);
			rectangle.setSize(sf::Vector2f(bounds.width, bounds.height));

			sf::Color collisionColor = sf::Color::Green;

			if (m_colliding)
				collisionColor = sf::Color::Red;

			collisionColor.a = 100;
			rectangle.setFillColor(collisionColor);
			_target.draw(rectangle);
		}
			break;
		default:
			break;
		}
	}
}

sf::Vector2f SpriteObject::GetPosition()
{
	return m_position;
}

void SpriteObject::SetPosition(sf::Vector2f _newPosition)
{
	m_position = _newPosition;
	m_sprite.setPosition(_newPosition);
}

void SpriteObject::SetPosition(float m_newX, float m_newY)
{
	SetPosition(sf::Vector2f(m_newX, m_newY));
}

bool SpriteObject::CheckCollision(SpriteObject other)
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
			if(other.collisionType == CollisionType::AABB)

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

void SpriteObject::SetColliding(bool newColliding)
{
	m_colliding = newColliding;
}

sf::Vector2f SpriteObject::GetCollisionDepth(SpriteObject other)
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

void SpriteObject::HandleCollision(SpriteObject& other)
{
	//DO NOTHING, HANDLED IN CHILD CLASS.
}

void SpriteObject::SetAlive(bool _alive)
{
	m_isAlive = _alive;
}

sf::Vector2f SpriteObject::GetCollsionCentre()
{
	sf::Vector2f centre = m_position;

	sf::FloatRect bounds = m_sprite.getGlobalBounds();
	centre.x += bounds.width * 0.5f;
	centre.y += bounds.height * 0.5f;

	centre.x += m_CollisionOffset.x;
	centre.y += m_CollisionOffset.y;

	return centre;
}

float SpriteObject::GetCircleColliderRadius()
{
	sf::FloatRect bounds = m_sprite.getGlobalBounds();

	bounds.width = bounds.width * m_CollisionScale.x;
	bounds.height = bounds.height * m_CollisionScale.y;


	if (bounds.width > bounds.height)
	{
		return bounds.width *0.5f;
	}
	else
		return bounds.height* 0.5f;
}

sf::FloatRect SpriteObject::GetAABB()
{
	sf::FloatRect bounds = m_sprite.getGlobalBounds();

	bounds.width = bounds.width * m_CollisionScale.x;
	bounds.height = bounds.height * m_CollisionScale.y;

	sf::Vector2f centre = GetCollsionCentre();

	bounds.left = centre.x - bounds.width  * 0.5f;
	bounds.top = centre.y - bounds.height * 0.5f;


	return bounds;
}
