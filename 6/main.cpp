#include <map>
#include <regex>
#include <vector>
#include <stddef.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string.h>

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

struct Position {
	int x, y;
	bool operator==(const Position& a) const
	{
		return (x == a.x && y == a.y);
	}
};

using StringVector = std::vector<std::string>;

StringVector loadFile(std::string filePath) {
	std::ifstream inFile(filePath);
	StringVector fileAsString;

	if (inFile.is_open()) {
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		std::string line;
		while (std::getline(inFile, line)) {
			fileAsString.push_back(line);
		}
		inFile.close();
	}
	return fileAsString;
}

void getInitialPosition(Position* pos, StringVector *lines) {
	for(int i = 0; i < lines->size(); i++) {
		for(int j = 0; j < lines->at(i).size(); j++) {
			if(lines->at(i).at(j) == '^') {
				pos->x = j;
				pos->y = i;
			}
		}
	}
}


bool isOutOfBounds(int newX, int newY, StringVector *lines) {
	return newY < 0 || newY >= lines->size() || newX < 0 || newX >= lines->at(0).size();
}

bool canMove(int newX, int newY, StringVector *lines) {
	return isOutOfBounds(newX, newY, lines) || lines->at(newY).at(newX) != '#';
}

void nextPosition(Position *newPos, Direction *dir){
	switch(*dir){
		case UP:
			newPos->y--;
			break;
		case DOWN:
			newPos->y++;
			break;
		case RIGHT:
			newPos->x++;
			break;
		case LEFT:
			newPos->x--;
			break;
	}
}

Position findNextPos(Direction *dir, Position* pos, StringVector *lines) {
	Position *newPos = (Position*) malloc(sizeof(Position));
	newPos->y = pos->y;
	newPos->x = pos->x;
	nextPosition(newPos, dir);
	while(!canMove(newPos->x, newPos->y, lines)) {
		switch(*dir){
			case UP:
				*dir = RIGHT;
				break;
			case DOWN:
				*dir = LEFT;
				break;
			case RIGHT:
				*dir = DOWN;
				break;
			case LEFT:
				*dir = UP;
				break;
		}
		
		newPos->y = pos->y;
		newPos->x = pos->x;
		nextPosition(newPos, dir);
	}
	
	return *newPos;
}

bool isObstructed(Position *obsPos, Position *ogPos, StringVector *lines) {
	Position *localOg = (Position*) malloc(sizeof(Position));
	*localOg = *ogPos;

	Direction dir = UP;

	StringVector mapCopy(*lines);
	mapCopy.at(obsPos->y).at(obsPos->x) = '#';

	std::vector<std::tuple<Position, Direction>> turns;
	bool infiniteLoop = false;
	bool stayIn = true;
	while(stayIn) {
		Position newPos = findNextPos(&dir, localOg, &mapCopy);
		if(isOutOfBounds(newPos.x, newPos.y, &mapCopy)) {
			stayIn = false;
			break;
		}
		std::tuple<Position, Direction> turku(newPos, dir);
		*localOg = newPos;
		bool contains = false;
		for(int i = 0; i < turns.size(); i++){
			std::tuple<Position, Direction> turn = turns.at(i);
			if(std::get<0>(turn).x == std::get<0>(turku).x && std::get<0>(turn).y == std::get<0>(turku).y && std::get<1>(turn) == std::get<1>(turku)) {
				contains = true;
				break;
			}
		}
		if(contains) {
			infiniteLoop = true;
			stayIn = false;
			break;
		} else {
			turns.push_back(turku);
		}
	}
	return infiniteLoop;
}

int main() {
	StringVector lines = loadFile("input.txt");
	std::vector<Position> positions;
	Position *pos = (Position*) malloc(sizeof(Position));
	Position *ogPos = (Position*) malloc(sizeof(Position));
	getInitialPosition(pos, &lines);
	memcpy(ogPos, pos, sizeof(Position));
	Direction dir = UP;
	long possibleObstructions = 0;
	while(true) {
		Position newPos = findNextPos(&dir, pos, &lines);
		if(isOutOfBounds(newPos.x, newPos.y, &lines))
			break;
		else {
			pos->x = newPos.x;
			pos->y = newPos.y;
			std::vector<Position>::iterator itr = std::find(positions.begin(), positions.end(), newPos);
			if(itr == positions.end()) {
				positions.push_back(newPos);
			}
		}
	}

	for(auto position : positions) {
		if(isObstructed(&position, ogPos, &lines))
			possibleObstructions++;
		printf("Obstrooctons: %ld\n", possibleObstructions);
	}
	
	printf("Distinct positions: %ld\n", positions.size() + 1); // Genius offset correction
	printf("Possible obstructions: %ld\n", possibleObstructions);
	free(pos);
	free(ogPos);
	return 0;
}