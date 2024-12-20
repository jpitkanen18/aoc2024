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
#include <chrono>
#include "../lib/timer.h"


using StringVector = std::vector<std::string>;
using Equations = std::map<long, std::vector<long>>;

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

std::vector<long> getEqNums(std::string *numsAsString) {
	std::regex r("\\d+");
	std::vector<long> nums;
	for(std::sregex_iterator i = std::sregex_iterator(numsAsString->begin(), numsAsString->end(), r);
                            i != std::sregex_iterator();
                            ++i )
    {
        std::smatch m = *i;
		long num = atol(m.str().c_str());
		nums.push_back(num);
        std::cout << m.str() << " at position " << m.position() << '\n';
    }
	return nums;
}

Equations getEquations(StringVector *lines) {
	Equations equations;
	for(auto line : *lines) {
		std::stringstream iss(line);
		std::string s;
		while(std::getline(iss, s, ':')) {
			long eqVal = atol(s.c_str());
			std::getline(iss, s, ':');
			std::vector<long> eqNums = getEqNums(&s);
			equations.insert(std::pair<long, std::vector<long>> (eqVal, eqNums));
		}
	}
	return equations;
}

void processRow(std::pair<long, std::vector<long>> eq, std::shared_ptr<unsigned long> accP1, std::shared_ptr<unsigned long> accP2) {
	long target = eq.first;
	std::vector<long> values = eq.second;
	std::vector<std::string> stringValues;
	for(auto val : values) {
		stringValues.push_back(std::to_string(val));
	}
	bool foundPart2 = false;
	long long pow = (long long)std::pow(2, values.size());
	for(long multiply = 0; multiply < pow; multiply++) {
		long total = 0;
		for(long x = 0; x < values.size(); x++) {
			if((multiply & (1 << x)) != 0)
				total *= values.at(x);
			else
				total += values.at(x);
		}
		if (total == target) {
			*accP1 += target;
			if(!foundPart2)
				*accP2 += target;
			break;
		}
		for(long concat = 0; concat < pow; concat++) {
			long totalP2 = 0;
			for(int x = 0; x < values.size(); x++) {
				if((multiply & (1 << x)) != 0)
					totalP2 *= values.at(x);
				else if((concat & (1 << x)) != 0) {
					totalP2 = totalP2 * (long)std::pow(10, stringValues.at(x).size()) + values.at(x);
				} else
					totalP2 += values.at(x);
				if (totalP2 > target) {
					break;
				}
			}
			if(totalP2 == target) {
				*accP2 += totalP2;
				foundPart2 = true;
				break;
			}
		}
	}
}

void getEquationsTotal(Equations *equations, std::shared_ptr<unsigned long> accP1, std::shared_ptr<unsigned long> accP2) {
	long equationsSize = equations->size();
	long accumulator = 0;
	std::vector<std::thread> threads;
	for (auto it = std::begin(*equations); it != std::end(*equations); it++) {
		threads.push_back(std::thread(processRow, *it, std::ref(accP1), std::ref(accP2)));
		if (threads.size() == equations->size()) {
            for (auto& t : threads) {
				accumulator++;
				// printf("\e[1;1H\e[2J");
				// printf("Equations done: %ld/%ld\n", accumulator, equationsSize);
				// printf("\nEquations total: %ld\n", *accP1);
				// printf("Calibration total: %ld\n", *accP2);
                t.join();
            }
            threads.clear();
        }
	}
	for (auto& t : threads) {
		accumulator++;
		// printf("\e[1;1H\e[2J");
		// printf("Equations done: %ld/%ld\n", accumulator, equationsSize);
		// printf("\nEquations total: %ld\n", *accP1);
		// printf("Calibration total: %ld\n", *accP2);
    	t.join();
	}
}

int main(void) {
	Timer timer = Timer();
	timer.Start();
	StringVector lines = loadFile("input.txt");
	Equations eqs = getEquations(&lines);
	std::shared_ptr<unsigned long> eqTotal(static_cast<unsigned long*>(malloc(sizeof(unsigned long))), free);
	std::shared_ptr<unsigned long> calTotal(static_cast<unsigned long*>(malloc(sizeof(unsigned long))), free);
	*eqTotal = 0;
	*calTotal = 0;
	getEquationsTotal(&eqs, eqTotal, calTotal);
	printf("\e[1;1H\e[2J");
	printf("\nEquations total: %ld\n", *eqTotal);
	printf("Calibration total: %ld\n", *calTotal);
	timer.End();
	return 0;
}