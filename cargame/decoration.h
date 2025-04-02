#pragma once
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

class decoration
{
private:
	Vector2 pos;
	float radius;

	void DrawTriangleStrip3DNormals(const Vector3* points, int pointCount, Color color);

	Vector3 calculatenormal(Vector3 p1, Vector3 p2, Vector3 p3);
public:
	float lifetime = 0;

	decoration(Vector2 position, float size);

	void draw(Shader shader);
};

