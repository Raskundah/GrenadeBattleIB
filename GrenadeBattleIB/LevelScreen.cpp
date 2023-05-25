#include "LevelScreen.h"
#include "AssetManager.h"
#include "Platform.h"
#include "Game.h"
#include "Physics.h"

LevelScreen::LevelScreen(Game* newGamePointer)
	: Screen(newGamePointer)
	, playerOne(0)
	, playerTwo(1)
	, platforms()

	, endPanel(newGamePointer->GetWindow())
	, gameRunning(true)
{
	//default positions for non dynamically allocated and test objects.

	//TODO: add vectors of object positions.


	int maxWidth = newGamePointer->GetWindow()->getSize().x;
	int maxHeight = newGamePointer->GetWindow()->getSize().y;
	int width = 0;
	int height = 0;


	playerOne.SetPosition(30, maxHeight-53-100);
	playerTwo.SetPosition(maxWidth - 100, maxHeight-53-100);

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

}

void LevelScreen::Update(sf::Time frameTime)
{
	if (gameRunning)
	{

		//update moving positions

		playerOne.Update(frameTime);
		playerTwo.Update(frameTime);
		/*for (int i = 0; i < platforms.size(); ++i)
		{
			platforms[i]->Update(frameTime);
		}
		*/

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
				//platforms[i]->HandleCollision(playerOne);
			}

			if (platforms[i]->CheckCollision(playerTwo))
			{
				playerTwo.SetColliding(true);
				platforms[i]->SetColliding(true);
				playerTwo.HandleCollision(*platforms[i]);
				// platforms[i]->HandleCollision(playerTwo);
			}
		}

		
	}

		endPanel.Update(frameTime);

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
}

void LevelScreen::TriggerEndState(bool _win)
{
	gameRunning = false;
	endPanel.StartAnimation();
}
