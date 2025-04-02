#include "decoration.h"

decoration::decoration(Vector2 position, float size) {
	pos = position;
	radius = size;
}

void decoration::draw(Shader shader) {

    DrawCircleGradient(pos.x, pos.y, radius, Color{ 0, 0, 0, 30 }, Color{ 0, 0, 0, 0 });

    Vector3 points[4] ={{pos.x, pos.y, 1},
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20)),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, -radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20)),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius, 0, 1}, { 0, 0, 1 }, 2 * sin(lifetime / 20 - 0.08)) };

    Vector3 points1[4] = { {pos.x, pos.y, 1},
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 2 * PI/5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, -radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 2 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius, 0, 1}, { 0, 0, 1 }, 2 * sin(lifetime / 20 - 0.08) + 2 * PI / 5) };

    Vector3 points2[4] = { {pos.x, pos.y, 1},
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 4 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, -radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 4 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius, 0, 1}, { 0, 0, 1 }, 2 * sin(lifetime / 20 - 0.08) + 4 * PI / 5) };

    Vector3 points3[4] = { {pos.x, pos.y, 1},
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 6 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, -radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 6 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius, 0, 1}, { 0, 0, 1 }, 2 * sin(lifetime / 20 - 0.08) + 6 * PI / 5) };

    Vector3 points4[4] = { {pos.x, pos.y, 1},
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 8 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius / 2, -radius / 6, 3}, {0, 0, 1}, 2 * sin(lifetime / 20) + 8 * PI / 5),
                        Vector3{pos.x, pos.y, 1} + Vector3RotateByAxisAngle({radius, 0, 1}, { 0, 0, 1 }, 2 * sin(lifetime / 20 - 0.08) + 8 * PI / 5) };
	Vector3* pointptr  = &points[0];
    Vector3* pointptr1 = &points1[0];
    Vector3* pointptr2 = &points2[0];
    Vector3* pointptr3 = &points3[0];
    Vector3* pointptr4 = &points4[0];

	DrawTriangleStrip3DNormals(pointptr,  4, Color{ 28, 180, 41, 255});
    DrawTriangleStrip3DNormals(pointptr1, 4, Color{ 28, 180, 41, 255 });
    DrawTriangleStrip3DNormals(pointptr2, 4, Color{ 28, 180, 41, 255 });
    DrawTriangleStrip3DNormals(pointptr3, 4, Color{ 28, 180, 41, 255 });
    DrawTriangleStrip3DNormals(pointptr4, 4, Color{ 28, 180, 41, 255 });
}

Vector3 decoration::calculatenormal(Vector3 p1, Vector3 p2, Vector3 p3) {
    if (p1 == p2 && p1 == p3) {
        return Vector3{ 0, 0, 0 };
    }
    Vector3 U = p2 - p1;
    Vector3 V = p3 - p1;

    return Vector3Normalize(Vector3CrossProduct(U, V));
}

void decoration::DrawTriangleStrip3DNormals(const Vector3* points, int pointCount, Color color)
{
    if (pointCount < 3) return; // Security check

    rlBegin(RL_TRIANGLES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    for (int i = 2; i < pointCount; i++)
    {
        if ((i % 2) == 0)
        {
            Vector3 normal = calculatenormal(points[i], points[i - 1], points[i - 2]);
            rlNormal3f(normal.x, normal.y, normal.z);
            rlVertex3f(points[i].x, points[i].y, points[i].z);

            rlNormal3f(normal.x, normal.y, normal.z);
            rlVertex3f(points[i - 2].x, points[i - 2].y, points[i - 2].z);

            rlNormal3f(normal.x, normal.y, normal.z);
            rlVertex3f(points[i - 1].x, points[i - 1].y, points[i - 1].z);
        }
        else
        {
            Vector3 normal = calculatenormal(points[i], points[i - 2], points[i - 1]);
            rlNormal3f(normal.x, normal.y, normal.z);
            rlVertex3f(points[i].x, points[i].y, points[i].z);

            rlNormal3f(normal.x, normal.y, normal.z);
            rlVertex3f(points[i - 1].x, points[i - 1].y, points[i - 1].z);

            rlNormal3f(normal.x, normal.y, normal.z);
            rlVertex3f(points[i - 2].x, points[i - 2].y, points[i - 2].z);
        }
    }
    rlEnd();
}
