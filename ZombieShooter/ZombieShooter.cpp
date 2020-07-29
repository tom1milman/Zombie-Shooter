#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieShooter.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <sstream>
#include <fstream>

using namespace sf;

int main()
{
	TextureHolder holder;
	//Four game states
	enum class State
	{
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};
	State state = State::GAME_OVER;

	//create sfml view from screen resolution
	Vector2f resolution;
	resolution.x = 1280;
	resolution.y = 720;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Shooter", Style::Fullscreen);

	//Create SFML view for main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Main clock
	Clock clock;

	Time gameTimeTotal;

	//Mouse position to world coordinate
	Vector2f mouseWorldPosition;
	//Mouse position to screen coordinates
	Vector2i mouseScreenPosition;

	//Player
	Player player;

	//Arena bounderies
	IntRect arena;

	//Create background
	VertexArray background;
	Texture backgroundTexture = TextureHolder::GetTexture("graphics/background_sheet.png");

	//Zombie variables
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//Bullet variables
	int const TOTAL_NUM_BULLETS = 100;
	Bullet bullets[TOTAL_NUM_BULLETS];
	int currentBullet = 0;
	int bulletsSpare = 30;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;

	//last fire button pressed
	Time lastPressed;

	//Mouse crosshair
	window.setMouseCursorVisible(false);
	Sprite crosshairSprite;
	
	crosshairSprite.setTexture(TextureHolder::GetTexture("graphics/crosshair.png"));
	crosshairSprite.setOrigin(25, 25);

	//Pickup items
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//Score
	int score = 0;
	int hiScore = 0;

	//gamvoer sprite
	Sprite gameOverSprite;
	gameOverSprite.setTexture(TextureHolder::GetTexture("graphics/background.png"));
	gameOverSprite.setPosition(0, 0);

	//HUD view
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Ammo icon sprite
	Sprite ammoIconSprite;
	ammoIconSprite.setTexture(TextureHolder::GetTexture("graphics/ammo_icon.png"));
	ammoIconSprite.setPosition(28, 620);

	//Font
	Font font; 
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//Game Pause text
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(85);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(350, 200);
	pausedText.setString("Press Enter\nto continue");

	//Gameover text
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(80);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(280, 540);
	gameOverText.setString("Press Enter to play");

	//Leveling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(60);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(80, 150);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1 - Increased fire rate" <<
		"\n2 - Increased clip size" <<
		"\n3 - Increased max health" <<
		"\n4 - Increased speed" <<
		"\n5 - Better health packs" <<
		"\n6 - Better ammo packs";
	levelUpText.setString(levelUpStream.str());

	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(50);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(116, 620);

	//score Text
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	//Load high score
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	//Hi score text
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(50);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(980, 0);
	std::stringstream s;
	s << "Hi Score: " << hiScore;
	hiScoreText.setString(s.str());

	//Zombie alive
	Text zombieAliveText;
	zombieAliveText.setFont(font);
	zombieAliveText.setCharacterSize(50);
	zombieAliveText.setFillColor(Color::White);
	zombieAliveText.setPosition(975,620);
	zombieAliveText.setString("Zombies: 100");
	
	//Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(50);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(750, 620);
	waveNumberText.setString("Wave: 0");

	//Health Bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(300, 620);

	//Hud update time
	int frameSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 100;

	int soundVolume = 5;

	//Hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);
	hit.setVolume(soundVolume);

	//Splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);
	splat.setVolume(soundVolume);

	//Shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);
	shoot.setVolume(soundVolume);

	//Reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);
	reload.setVolume(soundVolume);

	//Reload failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);
	reloadFailed.setVolume(soundVolume);

	//powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);
	powerup.setVolume(soundVolume);

	//Pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);
	pickup.setVolume(soundVolume);

	//Main game loop
	while (window.isOpen())
	{
#pragma region Player input
		//Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				//If Enter key was pressed
				if (event.key.code == Keyboard::Enter)
				{
					//pause game while palying
					if (state == State::PLAYING)
						state = State::PAUSED;
					//Restart while paused
					else if (state == State::PAUSED)
					{
						state = State::PLAYING;
						clock.restart();

					}
					//Start game after game over
					else if (state == State::GAME_OVER)
					{
						state = State::LEVELING_UP;
						wave = 0;
						score = 0;

						currentBullet = 0;
						bulletsSpare = 30;
						bulletsInClip = 6;
						clipSize = 6; 
						fireRate = 1;

						player.resetPlayerStats();
					}
				}

				if (state == State::PLAYING)
				{
					//Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0)
						{
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else
						{
							reloadFailed.play();
						}
					}
				}
			}
		}//End event polling

		//quit game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		//Handle gameplay input
		if (state == State::PLAYING)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
				player.moveUp();
			else
				player.stopUp();

			if (Keyboard::isKeyPressed(Keyboard::S))
				player.moveDown();
			else
				player.stopDown();

			if (Keyboard::isKeyPressed(Keyboard::A))
				player.moveLeft();
			else
				player.stopLeft();

			if (Keyboard::isKeyPressed(Keyboard::D))
				player.moveRight();
			else
				player.stopRight();

			//Fire bullet
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate
					&& bulletsInClip > 0)
				{
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;

					if (currentBullet > TOTAL_NUM_BULLETS - 1)
						currentBullet = 0;

					lastPressed = gameTimeTotal;

					shoot.play();

					bulletsInClip--;
				}
			}// End firing fullet
		}

		//Leveling up state
		if (state == State::LEVELING_UP)
		{
			//Increase fire rate
			if (event.key.code == Keyboard::Num1)
			{
				fireRate++;
				state = State::PLAYING;
			}

			//Increase clip size
			if (event.key.code == Keyboard::Num2)
			{
				clipSize += clipSize;
				state = State::PLAYING;
			}

			//Increase health
			if (event.key.code == Keyboard::Num3)
			{
				player.UpgradeHealth();
				state = State::PLAYING;
			}

			//Increase speed
			if (event.key.code == Keyboard::Num4)
			{
				player.UpgradeSpeed();
				state = State::PLAYING;
			}

			//Upgrade heakth pickup
			if (event.key.code == Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			//Upgrade ammo pickup
			if (event.key.code == Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				wave++;

				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;
				
				//Creating background
				int tileSize = createBackground(background, arena);

				//Spawn player
				player.spawn(arena, resolution, tileSize);

				//Configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//Create zombies horde
				numZombies = 5 * wave;
				
				//Delete previously allocated memory
				delete[] zombies;
				//Create new zombies
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//play powerup sound
				powerup.play();

				clock.restart();
			}
		}//End leveling up state
#pragma endregion

#pragma region Frame Update
		if (state == State::PLAYING) 
		{
			//Update delta time
			Time dt = clock.restart();
			float dtAsSeconds = dt.asSeconds();
			//update total game time
			gameTimeTotal += dt;

			//Get mouse position
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//Position crosshair with mouse position
			crosshairSprite.setPosition(mouseWorldPosition);

			//Update player
			player.Update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());
			
			//Center view to palyer
			mainView.setCenter(player.getCenter());

			//update zombies
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
					zombies[i].update(dt.asSeconds(), playerPosition);
			}

			//Update in flight bullets
			for (int i = 0; i < TOTAL_NUM_BULLETS; i++)
			{	
				if (bullets[i].isInFlight())
					bullets[i].update(dtAsSeconds);
			}

			//Update pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			//Bullet zombie collision detection
			for (int i = 0; i < TOTAL_NUM_BULLETS; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						//did bullet intersect with zombie
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							//stop bullet
							bullets[i].stop();

							//did zombie die?
							if (zombies[j].hit())
							{
								score += 10;

								if (score >= hiScore)
									hiScore = score;

								numZombiesAlive--;

								if (numZombiesAlive <= 0)
									state = State::LEVELING_UP;
							}

							splat.play();
						}
					}
				}
			}// end bullet zombie collision detection


			//Player Zombie collision
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive() && player.getPosition().intersects(zombies[i].getPosition()))
				{
					if (player.hit(gameTimeTotal))
					{
						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			} //end player zombie collision

			//Player health pickup collision
			if (healthPickup.isSpawned() && player.getPosition().intersects(healthPickup.getPosition()))
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();
			}
			//Player ammo pickup collision
			if (ammoPickup.isSpawned() && player.getPosition().intersects(ammoPickup.getPosition()))
			{
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
			}

			//Health bar size update
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			frameSinceLastHUDUpdate++;

			//Update HUD
			if (frameSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//Update ammo
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				//Update score
				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());

				//Update high score
				ssHiScore << "High Score: " << hiScore;
				hiScoreText.setString(ssHiScore.str());

				//update wave
				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());

				//Update zombies alive
				ssZombiesAlive << "Zombies: " << numZombiesAlive;
				zombieAliveText.setString(ssZombiesAlive.str());

				frameSinceLastHUDUpdate = 0;
			}//End HUD update

		}//End frame update;
#pragma endregion

#pragma region Draw Scene
		if (state == State::PLAYING)
		{
			window.clear();

			//Set mainView to be displayed in window and draw everything related to it
			window.setView(mainView);

			//Draw background
			window.draw(background, &backgroundTexture);

			//Draw zombies
			for (int i = 0; i < numZombies; i++)
				window.draw(zombies[i].getSprite());

			for (int i = 0; i < TOTAL_NUM_BULLETS; i++)
			{
				if (bullets[i].isInFlight())
					window.draw(bullets[i].getShape());
			}

			//Draw player
			window.draw(player.getSprite());

			//Draw pickups
			if (ammoPickup.isSpawned())
				window.draw(ammoPickup.getSprite());

			if (healthPickup.isSpawned())
				window.draw(healthPickup.getSprite());

			//Draw crosshair
			window.draw(crosshairSprite);

			//Switch to HUD view
			window.setView(hudView);

			//Draw HUD elements
			window.draw(ammoIconSprite);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombieAliveText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(gameOverSprite);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(gameOverSprite);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();
#pragma endregion


	}//End game loop

	delete[]zombies;

	return 0;
}
