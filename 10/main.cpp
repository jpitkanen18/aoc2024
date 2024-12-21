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
#include <queue>
#include <set>
#include "../lib/utils.h"

class Pathfinder {
	StringVector Lines;
	std::vector<Vec2> ZeroIndices;
	int MapWidth;
	int MapHeight;
	std::vector<std::array<int, 2>> Directions;
	public:
		Pathfinder(std::string filePath = "input.txt") {
			Lines = Utils::loadFile(filePath);
			MapWidth = Lines[0].size();
			MapHeight = Lines.size();
			Directions = {std::array<int, 2>{-1, 0}, std::array<int, 2>{1, 0}, std::array<int, 2>{0, -1}, std::array<int, 2>{0, 1}};
			ZeroIndices = FindZeroIndices();
		}

		bool isValid(Vec2 *pos) {
			return pos->x >= 0 && pos->x < MapWidth && pos->y >= 0 && pos->y < MapHeight; 
		}

		std::vector<Vec2> FindZeroIndices() {
			std::vector<Vec2> zeroIndices;
			for (int i = 0; i < Lines.size(); i++) {
				std::string line = Lines[i];
				for (int j = 0; j < line.size(); j++) {
					if (line.at(j) - 48 == 0) {
						zeroIndices.push_back(Vec2(j, i));
					}
				}
			}
			return zeroIndices;
		}

		long TrailHeadScore(Vec2 *start, long prev,long trail, long *nextId, std::vector<std::vector<bool>> *maxHeightVisited, std::vector<std::vector<Vec2>> *distinctTrails) {
			long score = 0;
			long curr = Lines[start->y][start->x] - 48;
			if(curr - prev == 1) {
				if(trail >= distinctTrails->size()) {
					for(int i = distinctTrails->size(); i < trail + 1; i++) {
						distinctTrails->push_back(std::vector<Vec2>());
					}
				}
				if(isValid(start))
					distinctTrails->at(trail).push_back(Vec2(start->x, start->y));
				if(curr == 9) {
					if(!maxHeightVisited->at(start->y).at(start->x)) {
						score = 1;
						maxHeightVisited->at(start->y).at(start->x) = true;
					}
				} else {
					for(auto dir : Directions) {
						struct Vec2 *newPos = (Vec2*) malloc(sizeof(Vec2));
						newPos->x = start->x + dir[0];
						newPos->y = start->y + dir[1];
						*nextId += 1;
						if(isValid(newPos)) {
							score += TrailHeadScore(newPos, curr, *nextId, nextId, maxHeightVisited, distinctTrails);
						}
					}
				}
			}
			return score;
		}

		std::string BuildTrailString(std::vector<Vec2> *trail) {
			std::string s;
			for(auto t : *trail) {
				s.append(std::string(1, (Lines[t.y][t.x])));
			}
			return s;
		}

		std::tuple<long, long> TrailheadsSum() {
			std::vector<std::vector<Vec2>> distinctTrails;
			long sum = 0;
			long nextId = 0;
			for(auto zero : ZeroIndices) {
				std::vector<std::vector<bool>> maxHeightVisited(MapHeight, std::vector<bool>(MapWidth, false));
				sum += TrailHeadScore(&zero, -1, 0, &nextId, &maxHeightVisited, &distinctTrails); 
			}
			long distinctScore = 0;
			std::vector<std::string> distinctStrings;
			for(auto dist : distinctTrails) {
				std::string dstr = BuildTrailString(&dist);
				bool includes = false;
				for(auto str : distinctStrings) {
					if(dstr == str) {
						includes = true;
						break;
					}
				}
				if(dstr.size() > 0 && includes == false && Lines[dist.back().y][dist.back().x] - 48 == 9 ) {
					distinctScore++;
				}
			}
			return std::tuple<long, long>(sum, distinctScore);
		}
};

int main(int argc, char *argv[]) {
	std::string filePath = "input.txt";
	if(argc > 1) {
		filePath = argv[1];
	}
	Pathfinder *pathFinder = new Pathfinder(std::string(filePath));
	std::tuple<long, long> sums = pathFinder->TrailheadsSum();
	printf("Sum: %ld\n", std::get<0>(sums));
	printf("Dist scores: %ld\n", std::get<1>(sums));
	printf("Filepath: %s\n", filePath.c_str());
	delete pathFinder;
	return 0;
}