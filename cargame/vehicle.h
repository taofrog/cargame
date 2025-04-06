#pragma once
#include <raylib.h>
#include <cmath>
#include <iostream>

class vehicle
{
private:
	Vector2 pos;
	float vel;
	Vector2 siz;
	float rot;
	float rotvel;
	Texture2D icon;
public:
	float accel = 80; // p s^-2
	float decel = 250;  // p s^-2
	float rotaccel = 1000; //degree s^-2
	float rotdecel = 2000; //degree s^-2
	float maxrotvel = 600;

	Color col = RED;

	Vector2 getSize() { return siz; }
	Vector2 getPosition() { return pos; }
	float getVelocity() { return vel; }
	float getRotation() { return rot; }
	float getRotationVel() { return rotvel; }

	vehicle(Vector2 position, Vector2 size, std::string texpath = "", float velocity = 0);

	void updatePhysics(float dt);

	void updateMovement(Vector2 mov, float dt);

	void bcollide(Vector2 contact, float angle);

	void collide(Vector2 contact, float angle);

	void draw(Color colour);

	void draw() { draw(col); }
};

