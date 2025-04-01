#include "obstacle.h"

obstacle::obstacle(Vector2 start, Vector2 end) 
{
	p1 = start;
	p2 = end;

	Rectangle r = { p1.x, p1.y, abs(p1.x - p2.x), abs(p1.y - p2.y) };
	if (p1.x > p2.x) {
		r.x = p2.x;
	}
	if (p1.y > p2.y) {
		r.y = p2.y;
	}

	bounds = r;
}

Vector2 obstacle::findClosestPoint(Vector2 targe) {
	Vector2 target = targe;
	// rotate so line is x axis
	Vector2 v1 = { 0, 0 };
	Vector2 v2 = { p2.x - p1.x, p2.y - p1.y };
	target.x = target.x - p1.x;
	target.y = target.y - p1.y;

	float angle = atan2f(v2.x, v2.y);
	Vector2 rv2, rtarget;
	rv2.y = v2.x * cos(angle) - v2.y * sin(angle);
	rv2.x = v2.x * sin(angle) + v2.y * cos(angle);

	rtarget.y = target.x * cos(angle) - target.y * sin(angle);
	rtarget.x = target.x * sin(angle) + target.y * cos(angle);
	rtarget.x = std::min(std::max(rtarget.x, 0.0f), rv2.x);
	rtarget.y = 0;

	target.y = rtarget.x * cos(angle) - rtarget.y * sin(angle);
	target.x = rtarget.x * sin(angle) + rtarget.y * cos(angle);

	target.x = target.x + p1.x;
	target.y = target.y + p1.y;

	return target;
}

float obstacle::angle() {
	Vector2 v = { p2.x - p1.x, p2.y - p1.y };

	return atan2f(v.y, v.x);
}

void obstacle::draw(Color colour) 
{
	//DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, {0, 255, 0, 100});
	DrawLine(p1.x, p1.y, p2.x, p2.y, colour);
}