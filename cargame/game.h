#pragma once
#include <iostream>
#include <raylib.h>
#include <cmath>
#include "vehicle.h"
#include "obstacle.h"
#include "wallloop.h"
#include "button.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

class game
{
public:
	game(int w, int h) {
		swidth = w;
		sheight = h;
	}

	void rungame(int level);

	void runmapcreation();

	int runmenu();

private:
	int swidth;
	int sheight;

	void savelevel();

	void loadlevel() {}

	Texture drawmap(wallloop walls, int width, int height);

	static Vector2 getdirection(int left = KEY_NULL, int right = KEY_NULL, int forward = KEY_NULL, int back = KEY_NULL);
};

