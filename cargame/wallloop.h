#pragma once
#include <raylib.h>
#include <raymath.h>
#include "obstacle.h"
#include "vehicle.h"
#include <vector>
#include <string>

class wallloop
{
private:
	std::vector<obstacle> walls;
public:
	wallloop() {};

	wallloop(std::string args);

	wallloop(std::string args1, std::string args2);

	std::vector<obstacle> getWalls() { return walls; }
	
	void addloop(std::string args);

	void addloop(std::string args1, std::string args2) { addloop(args1); addloop(args2); }

	void collisions(vehicle &car);

	void draw();
};

