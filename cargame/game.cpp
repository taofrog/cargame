#include "game.h"

void game::savelevel() {
    int pathlen = (int)fs::current_path().string().length() + 1;
    std::vector<int> levels;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        std::string file = entry.path().string().erase(0, pathlen);
        if (file.length() > 9) {
            if (file.substr(0, 5) == "level") {
                file = file.erase(file.length() - 4, file.length() - 1);
                file = file.erase(0, 5);
                levels.push_back(std::stoi(file));
            }
        }
    }

    int freefile = (int)levels.size();
    int prev = -1;
    for (int i = 0; i < levels.size(); i++) {
        if (i != levels[i]) {
            freefile = i;
            break;
        }
    }

    std::ifstream currentfile = std::ifstream("level.txt");
    std::string linedata;

    std::string newfilename = std::string("level") + std::to_string(freefile) + std::string(".txt");
    std::ofstream newfile(newfilename);

    if (currentfile.is_open() && newfile.is_open()) {
        newfile.clear();
        while (std::getline(currentfile, linedata)) {
            newfile << linedata << "\n";
        }
        currentfile.close();
    }
    else {
        std::cout << "file wont open\n";
    }
}

void game::rungame(int level) {
    SetWindowTitle("Racing");

    const int carnum = 4;

    vehicle cars[carnum] = { vehicle(Vector2{ 450, 350 }, Vector2{ 10, 10 }),
                       vehicle(Vector2{ 450, 400 }, Vector2{ 10, 10 }),
                       vehicle(Vector2{ 450, 450 }, Vector2{ 10, 10 }),
                       vehicle(Vector2{ 450, 500 }, Vector2{ 10, 10 }) };
    cars[1].col = BLUE;
    cars[2].col = GREEN;
    cars[3].col = YELLOW;

    wallloop walls("");

    std::ifstream levelfile = std::ifstream();
    if (level < 0) {
        levelfile.open("level.txt");
    }
    else {
        std::string filename = std::string("level") + std::to_string(level) + std::string(".txt");
        levelfile.open(filename);
    }
    std::string leveldat;

    if (levelfile.is_open()) {
        while (std::getline(levelfile, leveldat)) {
            walls.addloop(leveldat);
        }
        levelfile.close();
    }
    else {
        std::cout << "file wont open\n";
    }

    bool saved = false;
    if (level >= 0) {
        saved = true;
    }

    Texture map = drawmap(walls, swidth, sheight);

    button savebutton(Vector2{ 1400, 20 }, Vector2{ 50, 20 }, RED, "Save");

    float dt = 0;

    bool game = true;
    while (game) {
        if (WindowShouldClose()) {
            game = false;
        }

        dt += 1 / (float)GetFPS();
        //stop dt being crazy
        if (dt > 1) { dt = 1; };

        if (!saved && savebutton.pressed()) {
            saved = true;
            savelevel();
        }

        Vector2 directions[] = { getdirection(KEY_Q, KEY_E, KEY_W), 
                                 getdirection(KEY_I, KEY_P, KEY_O), 
                                 getdirection(KEY_B, KEY_M, KEY_N), 
                                 getdirection(KEY_Z, KEY_C, KEY_X) };

        //fixed update
        float smallstep = 0.004f;
        while (dt >= smallstep) {
            for (int i = 0; i < carnum; i++) {
                cars[i].updateMovement(directions[i], smallstep);
            }
            for (int i = 0; i < carnum; i++) {
                cars[i].updatePhysics(smallstep);
            }
            for (int i = 0; i < carnum; i++) {
                for (int j = 3; j > i; j--) {
                    Vector2 p1 = cars[i].getPosition();
                    Vector2 p2 = cars[j].getPosition();
                    float r1 = cars[i].getSize().x;
                    float r2 = cars[j].getSize().x;

                    if (CheckCollisionCircles(p1, r1, p2, r2)) {
                        cars[i].bcollide(Vector2{ p1.x + (p1.x - p2.x) / 2, p1.y + (p1.y - p2.y) / 2 }, 90 + atan2(p2.y - p1.y, p2.x - p1.x));
                        cars[j].bcollide(Vector2{ p1.x + (p1.x - p2.x) / 2, p1.y + (p1.y - p2.y) / 2 }, 90 + atan2(p1.y - p2.y, p1.x - p2.x));
                    }
                }
            }

            for (int i = 0; i < carnum; i++) {
                walls.collisions(cars[i]);
            }

            dt -= smallstep;
        }

        BeginDrawing();
        ClearBackground(Color{ 255, 255, 255 });

        DrawTextureRec(map, Rectangle{ 0, 0, (float)map.width, (float)-map.height }, Vector2Zero(), WHITE);
        for (int i = 0; i < carnum; i++) {
            cars[i].draw();
        }

        if (!saved) {
            savebutton.draw();
        }

        //DrawText(TextFormat("%i", fps), 5, 5, 15, BLACK);
        DrawFPS(0, 0);

        EndDrawing();
    }
}

