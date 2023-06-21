#include "Grenade.h"
#include "AssetManager.h"
#include "Platform.h"
#include "Player.h"
#include "VectorHelper.h"



Grenade::Grenade(int _whichPlayer)
    : Physics()
    , whichPlayer(_whichPlayer)
    , expiryTime(sf::seconds(2.5f))
    , shouldBeDeleted(false)
    , expiryClock()
    , hitPlayer(false)
	
{
	m_sprite.setTexture(AssetManager::RequestTexture("Assets/grenade.png"));
	m_sprite.setScale(sf::Vector2f(0.5f, 0.5f));
	applyDrag = false;
}



void Grenade::FireGrenade(sf::Vector2f fireVel)
{
	m_velocity = fireVel;
}

void Grenade::HandleCollision(Physics& other)
{
    if (dynamic_cast<Platform*>(&other) != nullptr)
    {

        sf::Vector2f depth = (GetCollisionDepth(other));
        sf::Vector2f newPosition = GetPosition();

        sf::Vector2f topRight = sf::Vector2f(other.GetAABB().left + other.GetAABB().width, other.GetAABB().top);
        sf::Vector2f bottomLeft = sf::Vector2f(other.GetAABB().left, other.GetAABB().top + other.GetAABB().height);
        sf::Vector2f topLeft = sf::Vector2f(other.GetAABB().left, other.GetAABB().top);

        sf::Vector2f line;

        if (abs(depth.x) < abs(depth.y))
        {
            line = VectorHelper::Normalise(VectorHelper::GetNormal(topLeft - bottomLeft));

            //Move in x direction
            newPosition.x += depth.x * 2.0f;
            m_velocity = VectorHelper::GetReflection(m_velocity, line);

           
        }
        else
        {
            line = VectorHelper::Normalise(VectorHelper::GetNormal(topLeft - topRight));

            //Move in y direction
            newPosition.y += depth.y * 2.0f;
            m_velocity = VectorHelper::GetReflection(m_velocity, line);

     

            /*Collision from above
            if (depth.y < 0)
            {
            }
            */
        }

        SetPosition(newPosition);
    }

    if ((dynamic_cast<Player*>(&other) != nullptr) && !hitPlayer && dynamic_cast<Player*>(&other)->GetPlayerID() != whichPlayer)
    {
          hitPlayer = true;
          shouldBeDeleted = true;
    }
}

bool Grenade::GetmarkedForDeletion()
{
    return shouldBeDeleted;
}

void  Grenade::SetMarkedForDeletion(bool value)
{
    shouldBeDeleted = value;
}

sf::Clock Grenade::GetClock()
{
    return expiryClock;
}

void Grenade::ResetClock()
{
    expiryClock.restart();
}

int Grenade::GetPlayerID()
{
    return whichPlayer;
}

bool Grenade::GetHitPlayer()
{
    return hitPlayer;
}

