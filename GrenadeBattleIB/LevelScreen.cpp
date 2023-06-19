#include "LevelScreen.h"
#include "AssetManager.h"
#include "Platform.h"
#include "Game.h"
#include "Physics.h"


LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, playerOne(0, this)
	, playerTwo(1, this)
	, platforms()
	, endPanel(newGamePointer->GetWindow())
	, gameRunning(true)
{
	//default positions for non dynamically allocated and test objects.


	int maxWidth = newGamePointer->GetWindow()->getSize().x;
	int maxHeight = newGamePointer->GetWindow()->getSize().y;
	int width = 0;
	int height = 0;

	playerOne.SetPosition(30, maxHeight-400);
	playerTwo.SetPosition(maxWidth - 100, maxHeight-400);

	for (int i = 0; width <= maxWidth; ++i)
	{
		platforms.push_back(new Platform(sf::Vector2f(width, 0)));
		platforms.push_back(new Platform(sf::Vector2f(width, maxHeight-48)));
		width += 24;
	}

	for (int i = 0; height <= maxHeight; ++i)
	{
		platforms.push_back(new Platform(sf::Vector2f(0, height)));
		platforms.push_back(new Platform(sf::Vector2f(maxWidth-24, height)));
		height += 24;

	}

	width = 0;
	height = 0;

	for (int i = 0; i < 10; ++i)
	{
		platforms.push_back(new Platform(sf::Vector2f(maxWidth *0.4f + width, maxHeight *0.75f )));
		width += 24;
	}

	width = 0;

	for (int i = 0; i < 5; ++i)
	{
		platforms.push_back(new Platform(sf::Vector2f(maxWidth *0.6f + width, maxHeight * 0.75f)));
		platforms.push_back(new Platform(sf::Vector2f(maxWidth * 0.2f + width, maxHeight * 0.75f)));

		width += 24;
	}
	width = 0;
}

void LevelScreen::Update(sf::Time frameTime)
{
	if (playerOne.GetLives() == 0)
	{
		gameRunning = false;
		endPanel.StartAnimation(); //Practical Task - Easing Function

		endPanel.SetString("Player One Wins!");
	}

	if (playerTwo.GetLives() == 0)
	{
		gameRunning = false;
		endPanel.StartAnimation(); // Practical Task - Easing Function
		endPanel.SetString("Player Two Wins");
	}



	if (gameRunning)
	{
		SetupUI();

		//update moving positions

		playerOne.Update(frameTime);
		playerTwo.Update(frameTime);
		for (int i = 0; i < grenades.size(); ++i)
		{
			grenades[i]->Update(frameTime);

			if (grenades[i]->GetClock().getElapsedTime() > sf::seconds(2.5f))
				grenades[i]->SetMarkedForDeletion(true);
		}	
		//default colllisiuon states

		playerOne.SetColliding(false);
		playerTwo.SetColliding(false);

		for (int i = 0; i < platforms.size(); ++i)
		{
			platforms[i]->SetColliding(false);

			if (platforms[i]->CheckCollision(playerOne))
			{
				playerOne.SetColliding(true);
				platforms[i]->SetColliding(true);
				playerOne.HandleCollision(*platforms[i]);
			}

			if (platforms[i]->CheckCollision(playerTwo))
			{
				playerTwo.SetColliding(true);
				platforms[i]->SetColliding(true);
				playerTwo.HandleCollision(*platforms[i]);
			}

			for (int g = 0; g < grenades.size(); g++)
			{
				if (grenades[g]->CheckCollision(*platforms[i]))
				{
					grenades[g]->HandleCollision(*platforms[i]);
				}
			}
			for (int g = 0; g < grenades.size(); g++)
			{
				if (grenades[g]->CheckCollision(playerOne))
				{
					grenades[g]->HandleCollision(playerOne);
				}

				if (grenades[g]->CheckCollision(playerTwo))
				{
					grenades[g]->HandleCollision(playerTwo);
				}
			}
		}	
	}
	else
	{
		{
			endPanel.Update(frameTime);
			Reset();
		}
	}

		CleanGrenades();

}

//draw all objects to game window

void LevelScreen::Draw(sf::RenderTarget& _target)
{
	for (int i = 0; i < platforms.size(); ++i)
	{
		platforms[i]->Draw(_target);
	}	

	playerOne.Draw(_target);
	playerTwo.Draw(_target);

	if (!gameRunning)
	{
		endPanel.Draw(_target);
	}

	for (int i = 0; i < grenades.size(); ++i)
	{
		grenades[i]->Draw(_target);
	}
}

void LevelScreen::TriggerEndState(bool _win)
{
	gameRunning = false;
	endPanel.StartAnimation();
}

void LevelScreen::ShootGrenade(sf::Vector2f position, sf::Vector2f velocity, int playerID)
{
	Grenade* newNade = new Grenade(playerID);
	newNade->SetPosition(position.x, position.y - 10);
	newNade->FireGrenade(velocity);
	grenades.push_back(newNade);

}



void LevelScreen::CleanGrenades()
{
	for (int i = grenades.size() - 1; i >= 0; --i)
	{
		if (grenades[i]->GetmarkedForDeletion())
		{
			delete grenades[i];
			grenades.erase(grenades.begin() + i);
		}
	}
}

void LevelScreen::SetupUI()
{
	playerOneHud.setString(std::to_string(playerOne.GetLives()));
	playerOneHud.setFillColor(sf::Color::Cyan);
	playerOneHud.setOutlineThickness(2.0f);
	playerOneHud.setOutlineColor(sf::Color::Black);
	playerOneHud.setCharacterSize(22);
	playerOneHud.setPosition(0, 20);
	playerOneHud.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));

	playerTwoHud.setString(std::to_string(playerOne.GetLives()));
	playerTwoHud.setFillColor(sf::Color::Cyan);
	playerTwoHud.setOutlineThickness(2.0f);
	playerTwoHud.setOutlineColor(sf::Color::Black);
	playerTwoHud.setCharacterSize(22);
	playerTwoHud.setPosition(1900, 20);
	playerTwoHud.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));
}

void LevelScreen::Reset()
{
	if ((sf::Joystick::isButtonPressed(1, 0) || sf::Joystick::isButtonPressed(0, 0)) && !gameRunning)
	{
		endPanel.ResetPosition();
		playerOne.SetLives(3);
		playerTwo.SetLives(3);
		gameRunning = true;

	}
}
