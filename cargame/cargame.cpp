// cargame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
#include "game.h"
namespace fs = std::filesystem;

int main()
{
    const int screenwidth = 1500;
    const int screenheight = 1000;
    InitWindow(screenwidth, screenheight, "Racing");

    game Game = game(screenwidth, screenheight);

    SetTargetFPS(120);
    int level = Game.runmenu();
    if (level == -1) {
        Game.runmapcreation();
    }
    Game.rungame(level);

    CloseWindow();
    return 0;
}
