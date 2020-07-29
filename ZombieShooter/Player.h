#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	//Player's position
	Vector2f m_Position;

	// PLayer's sprite and texture
	Sprite m_Sprite;
	Texture m_Texture;

	//Screen resolution
	Vector2f m_Resolution;

	//Arena size
	IntRect m_Arena;
	int m_TileSize;

	//Player movement direction
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	//Current health
	int m_Health;

	//Max health
	int m_MaxHealth;

	//Last time player was hit
	Time m_LastHit;

	//Player's speed
	float m_Speed;

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	void resetPlayerStats();

	//Handles player getting hit
	bool hit(Time timeHit);

	//How long ago was the player last hit
	Time getLastHitTime();

	//Player's location
	FloatRect getPosition();

	//Player's center
	Vector2f getCenter();

	//get player's rotation
	float getRotation();

	//Get player's sprite
	Sprite getSprite();

	//Move Player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//stop movement to specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//Called every frame
	void Update(float elapsedTime, Vector2i mousePosition);

	//gives speed boost 
	void UpgradeSpeed();

	//heals player
	void UpgradeHealth();

	//Increases max health palyer can have
	void increaseHealthLevel(int amount);

	//return current health
	int getHealth();

	void setFullHealth();
};
