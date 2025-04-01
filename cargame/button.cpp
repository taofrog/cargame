#include "button.h"

button::button(Vector2 pos, Vector2 size, Color colour, std::string label) {
	box = { pos.x, pos.y, size.x, size.y };
	col = colour;
	text = label;
}

bool button::pressed() {
	if (IsMouseButtonPressed(0)) {
		if (CheckCollisionPointRec(GetMousePosition(), box)) {
			return true;
		}
	}
	return false;
}

void button::draw() {
	DrawRectangleRec(box, col);
	if (col.r != 0 || col.g != 0 || col.b != 0) {
		DrawText(text.c_str(), box.x, box.y, box.height, BLACK);
	}
	else {
		DrawText(text.c_str(), box.x, box.y, box.height, WHITE);
	}
}