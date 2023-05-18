#include "EndPanel.h"
#include "AssetManager.h"
#include "Easing.h"

EndPanel::EndPanel(sf::RenderWindow* newWindow)
	: m_background()
	, m_message()
	, m_title()
	,m_position(0, 0)
	,m_window(newWindow)
	, m_animatingIn(false)
	, m_animationClock()
{
	//.setTexture(AssetManager::RequestTexture("Assets/Graphics/Panel.png"));
	//m_background.setScale(5.0f, 5.0f);

	m_title.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));
	m_title.setCharacterSize(70);
	m_title.setString("YOU WIN!");

	m_message.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));
	m_message.setCharacterSize(30);
	m_message.setString("Press R to reset game, \nor ESCAPE to Quit.");

	float xPos = (m_window->getSize().x - m_background.getGlobalBounds().width) * 0.5f;
	float yPos = m_window->getSize().y;
	SetPosition(sf::Vector2f(xPos, yPos));
}


void EndPanel::Update(sf::Time _frameTime)
{
	if (m_animatingIn) {

		float xPos = (m_window->getSize().x - m_background.getGlobalBounds().width) * 0.5f;
		float yPos = (m_window->getSize().y);

		sf::Vector2f begin(xPos, yPos);
		float finalYPos = (m_window->getSize().y - m_background.getGlobalBounds().height) * 0.5f;

		sf::Vector2f change(0, finalYPos - yPos);
		float duration = 1.0f;
		float time = m_animationClock.getElapsedTime().asSeconds();

		sf::Vector2f newPosition = Easing::QuadEaseIn(begin, change, duration, time);
		SetPosition(newPosition);

		if (time >= duration)
		{
			SetPosition(begin + change);
			m_animatingIn = false;
		}
	}
}

void EndPanel::Draw(sf::RenderTarget& _target)
{
	_target.draw(m_background);
	_target.draw(m_message);
	_target.draw(m_title);
}

void EndPanel::SetPosition(sf::Vector2f newPosition)
{
	m_background.setPosition(newPosition);

	//centre the title on the x direction
	float titleX = m_background.getGlobalBounds().width * 0.5f - m_title.getGlobalBounds().width * 0.5f;
	float titleY = m_background.getGlobalBounds().height * 0.5f - m_title.getGlobalBounds().height * 0.5f;

	m_title.setPosition(newPosition.x + titleX, newPosition.y + 50);

	float messageX = m_background.getGlobalBounds().width * 0.5f - m_message.getGlobalBounds().width * 0.5f;
	float messageY = m_background.getGlobalBounds().height * 0.5f - m_message.getGlobalBounds().height * 0.5f;

	m_message.setPosition(newPosition.x + messageX, newPosition.y + messageY);

}

void EndPanel::StartAnimation()
{
	m_animatingIn = true;
	m_animationClock.restart();
}
