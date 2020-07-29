#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:
	//Zombie speeds
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	//Zombie health
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//Zombie position
	Vector2f m_Position;

	//Zombie sprite
	Sprite m_Sprite;

	//individual speed
	float m_Speed;

	//health
	float m_Health;

	bool m_Alive;

public:
	//Handles when bullet hits zombie
	bool hit();

	//Is zombie still alive
	bool isAlive();

	//Soawns a zombie
	void spawn(float startX, float startY, int type, int seed);

	//returns zombie position
	FloatRect getPosition();

	//returns sprite
	Sprite getSprite();

	//Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
};