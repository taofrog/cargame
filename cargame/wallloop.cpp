#include "wallloop.h"

wallloop::wallloop(std::string args) {
	addloop(args);
}

wallloop::wallloop(std::string args1, std::string args2) {
	addloop(args1, args2);
}

void wallloop::addloop(std::string args) {
	// convert string with format {x,y}{x,y} to vector of vector2s
	std::vector<Vector2> points;

	std::string sx, sy = "";
	bool read = false;
	bool readx = false;
	for (int i = 0; i < args.size(); i++) {
		if (read) {
			if (readx) {
				if (args[i] == 44) { // ,
					readx = false;
				}
				else {
					sx.push_back(args[i]);
				}
			}
			else {
				if (args[i] == 125) { // }
					read = false;
					// write final values
					int x = 0;
					int y = 0;
					for (int j = 0; j < sx.size(); j++) {
						x += (sx[sx.size() - j - 1] - 48) * pow(10, j);
					}
					for (int j = 0; j < sy.size(); j++) {
						y += (sy[sy.size() - j - 1] - 48) * pow(10, j);
					}

					points.push_back(Vector2{ (float)x, (float)y });

					sx.clear();
					sy.clear();
				}
				else {
					sy.push_back(args[i]);
				}
			}
		}
		else if (args[i] == 123) { // {
			read = true;
			readx = true;
		}
	}

	//convert vector of vectors to vector of obstacles
	for (int i = 0; i < points.size(); i++) {
		walls.push_back(obstacle(points[i], points[(i + 1) % points.size()]));
	}
}

void wallloop::collisions(vehicle &car) {
	for (int i = 0; i < walls.size(); i++) {
		//check collisions
		
		Rectangle r = walls[i].getRecBounds();
		//if (CheckCollisionCircleRec(car.getPosition(), car.getSize().x, r)) {
			//std::cout << "bollocks\n";
			Vector2 p = walls[i].findClosestPoint(car.getPosition());

			if (CheckCollisionPointCircle(p, car.getPosition(), car.getSize().x)) {
				car.collide(p, walls[i].angle());
			}
		//}
	}
}


void wallloop::draw() {
	std::vector<std::vector<Vector2>> polys = { std::vector<Vector2>() };
	int c = 0; // current polygon
	for (int i = 0; i < walls.size(); i++) {
		Vector2 a = walls[i].getp1();
		Vector2 b = walls[i].getp2();
		polys[c].push_back(a);
		for (int j = 0; j < polys[c].size(); j++) {
			Vector2 p = polys[c][j];
			if (p.x == b.x && p.y == b.y) {
				polys.push_back(std::vector<Vector2>());
				c++;
			}
		}
	}
	/*
	for (int i = 0; i < walls.size(); i++) {
		Rectangle r = walls[i].getRecBounds();

		Vector2 b = walls[i].getp2();
		Vector2 c = walls[i].getp1();

		for (int j = 0; j < 4; j++) {
			int e = j % 2;
			int e2 = j * 0.5;
			Vector2 a = { r.x + r.width * e, r.y + r.height * e2 };
			
			if (a.x != b.x || a.y != b.y || a.x != c.x || a.y != c.y) {
				Vector3 a1 = { b.x - a.x, b.y - a.y, 0 };
				Vector3 b1 = { c.x - a.x, c.y - a.y, 0 };

				float winding = Vector3CrossProduct(a1, b1).z;
				if (winding < 0) {
					DrawTriangle(a, b, c, GRAY);
				}
			}
		}
	}*/

	for (int y = 0; y < 1000; y++) {
		bool inside = false;
		for (int x = 0; x < 1500; x++) {
			for (int i = 0; i < walls.size(); i++) {
				Rectangle r = walls[i].getRecBounds();
				r.x -= 2;
				r.y -= 2;
				r.width += 4;
				r.height += 4;
				if (CheckCollisionPointRec(Vector2{ (float)x, (float)y }, r)) {
					//DrawPixel(x, y, Color{ 255, 0, 0, 150 });
					Vector2 a = { x, y };
					Vector2 b = walls[i].getp1();
					Vector2 c = walls[i].getp2();

					Vector3 a1 = { b.x - a.x, b.y - a.y, 0 };
					Vector3 b1 = { c.x - a.x, c.y - a.y, 0 };

					float winding = Vector3CrossProduct(a1, b1).z;
					if (winding > 0) {
						inside = true;
					}
					else {
						inside = false;
					}
				}
				if (inside) {
					DrawPixel(x, y, GRAY);
				}
			}
		}
	}

	for (int i = 0; i < walls.size(); i++) {
		walls[i].draw();
	}
}