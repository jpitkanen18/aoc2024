#pragma once
#include <vector>
#include <string>

using StringVector = std::vector<std::string>;

struct Vec2 {
	long x, y;
	Vec2(long x, long y) : x(x), y(y) {}

	friend bool operator==(Vec2 lhs, Vec2 rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	Vec2& operator+=(const Vec2& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};
