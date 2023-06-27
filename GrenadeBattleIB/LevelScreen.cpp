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
	, loopBreak(false)
	, doAnimations(true)
	, maxHeight(newGamePointer->GetWindow()->getSize().y)
	, maxWidth(newGamePointer->GetWindow()->getSize().x)
{
	//default positions for non dynamically allocated and test objects.



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
	if (playerOne.GetLives() <= 0)
	{
		if (doAnimations)
		{
			gameRunning = false;
			endPanel.StartAnimation(); //Practical Task - Easing Function
			endPanel.SetString("Player Two Wins!");
			doAnimations = false;
		}
	}

	if (playerTwo.GetLives() <= 0)
	{
		if (doAnimations)
		{
			gameRunning = false;
			endPanel.StartAnimation(); // Practical Task - Easing Function
			endPanel.SetString("Player One Wins");
			doAnimations = false;
		}
	}



	if (gameRunning)
	{

		doAnimations = true;
		CleanGrenades();

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

		}	

		for (int g = 0; g < grenades.size(); g++)
		{
			if (grenades[g]->CheckCollision(playerOne))
			{
				grenades[g]->HandleCollision(playerOne);
				playerOne.HandleCollision(*grenades[g]);
			}

			if (grenades[g]->CheckCollision(playerTwo))
			{
				grenades[g]->HandleCollision(playerTwo);
				playerTwo.HandleCollision(*grenades[g]);

			}
		}
	}
	else
	{
		{
			SetupUI();
			endPanel.Update(frameTime);
			Reset();
		}
	}


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
	_target.draw(playerOneHud);
	_target.draw(playerTwoHud);

	if (!gameRunning)
	{
		_target.draw(playerOneHud);
		_target.draw(playerTwoHud);
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
	newNade->SetPosition(position.x, position.y - 1);
	newNade->FireGrenade(velocity);
	grenades.push_back(newNade);

}



void LevelScreen::CleanGrenades()
{
	for (int i = grenades.size() - 1; i >= 0; --i)
	{
		if (grenades[i]->GetHitPlayer())
			loopBreak = true;

		if (grenades[i]->GetmarkedForDeletion())
		{
			
			delete grenades[i];
			grenades.erase(grenades.begin() + i);

			if (loopBreak)
			{				
				break;
			}
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
	playerOneHud.setPosition(50, 50);
	playerOneHud.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));

	playerTwoHud.setString(std::to_string(playerTwo.GetLives()));
	playerTwoHud.setFillColor(sf::Color::Cyan);
	playerTwoHud.setOutlineThickness(2.0f);
	playerTwoHud.setOutlineColor(sf::Color::Black);
	playerTwoHud.setCharacterSize(22);
	playerTwoHud.setPosition(1850, 50);
	playerTwoHud.setFont(AssetManager::RequestFont("Assets/dogica.ttf"));
}

void LevelScreen::Reset()
{
	if ((sf::Joystick::isButtonPressed(1, 0) || sf::Joystick::isButtonPressed(0, 0)) && !gameRunning)
	{
		endPanel.ResetPosition();
		playerOne.ResetLives();
		playerTwo.ResetLives();
		gameRunning = true;
		playerOne.SetPosition(30, maxHeight - 70);
		playerTwo.SetPosition(maxWidth - 100, maxHeight - 70);
	}
}
