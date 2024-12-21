#pragma once
#include <vector>
#include <string>

using StringVector = std::vector<std::string>;

struct Vec2 {
	long x, y;
	Vec2(long x, long y) : x(x), y(y) {}
};
