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
#include <cmath>
#include <thread>
#include <memory>
#include <mutex>
#include <chrono>

using StringVector = std::vector<std::string>;

using SymbolAt = std::tuple<std::string, int, int>;

struct Position {
	int x, y;
};

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

std::vector<SymbolAt> getMatches(std::string *line, int y) {
	std::regex r("[A-Za-z0-9]");
	std::vector<SymbolAt> matches;
	int x = 0;
	for(std::sregex_iterator i = std::sregex_iterator(line->begin(), line->end(), r);
                            i != std::sregex_iterator();
                            ++i )
    {
        std::smatch m = *i;
		SymbolAt match(m.str(), m.position(), y);
		matches.push_back(match);
        //std::cout << m.str() << " at position " << m.position() << '\n';
		x++;
    }
	return matches;
}

 std::vector<SymbolAt> processRow(std::string line, int y) {
	std::vector<SymbolAt> matches = getMatches(&line, y);
	return matches;
}


std::vector<SymbolAt> getSymbols(StringVector *lines) {
    std::vector<SymbolAt> symbols;
    std::vector<std::thread> threads;
    std::shared_ptr<std::vector<SymbolAt>> symbolsPtr = std::make_shared<std::vector<SymbolAt>>();
    std::mutex mtx; // Mutex to protect the shared vector

    for(int y = 0; y < lines->size(); y++) {
        std::string line = lines->at(y);
        threads.push_back(std::thread([line, y, symbolsPtr, &mtx]() {
            std::vector<SymbolAt> result = processRow(line, y);
            std::lock_guard<std::mutex> lock(mtx);
            symbolsPtr->insert(symbolsPtr->end(), result.begin(), result.end());
        }));
        
        if (threads.size() == 16) {
            for (auto& t : threads) {
                t.join();
            }
            threads.clear();
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    return *symbolsPtr;
}

StringVector getUniques(std::vector<SymbolAt> *symbols) {
	std::vector<std::string> uniqueSymbols;
	for(auto symbol : *symbols) {
		std::string symbolStr = std::get<0>(symbol);
		bool includes = false;
		for(auto uniqueSymbol: uniqueSymbols) {
			if(symbolStr == uniqueSymbol)
				includes = true;
		}
		if(!includes) {
			uniqueSymbols.push_back(symbolStr);
		}
	}
	return uniqueSymbols;
}

std::vector<SymbolAt> filterLocations(std::vector<SymbolAt> *symbols, std::string symbol) {
	std::vector<SymbolAt> locations;
	for(auto symbolPos : *symbols) {
		if(std::get<0>(symbolPos) == symbol) {
			locations.push_back(symbolPos);
		}
	}
	return locations;
}

std::vector<Position> getUniquePositions(std::vector<Position> *positions) {
	std::vector<Position> uniques;
	for(auto pos : *positions) {
		bool includes = false;
		for(auto unique : uniques) {
			if(pos.x == unique.x && pos.y == unique.y)
				includes = true;
		}
		if(!includes){
			uniques.push_back(pos);
		}
	}
	return uniques;
}

int main(void) {
	StringVector lines = loadFile("input.txt");
	std::vector<SymbolAt> symbols = getSymbols(&lines);
	std::vector<std::string> uniqueSymbols = getUniques(&symbols);
	std::vector<Position> positions;
	std::vector<Position> positionsP2;
	for(auto unique : uniqueSymbols) {
		std::vector<SymbolAt> locations = filterLocations(&symbols, unique);
		for(int i = 0; i <  locations.size(); i++) {
			for(int j = i + 1; j < locations.size(); j++) {
				Position *node1 = (Position*) malloc(sizeof(Position));
				Position *node2 = (Position*) malloc(sizeof(Position));
				SymbolAt node1loc = locations.at(i);
				SymbolAt node2loc = locations.at(j);

				node1->x = std::get<1>(node1loc);
				node1->y = std::get<2>(node1loc);
				node2->x = std::get<1>(node2loc);
				node2->y = std::get<2>(node2loc);

				int dx = node2->x - node1->x;
				int dy = node2->y - node1->y;

				printf("node1->x: %d, node2->x: %d\n", node1->x, node2->x);

				double distance = sqrt(dx * dx + dy * dy);
				double angle = atan2(dy, dx);

				Position *antinode1 = (Position*)malloc(sizeof(Position));
				Position *antinode2 = (Position*)malloc(sizeof(Position));

				antinode1->x = static_cast<int>(std::round(node1->x + distance * cos(angle + M_PI)));
				antinode1->y = static_cast<int>(std::round(node1->y + distance * sin(angle + M_PI)));
				antinode2->x = static_cast<int>(std::round(node2->x + distance * cos(angle)));
				antinode2->y = static_cast<int>(std::round(node2->y + distance * sin(angle)));
				positions.push_back(*antinode1);
				positions.push_back(*antinode2);
				for(int d = 0; d < (int) lines.size() * 2; d++) {
					double newDistance = distance * d;
					Position *antinode1 = (Position*)malloc(sizeof(Position));
					Position *antinode2 = (Position*)malloc(sizeof(Position));
					antinode1->x = static_cast<int>(std::round(node1->x + newDistance * cos(angle + M_PI)));
					antinode1->y = static_cast<int>(std::round(node1->y + newDistance * sin(angle + M_PI)));
					antinode2->x = static_cast<int>(std::round(node2->x + newDistance * cos(angle)));
					antinode2->y = static_cast<int>(std::round(node2->y + newDistance * sin(angle)));
					positionsP2.push_back(*antinode1);
					positionsP2.push_back(*antinode2);
				}
			}
		}
	}	
	std::vector<Position> uniquePositions = getUniquePositions(&positions);
	std::vector<Position> uniquePositionsP2 = getUniquePositions(&positionsP2);
	int uniquesP1 = 0;
	for(auto pos : uniquePositions) {
		if(pos.x >= 0 && pos.y >= 0 && pos.x < lines.at(0).size() && pos.y < lines.size()) {
			lines.at(pos.y).at(pos.x) = '#';
			uniquesP1++;
		}
			
	}
	int uniquesP2 = 0;
	for(auto pos : uniquePositionsP2) {
		if(pos.x >= 0 && pos.y >= 0 && pos.x < lines.at(0).size() && pos.y < lines.size()) {
			uniquesP2++;
		}
			
	}

	for(auto line : lines) {
		printf("%s\n", line.c_str());
	}

	printf("Unique antinodes: %d\n", uniquesP1);
	printf("Unique antinodes P2: %d\n", uniquesP2);

	return 0;
}