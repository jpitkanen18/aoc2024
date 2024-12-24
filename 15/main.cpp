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

class WarehouseManager {
	StringVector Lines;
	StringVector Map;
	StringVector CurrentMap;
	std::vector<Vec2> Movements;
	std::vector<Vec2> BoxLocations;
	Vec2 Robo = Vec2(-1, -1); // Lol
	bool Part2 = false;
	public:
		WarehouseManager(StringVector &input, bool part2 = false) {
			Lines = input;
			Part2 = part2;
			if(Part2)
				ParseInputP2();
			else
				ParseInput();
			CurrentMap = Map;
			Robo = GetRobotLocation();
			GetInitialBoxLocations();
			PrintCurrentMap();
		}

		void ParseInputP2() {
			bool mapDone = false;
			for(auto line : Lines) {
				if(line.size() == 0){
					mapDone = true;
					continue;
				}
				if(!mapDone) {
					std::string lineXL;
					for(auto c : line) {
						switch(c) {
							case '#':
								lineXL.append("##");
								break;
							case '@':
								lineXL.append("@.");
								break;
							case 'O':
								lineXL.append("[]");
								break;
							case '.':
								lineXL.append("..");
								break;
						}
					}
					Map.push_back(lineXL);
				}
				else
					ParseMove(line);
			}
		}

		void ParseMove(std::string &line) {
			for(auto c : line) {
				switch(c) {
					case '^':
						Movements.push_back(Vec2(0, -1));
						break;
					case 'v':
						Movements.push_back(Vec2(0, 1));
							break;
					case '<':
						Movements.push_back(Vec2(-1, 0));
						break;
					case '>':
						Movements.push_back(Vec2(1, 0));
						break;
				}
			}
		}

		void ParseInput() {
			bool mapDone = false;
			for(auto line : Lines) {
				if(line.size() == 0){
					mapDone = true;
					continue;
				}
				if(!mapDone)
					Map.push_back(line);
				else
					ParseMove(line);
			}
		}

		void ClearMap() {
			for(auto &s : CurrentMap) {
				for(auto &c : s) {
					if(c != '#')
						c = '.';
				}
			}
		}

		void UpdateMap() {
			ClearMap();
			for(auto loc : BoxLocations) {
				if(Part2) {
					CurrentMap[loc.y][loc.x] = '[';
					CurrentMap[loc.y][loc.x + 1] = ']';
				} else {
					CurrentMap[loc.y][loc.x] = 'O';
				}
			}
			CurrentMap[Robo.y][Robo.x] = '@';
		}

		void GetInitialBoxLocations() {
			for(int i = 1; i < Map.size(); i++) {
				for(int j = 1; j < Map[i].size(); j++) {
					if(Part2) {
						if(Map[i][j] == '[' && Map[i][j + 1] == ']')
							BoxLocations.push_back(Vec2(j, i));
					}
					else {
						if(Map[i][j] == 'O')
							BoxLocations.push_back(Vec2(j, i));
					}

				}
			}
		}

		Vec2 GetRobotLocation() {
			Vec2 roboLoc(-99, -99);
			for(int i = 0; i < Map.size(); i++) {
				for(int j = 0; j < Map[i].size(); j++) {
					if(Map[i][j] == '@') {
						roboLoc.y = i;
						roboLoc.x = j;
					}
				}
			}
			return roboLoc;
		}

		bool IsInvalid(Vec2 &pos) {
			return pos.x == 0 || pos.x == Map[0].size() - 1 || pos.y == 0 || pos.y == Map.size() - 1 || Map[pos.y][pos.x] == '#';
		}

		bool IsInvalidP2(Vec2 &pos) {
			return pos.x <= 1 || pos.x >= Map[0].size() - 1 || pos.y == 0 || pos.y >= Map.size() - 1;
		}

		void FindInFront(std::set<Vec2*> &inFront, Vec2 &nextPos, Vec2 &mov, bool &canMove, bool isOther = false) {
			for(auto &boxLoc : BoxLocations) {
				Vec2 other(boxLoc.x + 1, boxLoc.y);
				if(nextPos == boxLoc || (Part2 && !isOther && nextPos == other)) {
					Vec2 nextBoxLoc(boxLoc.x + ((Part2) ? mov.x * 2 : mov.x), boxLoc.y + mov.y);
					Vec2 nextOther(other.x + (Part2 ? mov.x * 2 : mov.x), other.y + mov.y);
					if(Part2 ? IsInvalidP2(nextBoxLoc) : IsInvalid(nextBoxLoc)) {
						canMove = false;
						return;
					}
					if(Part2 && !isOther) {
						if(IsInvalidP2(nextOther) || (mov.y != 0 && (Map[nextBoxLoc.y][nextBoxLoc.x] == '#' && Map[nextOther.y][nextOther.x] == '#'))) {
							canMove = false;
							return;
						}
					}
					FindInFront(inFront, nextBoxLoc, mov, canMove);
					if(!isOther)
						FindInFront(inFront, nextOther, mov, canMove, true);
					inFront.insert(&boxLoc);
					return;
				}
			}
		}

		void MoveRobot(bool print = false) {
			for(auto mov : Movements) {
				Vec2 nextPos(Robo.x + mov.x, Robo.y + mov.y);
				if(IsInvalid(nextPos))
					continue;
				bool canMove = true;
				std::set<Vec2*> inFront;
				FindInFront(inFront, nextPos, mov, canMove);
				if(canMove) {
					for(auto &p : inFront) {
						*p += mov;
					}
					Robo = nextPos;
				}
				UpdateMap();
				if(print)
					PrintCurrentMap();
			}
		}

		long GetCoordinatesSum(bool print = false) {
			MoveRobot(print);
			long sum = 0;
			for(auto boxLoc : BoxLocations) {
				if(Part2) {
					int dx1 = boxLoc.x;
					int dx2 = CurrentMap[0].size() - boxLoc.x + 1;
					int x = dx1 < dx2 ? dx1 : dx2;
					sum += 100 * boxLoc.y + x;
				} else
					sum += 100 * boxLoc.y + boxLoc.x;
			}
			return sum;
		}

		void PrintCurrentMap() {
			for(auto s : CurrentMap) {
				printf("%s\n", s.c_str());
			}
			printf("\n");
		}
};

int main(int argc, char* argv[]) {
	Timer timer = Timer();
	std::string filePath = "testinput.txt";
	timer.Start();
	bool part2 = true;
	bool smallMap = false;
	if(argc > 1)
		filePath = argv[1];
	if(argc > 2 && strcmp(argv[2], "part2") == 0)
		part2 = true;
	StringVector input = Utils::loadFile(filePath);
	WarehouseManager wm = WarehouseManager(input, part2);
	long sum = wm.GetCoordinatesSum(true);
	timer.End();
	printf("Coords Sum: %ld\n", sum);
	return 0;
}