#include "game.h"

void game::savelevel() {
    std::vector<int> levels = levellist(); // list of level ids

    // find lowest unused number
    int freefile = (int)levels.size();
    int prev = -1;
    for (int i = 0; i < levels.size(); i++) {
        if (i != levels[i]) {
            freefile = i;
            break;
        }
    }

    // open current level file for reading and new file for writing
    std::ifstream currentfile = std::ifstream("level.txt");
    std::string linedata;

    std::string newfilename = std::string("level") + std::to_string(freefile) + std::string(".txt");
    std::ofstream newfile(newfilename);

    // write current file to new file, line by line
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

    BeginDrawing();
    ClearBackground(WHITE);
    DrawText("Loading level data", 300, 300, 100, BLACK);
    EndDrawing();

    const int carnum = 4; //PLAYER COUNT (local)

    vehicle cars[carnum] = { vehicle(Vector2{ 450, 350 }, Vector2{ 10, 10 }),
                       vehicle(Vector2{ 450, 400 }, Vector2{ 10, 10 }),
                       vehicle(Vector2{ 450, 450 }, Vector2{ 10, 10 }),
                       vehicle(Vector2{ 450, 500 }, Vector2{ 10, 10 }) };
    cars[1].col = BLUE;
    cars[2].col = GREEN;
    cars[3].col = YELLOW;

    button savebutton(Vector2{ 1400, 20 }, Vector2{ 50, 20 }, RED, "Save");

    // CREATING LEVEL ------------------------------------------------------------------------------------------
    wallloop walls(""); //create as empty

    //  load level from file
    bool saved = true;
    std::ifstream levelfile = std::ifstream();
    if (level < 0) {
        saved = false;
        levelfile.open("level.txt"); // open prev unsaved if level is not supplied, possibly newly created level
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

    BeginDrawing();
    ClearBackground(WHITE);
    for (int i = 0; i < walls.getWalls().size(); i++) {
        walls.getWalls()[i].draw(RED);
    }
    DrawText("Rendering level", 300, 300, 100, BLACK);
    EndDrawing();

    Texture map = drawmap(walls, swidth, sheight); // turn the level into a texture that can be drawn each frame. terribly optimised, for large levels this can take multiple minutes

    const int maxdecocount = 1200;
    std::vector<Vector2> decopoints = gendeco(map, maxdecocount); // create random points for deco, not on the road

    std::vector<decoration> deco = std::vector<decoration>();
    for (int i = 0; i < decopoints.size(); i++){
        deco.push_back(decoration(decopoints[i], 40*i/maxdecocount + 50));
        deco[i].lifetime = i;
    }
    //  ------------------------------------------------------------------------------------------

    Shader fader = LoadShader(0, "shaders/fade.glsl");
    Shader plantshader = LoadShader("shaders/plantv.glsl", "shaders/plantf.glsl");
    int p1uniform = GetShaderLocation(plantshader, "p1pos");
    int p2uniform = GetShaderLocation(plantshader, "p2pos");
    int p3uniform = GetShaderLocation(plantshader, "p3pos");
    int p4uniform = GetShaderLocation(plantshader, "p4pos");

    RenderTexture2D trailbuffer = LoadRenderTexture(1500, 1000);

    float dt = 0;

    bool game = true;
    while (game) {
        if (WindowShouldClose()) {
            game = false;
        }

        float deltatime = 1 / (float)GetFPS();

        dt += deltatime;
        //stop dt being crazy
        if (dt > 1) { dt = 1; };

        if (!saved && savebutton.pressed()) {
            saved = true;
            savelevel();
        }

        // user input
        Vector2 directions[] = { getdirection(KEY_Q, KEY_E, KEY_W),
                                 getdirection(KEY_I, KEY_P, KEY_O),
                                 getdirection(KEY_B, KEY_M, KEY_N),
                                 getdirection(KEY_Z, KEY_C, KEY_X) };

        //fixed update
        float smallstep = 0.004f;
        while (dt >= smallstep) {
            //update positions
            for (int i = 0; i < carnum; i++) {
                cars[i].updateMovement(directions[i], smallstep);
            }
            //update physics
            for (int i = 0; i < carnum; i++) {
                cars[i].updatePhysics(smallstep);
            }
            // collide cars with each other
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
            // collide cars with walls
            for (int i = 0; i < carnum; i++) {
                walls.collisions(cars[i]);
            }

            dt -= smallstep;
        }

        for (int i = 0; i < deco.size(); i++) {
            deco[i].lifetime += (float)i / 10000 + deltatime;
        }

        Vector2 p1 = cars[0].getPosition();
        Vector2 p2 = cars[1].getPosition();
        Vector2 p3 = cars[2].getPosition();
        Vector2 p4 = cars[3].getPosition();
        SetShaderValueV(plantshader, p1uniform, &p1, RL_SHADER_UNIFORM_VEC2, 1);
        SetShaderValueV(plantshader, p2uniform, &p2, RL_SHADER_UNIFORM_VEC2, 1);
        SetShaderValueV(plantshader, p3uniform, &p3, RL_SHADER_UNIFORM_VEC2, 1);
        SetShaderValueV(plantshader, p4uniform, &p4, RL_SHADER_UNIFORM_VEC2, 1);

        BeginTextureMode(trailbuffer); {
            BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
            DrawTextureRec(map, Rectangle{ 0, 0, (float)map.width, (float)-map.height }, Vector2Zero(), { 255, 255, 255, 255 });
            // draw cars
            for (int i = 0; i < carnum; i++) {
                cars[i].draw();
            }
            EndBlendMode();
        }
        EndTextureMode();

        BeginDrawing(); {
            ClearBackground(Color{ 255, 255, 255 });

            // draw map
            DrawTextureRec(map, Rectangle{ 0, 0, (float)map.width, (float)-map.height }, Vector2Zero(), { 255, 255, 255, 255 });

            DrawTextureRec(trailbuffer.texture, Rectangle{ 0, 0, (float)trailbuffer.texture.width, (float)-trailbuffer.texture.height }, Vector2Zero(), { 255, 255, 255, 255 });

            //draw decorations
            BeginShaderMode(plantshader);
            for (int i = 0; i < deco.size(); i++) {
                deco[i].draw(plantshader);
            }
            EndShaderMode();
            
            // draw savebutton
            if (!saved) {
                savebutton.draw();
            }

            DrawFPS(0, 0);

            EndDrawing();
        }
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
            if (len >= 30) {
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
    int pathlen = (int)fs::current_path().string().length() + 1;

    std::vector<int> levels = levellist();

    std::vector<button> levelbuttons = {};

    for (int i = 0; i < levels.size(); i++) {
        levelbuttons.push_back(button(Vector2{ (float)(200 * (int)(i/20)), (50.0f * (i%20))}, Vector2{190, 45}, RED, std::string("Level ") + std::to_string(levels[i])));
        std::cout << std::string("Level ") + std::to_string(levels[i]) << "\n";
    }

    decoration plant = decoration(Vector2{500, 500}, 50);

    Shader plantshader = LoadShader("shaders/plantv.glsl", "shaders/plantf.glsl");

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

        plant.lifetime += 0.016;

        BeginDrawing();
        ClearBackground(Color{ 200, 200, 200 });

        for (int i = 0; i < levelbuttons.size(); i++) {
            levelbuttons[i].draw();
        }

        BeginShaderMode(plantshader);
            plant.draw(plantshader);
        EndShaderMode();

        DrawFPS(0, 0);

        EndDrawing();
    }

    return -1;
}

std::vector<int> game::levellist() {
    int pathlen = (int)fs::current_path().string().length() + 1;

    std::vector<int> levels;

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        std::string file = entry.path().string().erase(0, pathlen);
        if (file.length() > 9) {
            if (file.substr(0, 5) == "level") {
                file = file.erase(file.length() - 4, file.length() - 1);
                file = file.erase(0, 5);
                if (levels.size() > 0) {
                    if (std::stoi(file) > levels[levels.size() - 1]) {
                        levels.push_back(std::stoi(file));
                    }
                    else {
                        int i = 1;
                        while (i < levels.size() - 1 && std::stoi(file) < levels[levels.size() - i - 1]) {
                            i++;
                        }
                        std::vector<int>::iterator it = levels.end() - i;
                        levels.insert(it, std::stoi(file));
                    }
                }
                else {
                    levels.push_back(std::stoi(file));
                }
            }
        }
    }

    return levels;
}

Vector2 game::getdirection(int left, int right, int forward, int back) {
    Vector2 direction = Vector2{0, 0};
    if (IsKeyDown(left)) { direction.x -= 1; }
    if (IsKeyDown(right)) { direction.x += 1; }
    if (IsKeyDown(forward)) { direction.y += 1; }
    if (IsKeyDown(back)) { direction.y -= 1; }

    return direction;
}

Texture game::drawmap(wallloop walls, int width, int height) { // one time use function to draw the map to a texture
    RenderTexture2D map = LoadRenderTexture(width, height);
    BeginTextureMode(map);
        // clear with green and draw the map
        ClearBackground(DARKGREEN);
        walls.draw();
    EndTextureMode();

    Shader blur = LoadShader(0, "shaders/blur.glsl"); // loading shader for "antialiasing"

    BeginTextureMode(map);
        // redraw the texture to itself using the blur shader and drawing upside down to fix gl flipping issue
        BeginShaderMode(blur);
            DrawTextureRec(map.texture, Rectangle{0, 0, (float)map.texture.width, (float)-map.texture.height}, Vector2Zero(), WHITE);
        EndShaderMode();
    EndTextureMode();;

    return map.texture;
}

std::vector<Vector2> game::gendeco(Texture map, int maxcount) { // create a bunch of random points that can be used to render decorations
    Image mapimg = LoadImageFromTexture(map);
    ImageFlipVertical(&mapimg);
    Color* colours = LoadImageColors(mapimg);

    std::vector<Vector2> points = std::vector<Vector2>();
    for (int i = 0; i < maxcount; i++) {
        int x = GetRandomValue(1, 1499);
        int y = GetRandomValue(1, 999);

        int index = (y * mapimg.width) + x;

        if (colours[index].r == DARKGREEN.r && colours[index].g == DARKGREEN.g && colours[index].b == DARKGREEN.b) {
            points.push_back(Vector2{ (float)x, (float)y });
        }
    }

    return points;
}