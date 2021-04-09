#pragma once
#include "Include.h"

using namespace sf;

inline float dist(float x1, float y1, float x2, float y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}
inline float dist(Vector2f v1, Vector2f v2) {
	return dist(v1.x, v1.y, v2.x, v2.y);
}
inline float sqr(float f) {
	return f * f;
}
inline float cube(float f) {
	return f * f * f;
}