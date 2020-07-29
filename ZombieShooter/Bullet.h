#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	//Bullet position
	Vector2f m_Position;

	//Bullet shape
	RectangleShape m_BulletShape;

	//Is flying
	bool m_InFlight = false;

	//speed
	float m_BulletSpeed = 1000;

	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//boundaries for bullet
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	Bullet();

	//stop the bullet
	void stop();

	//Is in flight
	bool isInFlight();

	//Shoot new bullet
	void shoot(float startX, float startY, float xTarget, float yTarget);

	//get bullet position
	FloatRect getPosition();

	//return bullet shape
	RectangleShape getShape();

	//Update each frame
	void update(float elapsedTime);
};