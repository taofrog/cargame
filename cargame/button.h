#pragma once
#include <raylib.h>
#include <string>
class button
{
private:
	Rectangle box;
	Color col;
	std::string text;
public:
	button(Vector2 pos, Vector2 size, Color colour = BLACK, std::string label = "");

	bool pressed();

	void draw();
};

