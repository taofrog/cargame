#pragma once
#include <raylib.h>
#include <iostream>
#include <cmath>
#include <algorithm>
class obstacle
{
private:
	Vector2 p1, p2;

	Rectangle bounds;
public:
	obstacle(Vector2 start, Vector2 end);

	Vector2 findClosestPoint(Vector2 point);

	float angle();

	Rectangle getRecBounds() { return bounds; }
	
	Vector2 getp1() { return p1; }
	Vector2 getp2() { return p2; }

	void draw(Color colour);

	void draw() {
		draw({ 255, 0, 0, 255 });
	}
};

