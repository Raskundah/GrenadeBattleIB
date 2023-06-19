#include "EndPanel.h"
#include "AssetManager.h"
#include "Easing.h"

EndPanel::EndPanel(sf::RenderWindow* newWindow)
	: background()
	, title()
	, message()
	, position(0, 0)
	, window(newWindow)
	, animatingIn(false)
	, animationClock()
{
	background.setTexture(AssetManager::RequestTexture("Assets/Panel.png"));
	background.setScale(5.0f, 5.0f);

	title.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));
	title.setCharacterSize(70);
	title.setFillColor(sf::Color::Black);

	message.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));
	message.setCharacterSize(30);
	message.setString("Press A to restart,\nor ESCAPE to quit.");
	message.setFillColor(sf::Color::Black);

	ResetPosition();
}

void EndPanel::Update(sf::Time frameTime)
{
	if (animatingIn)
	{
		float xPos = window->getSize().x * 0.5f - background.getGlobalBounds().width * 0.5f;
		float yPos = window->getSize().y;
		float finalYPos = window->getSize().y * 0.5f - background.getGlobalBounds().height * 0.5f;

		sf::Vector2f begin(xPos, yPos);
		sf::Vector2f change(0, finalYPos - yPos);
		float duration = 1.0f;
		float time = animationClock.getElapsedTime().asSeconds();

		sf::Vector2f newPosition = Easing::QuadEaseOut(begin, change, duration, time);
		SetPosition(newPosition);

		if (time >= duration)
		{
			SetPosition(begin + change);
			animatingIn = false;
		}
	}

}

void EndPanel::Draw(sf::RenderTarget& target)
{
	target.draw(background);
	target.draw(message);
	target.draw(title);
}

void EndPanel::SetPosition(sf::Vector2f newPosition)
{
	background.setPosition(newPosition);

	// center the title on the x direction
	float titleX = background.getGlobalBounds().width * 0.5f - title.getGlobalBounds().width * 0.5f;
	title.setPosition(sf::Vector2f(newPosition.x + titleX, newPosition.y + 50));

	// center the message in the x and y direction
	float messageX = background.getGlobalBounds().width * 0.5f - message.getGlobalBounds().width * 0.5f;
	float messageY = background.getGlobalBounds().height * 0.5f - message.getGlobalBounds().height * 0.5f;
	message.setPosition(sf::Vector2f(newPosition.x + messageX, newPosition.y + messageY));
}

void EndPanel::StartAnimation()
{
	animatingIn = true;
	animationClock.restart();

}

void EndPanel::ResetPosition()
{
	float xPos = window->getSize().x * 0.5f - background.getGlobalBounds().width * 0.5f;
	float yPos = window->getSize().y;
	SetPosition(sf::Vector2f(xPos, yPos));
}

void EndPanel::SetString(std::string winText)
{
	title.setString(winText);
}
