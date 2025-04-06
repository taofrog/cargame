#pragma once
#include <iostream>
#include <raylib.h>
#include <cmath>
#include "vehicle.h"
#include "obstacle.h"
#include "wallloop.h"
#include "button.h"
#include "decoration.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <thread>
#include <future>
#include "glad/glad.h"
namespace fs = std::filesystem;

#define GLSL_VERSION 330

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

	wallloop loadlevel(int level);

	Texture drawmap(wallloop walls, int width, int height);

	std::vector<Vector2> gendeco(Texture map, int maxcount);

	std::vector<int> levellist();

	static Vector2 getdirection(int left = KEY_NULL, int right = KEY_NULL, int forward = KEY_NULL, int back = KEY_NULL);
};

