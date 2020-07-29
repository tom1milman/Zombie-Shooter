#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	//start values for pickups
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	//Sprite
	Sprite m_Sprite;

	//Arena
	IntRect m_Arena;

	//pcikup value
	int m_Value;

	//1 = health, 2 = ammo
	int m_Type;

	//spawning and disappearing variables
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup(int value);

	//Prepare new ppickup
	void setArena(IntRect arena);

	void spawn();

	//return position
	FloatRect getPosition();

	//retur sprite
	Sprite getSprite();

	//update pickup each frame
	void update(float elastedTime);

	//is pickup spawned
	bool isSpawned();

	//get pickup
	int gotIt();

	//Upgrade value of pickup
	void upgrade();
};