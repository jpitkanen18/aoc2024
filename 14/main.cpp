#include <regex>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <climits>
#include <mutex>
#include <thread>
#include <queue>
#include <set>
#include <chrono>
#include <atomic>
#include "../lib/utils.h"
#include "../lib/timer.h"
#include <unistd.h>

using BotLocation = std::pair<Vec2, Vec2>;

class BotSimulator {
	StringVector Lines;
	StringVector CurrentMap;
	std::vector<BotLocation> BotLocations;
	std::vector<BotLocation> CurrentBotLocations;
	long SecondsElapsed = 0;
	int MapWidth = 101;
	int MapHeight = 103;
	std::vector<std::vector<bool>> Visited;
	std::vector<std::array<int, 2>> Directions = {std::array<int, 2>{-1, 0}, std::array<int, 2>{1, 0}, std::array<int, 2>{0, -1}, std::array<int, 2>{0, 1}};
	public:
		BotSimulator(StringVector &input, bool smallMap = false) {
			if(smallMap) {
				MapWidth = 11;
				MapHeight = 7;
			}
			Lines = input;
			BotLocations = ParseInput();
			CurrentBotLocations = BotLocations;
			PrintCurrentBotLocations();
			std::vector<std::vector<bool>> temp(MapHeight, std::vector<bool>(MapWidth, false));
			Visited = temp;
		}

		bool IsValid(Vec2 *pos) {
			return pos->x >= 0 && pos->x < MapWidth && pos->y >= 0 && pos->y < MapHeight; 
		}

		void FindArea(Vec2 *pos, std::vector<Vec2> *plot, char curChar) {
			if (!IsValid(pos) || Visited[pos->y][pos->x] == true) return;
			if (CurrentMap[pos->y][pos->x] != curChar) return;

			Visited[pos->y][pos->x] = true;
			plot->push_back(*pos);
			for (const auto &dir : Directions) {
				Vec2 newPos(pos->x + dir[0], pos->y + dir[1]);
				FindArea(&newPos, plot, curChar);
			}
		}

		 void FindPlots() {
			std::vector<std::vector<bool>> temp(MapHeight, std::vector<bool>(MapWidth, false));
			Visited = temp;
			for (int i = 0; i < MapHeight; i++) {
				for (int j = 0; j < MapWidth; j++) {
					if (CurrentMap[i][j] == 'B' && !Visited[i][j]) {
						std::vector<Vec2> plot;
						Vec2 temp(j, i);
						FindArea(&temp, &plot, 'B');
						if (plot.size() > 20) {
							PrintMap();
							printf("🎄Christmas tree🎄 found YAY: %ld\n", SecondsElapsed);
							throw std::exception();
						}
					}
				}
			}
		}

		std::vector<BotLocation> ParseInput() {
			std::vector<BotLocation> bv;
			std::array<std::regex, 3> regexes = {
				std::regex("p=\\d+,\\d+"),
				std::regex("v=-{0,1}\\d+,-{0,1}\\d+"),
			};
			for(auto l : Lines) {
				std::vector<Vec2> vecs;
				std::stringstream iss(l);
				std::string s;
				int regexIter = 0;
				while(std::getline(iss, s, ' ')) {
					vecs.push_back(Utils::Vec2FromRegex(s, regexes[regexIter]));
					regexIter++;
				}
				bv.push_back(std::pair(vecs[0], vecs[1]));
			}
			return bv;
		}

		void Simulate(bool print = false, bool part2 = false) {
			for(auto& cb : CurrentBotLocations) {
				cb.first.x += cb.second.x;
				cb.first.y += cb.second.y;
				if(cb.first.x >= MapWidth)
					cb.first.x -= MapWidth;
				if(cb.first.x < 0)
					cb.first.x += MapWidth;
				if(cb.first.y >= MapHeight)
					cb.first.y -= MapHeight;
				if(cb.first.y < 0)
					cb.first.y += MapHeight;
			}
			if(print)
				PrintCurrentBotLocations();
			StringVector bm = BlankMap();
			for(auto botLoc : CurrentBotLocations) {
				Vec2 pos = botLoc.first;
				bm[pos.y][pos.x] = 'B';
			}
			CurrentMap = bm;
			SecondsElapsed++;
			if(part2)
				FindPlots();
		}

		long GetTotalSafety(bool print = false) {
			long centerY = MapHeight / 2;
			long centerX = MapWidth / 2;
			if(print) {
				printf("centerX: %ld\n", centerX);
				printf("centerY: %ld\n", centerY);
			}
			long quadrantA = 0;
			long quadrantB = 0;
			long quadrantC = 0;
			long quadrantD = 0;
			for(auto cb : CurrentBotLocations) {
				Vec2 pos = cb.first;
				if(pos.x == centerX || pos.y == centerY)
					continue;
				if(pos.x >= 0 && pos.x < centerX && pos.y >= 0 && pos.y < centerY)
					quadrantA++;
				if(pos.x > centerX && pos.x < MapWidth && pos.y >= 0 && pos.y < centerY)
					quadrantB++;
				if(pos.x >= 0 && pos.x < centerX && pos.y > centerY && pos.y < MapHeight)
					quadrantC++;
				if(pos.x > centerX && pos.x < MapWidth && pos.y > centerY && pos.y < MapHeight)
					quadrantD++;
			}
			if(print){
				printf("quadrantA: %ld\n", quadrantA);
				printf("quadrantB: %ld\n", quadrantB);
				printf("quadrantC: %ld\n", quadrantC);
				printf("quadrantD: %ld\n", quadrantD);
			}
			return quadrantA * quadrantB * quadrantC * quadrantD;
		}

		StringVector BlankMap() {
			return StringVector(MapHeight, std::string(MapWidth, '.'));
		}

		void Reset() {
			CurrentBotLocations = BotLocations;
			SecondsElapsed = 0;
		}

		void PrintMap() {
			StringVector bm = BlankMap();
			for(auto botLoc : CurrentBotLocations) {
				Vec2 pos = botLoc.first;
				bm[pos.y][pos.x] = 'B';
			}
			for(auto s : bm) {
				printf("%s\n", s.c_str());
			}
			printf("Seconds elapsed: %ld\n\n", SecondsElapsed);
		}

		void PrintCurrentBotLocations() {
			for(auto botLoc : CurrentBotLocations) {
				printf("p=%ld,%ld ", botLoc.first.x, botLoc.first.y);
				printf("v=%ld,%ld\n", botLoc.second.x, botLoc.second.y);
			}
			printf("\n");
		}
};

int main(int argc, char *argv[]) {
		Timer timer = Timer();
		timer.Start();
		std::string filePath = "input.txt";
		bool part2 = false;
		bool smallMap = false;
		if(argc > 1)
			filePath = argv[1];
		if(argc > 2 && strcmp(argv[2], "part2") == 0)
			part2 = true;
		if(filePath == "single.txt" || filePath == "testinput.txt")
			smallMap = true;
		StringVector input = Utils::loadFile(filePath);
		BotSimulator b = BotSimulator(input, smallMap);
		for(int i = 0; i < 100; i++) {
			b.Simulate();
		}
		long total = b.GetTotalSafety();
		printf("Total safety: %ld\n", total);
		//Part 2
		while(true) {
			b.Simulate(false, true);
		}
		b.PrintMap();
		timer.End();
		return 0;
}