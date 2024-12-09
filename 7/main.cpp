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

long getEquationsTotal(Equations *equations) {
	long equationsTotal = 0;
	for(auto eq : *equations) {
		long target = eq.first;
		std::vector<long> values = eq.second;
		for(int i = 0; i < pow(2, equations->size()); i++) {
			long total = 0;
			for(int j = 0; j < values.size(); j++) {
				if((i & (1 << j)) != 0)
					total *= values.at(j);
				else
					total += values.at(j);
			}
			if (total == target) {
				equationsTotal += target;
				break;
			} else if (total > target) {
				break;
			}
		}
	}
	return equationsTotal;
}


int main(void) {
	StringVector lines = loadFile("input.txt");
	Equations eqs = getEquations(&lines);
	for(auto eq : eqs) {
		printf("Equation: %ld : ", eq.first);
		for(auto num : eq.second) {
			printf("%ld, ", num);
		}
		printf("\n");
	}
	long eqTotal = getEquationsTotal(&eqs);
	printf("Equations total: %ld\n", eqTotal);
	return 0;
}