#include "vehicle.h"

//Vector2 pos;
//Vector2 vel;
//Vector2 siz;
//float rot;
//float rotvel;

vehicle::vehicle(Vector2 position, Vector2 size, float velocity) {
	pos = position;
	siz = size;
	vel = velocity;
	rot = 0;
	rotvel = 0;
}

void vehicle::updatePhysics(float dt) {
	rot += rotvel * dt;
	
	pos.x += vel * cos(rot * DEG2RAD) * dt;
	pos.y += vel * sin(rot * DEG2RAD) * dt;
}

void vehicle::updateMovement(Vector2 mov, float dt) {
	if (mov.x != 0) {
		// normal
		int sidemovement = mov.x / abs(mov.x);
		if (abs(rotvel) < maxrotvel) {
			rotvel += sidemovement * rotaccel * dt;
		}
	}
	else if (abs(rotvel) > rotdecel * dt) {
		rotvel -= rotvel / abs(rotvel) * rotdecel * dt;
	}
	else {
		rotvel = 0;
	}
	if (mov.y != 0) {
		// normal
		int forwardsmovement = mov.y / abs(mov.y);
		vel += forwardsmovement * accel * dt;
	}
	else if (abs(vel) > decel * dt){
		vel -= vel / abs(vel) * decel * dt;
	}
	else {
		vel = 0;
	}
}

void vehicle::draw(Color colour) {
	Rectangle r;
	r.x = pos.x;
	r.y = pos.y;
	r.width = siz.x * 2;
	r.height = siz.y * 2;

	DrawRectanglePro(r, siz, rot, colour);

	Vector2 p1, p2, p3;
	p1 = { 2 * siz.x * cos(rot * DEG2RAD), 2 * siz.x * sin(rot * DEG2RAD) };
	p2 = { siz.x * cos(rot * DEG2RAD) - -siz.y * sin(rot * DEG2RAD), siz.x * sin(rot * DEG2RAD) + -siz.y * cos(rot * DEG2RAD) };
	p3 = { siz.x * cos(rot * DEG2RAD) - siz.y * sin(rot * DEG2RAD), siz.x * sin(rot * DEG2RAD) + siz.y * cos(rot * DEG2RAD) };
	p1.x += pos.x;
	p1.y += pos.y;
	p2.x += pos.x;
	p2.y += pos.y;
	p3.x += pos.x;
	p3.y += pos.y;
	DrawTriangle(p1, p2, p3, BLACK);
}

void vehicle::bcollide(Vector2 contact, float angle) {
	Vector2 normal = { -sin(angle), cos(angle)};
	if (((int)(rot - angle * RAD2DEG - 90)%360 + 360)%360 > 180) {
		rotvel = 300;
	}
	else {
		rotvel = -300;
	}
	pos.x += normal.x;
	pos.y += normal.y;
}

void vehicle::collide(Vector2 contact, float angle) {
	vel = 0;
	bcollide(contact, angle);
}