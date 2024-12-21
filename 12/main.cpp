#include <regex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <thread>
#include <queue>
#include <set>
#include <chrono>
#include "../lib/utils.h"
#include "../lib/timer.h"

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Gardener {
	StringVector Lines;
	std::vector<std::vector<Vec2>> plots;
	long MapWidth;
	long MapHeight;
	std::vector<std::array<int, 2>> Directions = {std::array<int, 2>{-1, 0}, std::array<int, 2>{1, 0}, std::array<int, 2>{0, -1}, std::array<int, 2>{0, 1}};
	std::vector<std::array<int, 6>> CornerDirections = {
		std::array<int, 6>{-1, 0, -1, -1, 0, -1}, // Top left
		std::array<int, 6>{1, 0, 1, -1, 0, -1}, // Top Right
		std::array<int, 6>{-1, 0, -1, 1, 0, 1}, // Bottom Left
		std::array<int, 6>{1, 0, 1, 1, 0, 1}, // Bottom right
	};
	std::vector<std::vector<bool>> visited;
	public:
		Gardener(std::string *filePath) {
			Lines = Utils::loadFile(*filePath);
			MapWidth = Lines[0].size();
			MapHeight = Lines.size();
			std::vector<std::vector<bool>> temp(MapHeight, std::vector<bool>(MapWidth, false));
			visited = temp;
			FindPlots();
		}

		bool IsValid(Vec2 *pos) {
			return pos->x >= 0 && pos->x < MapWidth && pos->y >= 0 && pos->y < MapHeight; 
		}

		void FindPlot(Vec2 *pos,  std::vector<Vec2> *plot,  char curChar) {
			if (!IsValid(pos) || visited[pos->y][pos->x] == true) return;
			if (Lines[pos->y][pos->x] != curChar) return;

			visited[pos->y][pos->x] = true;
			plot->push_back(*pos);
			for (const auto &dir : Directions) {
				Vec2 newPos(pos->x + dir[0], pos->y + dir[1]);
				FindPlot(&newPos, plot, curChar); // Recursively explore the plot
			}
		}

		 void FindPlots() {
			for (int i = 0; i < MapHeight; i++) {
				for (int j = 0; j < MapWidth; j++) {
					if (!visited[i][j]) {
						char curChar = Lines[i][j];
						std::vector<Vec2> plot;
						Vec2 temp(j, i);
						FindPlot(&temp, &plot, curChar);
						if (!plot.empty()) {
							plots.push_back(plot);
						}
					}
				}
			}
		}

		long GetPerimiter(std::vector<Vec2> *plot) {
			long perimeter = 0;
			for (auto &pos : *plot) {
				for (auto &dir : Directions) {
					Vec2 neighbor(pos.x + dir[0], pos.y + dir[1]); 
					if (!IsValid(&neighbor) || Lines[neighbor.y][neighbor.x] != Lines[pos.y][pos.x]) {
						perimeter++;
					}
				}
			}

			return perimeter;
		}

		bool IsValidAndNotSame(Vec2 &vec, char same) {
			return !IsValid(&vec) || Lines[vec.y][vec.x] != same;
		}

		long GetSides(std::vector<Vec2> *plot) {
			std::vector<Vec2> copyPlot(*plot);
			long corners = 0;
			for (auto &pos : *plot) {
				char same = Lines[pos.y][pos.x]; 
				for (auto &dir : CornerDirections) {
					Vec2 left(pos.x + dir[0], pos.y + dir[1]);
					Vec2 center(pos.x + dir[2], pos.y + dir[3]);
					Vec2 right(pos.x + dir[4], pos.y + dir[5]);
					if (IsValidAndNotSame(left, same) && IsValidAndNotSame(right,same)) {
						corners++;
					} else if((IsValid(&left) && Lines[left.y][left.x] == same) && IsValidAndNotSame(center, same) && (IsValid(&right) && Lines[right.y][right.x] == same)) {
						corners++;
					}
				}
			}
			return corners;
		}

		long FencingPrice() {
			long total = 0;
			for(auto plot : plots) {
				long area = plot.size();
				// printf("Area: %ld\n", area);
				//long perimiter = GetPerimiter(&plot); // Part 1
				// printf("Perimiter: %ld\n", perimiter);
				long sides = GetSides(&plot); // Part 2
				// printf("Sides: %ld\n\n", sides);
				total += area * sides;
			}
			return total;
		}
};

int main(int argc, char *argv[]) {
	Timer timer = Timer();
	timer.Start();
	std::string filePath = "testinput.txt";
	if(argc > 1) {
		filePath = argv[1];
	}
	Gardener g = Gardener(&filePath);
	long res = g.FencingPrice();
	printf("Fencing price: %ld\n", res);
	timer.End();
	return 0;
}