void game::runmapcreation()
{
    SetWindowTitle("Creating map");
    std::vector<std::vector<Vector2>> points{ std::vector<Vector2> {} };

    int k = 0;

    float dt = 0;

    bool game = true;
    while (game) {
        if (WindowShouldClose()) {
            game = false;
        }

        dt += 1 / (float)GetFPS();
        //stop dt being crazy
        if (dt > 1) { dt = 1; };

        if (IsMouseButtonPressed(0)) {
            points[k].push_back(GetMousePosition());
        }
        else if (IsMouseButtonDown(0)) {
            Vector2 dfromlast = { points[k][points[k].size() - 1].x - GetMousePosition().x, points[k][points[k].size() - 1].y - GetMousePosition().y };
            float len = sqrt(dfromlast.x * dfromlast.x + dfromlast.y * dfromlast.y);
            if (len >= 15) {
                points[k].push_back(GetMousePosition());
            }
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            game = false;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            k++;
            points.push_back(std::vector<Vector2> {});
        }

        BeginDrawing();
        ClearBackground(Color{ 200, 200, 200 });


        for (int i = 0; i < points.size(); i++) {
            for (int j = 0; j < points[i].size(); j++) {
                Color colour = RED;
                float size = 3;
                if (i == k) {
                    colour = BLACK;
                    if (j == 0) {
                        size = 5;
                    }
                }
                DrawCircleV(points[i][j], size, colour);
            }
        }

        DrawFPS(0, 0);

        EndDrawing();
    }

    int p = 0;
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points[i].size(); j++) {
            p++;
        }
    }

    if (p != 0) {
        std::ofstream levelfile("level.txt");
        if (levelfile.is_open()) {
            levelfile.clear();
            for (int i = 0; i < points.size(); i++) {
                for (int j = 0; j < points[i].size(); j++) {
                    levelfile << "{" << points[i][j].x << "," << points[i][j].y << "}";
                }
                levelfile << "\n";
            }
        }
        else {
            std::cout << "file wont open\n";
        }
    }
}

int game::runmenu() {
    std::cout << fs::current_path().string() << "\n";
    int pathlen = (int)fs::current_path().string().length() + 1;

    std::vector<int> levels;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        std::string file = entry.path().string().erase(0, pathlen);
        if (file.length() > 9) {
            if (file.substr(0, 5) == "level") {
                file = file.erase(file.length() - 4, file.length() - 1);
                file = file.erase(0, 5);
                levels.push_back(std::stoi(file));
            }
        }
    }

    std::vector<button> levelbuttons = {};

    for (int i = 0; i < levels.size(); i++) {
        levelbuttons.push_back(button(Vector2{ 0, 50.0f * i }, Vector2{ 100, 40 }, RED, std::string("Level ") + std::to_string(levels[i])));
        std::cout << std::string("Level ") + std::to_string(levels[i]) << "\n";
    }

    bool game = true;
    while (game) {
        if (WindowShouldClose()) {
            game = false;
        }

        for (int i = 0; i < levelbuttons.size(); i++) {
            if (levelbuttons[i].pressed()) {
                return levels[i];
            }
        }

        BeginDrawing();
        ClearBackground(Color{ 200, 200, 200 });

        for (int i = 0; i < levelbuttons.size(); i++) {
            levelbuttons[i].draw();
        }

        DrawFPS(0, 0);

        EndDrawing();
    }

    return -1;
}

Vector2 game::getdirection(int left, int right, int forward, int back) {
    Vector2 direction = Vector2{0, 0};
    if (IsKeyDown(left)) { direction.x -= 1; }
    if (IsKeyDown(right)) { direction.x += 1; }
    if (IsKeyDown(forward)) { direction.y += 1; }
    if (IsKeyDown(back)) { direction.y -= 1; }

    return direction;
}

Texture game::drawmap(wallloop walls, int width, int height) {
    RenderTexture2D map = LoadRenderTexture(width, height);
    BeginTextureMode(map);

        walls.draw();

    EndTextureMode();

    Texture tex = map.texture;

    return tex;
}