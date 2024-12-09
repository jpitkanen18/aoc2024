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


struct Rule {
	int x, y;
};

using Update = std::vector<int>;

using RulesWithUpdates = std::tuple<std::vector<Rule>, std::vector<Update>>;

bool day2 = true;

RulesWithUpdates loadFile(std::string filePath) {
	std::ifstream inFile(filePath);
	std::vector<Rule> rules;
	std::vector<Update> updates;

	if (inFile.is_open()) {
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		std::string line;
		while (std::getline(inFile, line)) {
			std::istringstream iss(line);
			std::string s;
			Rule* rule = (Rule*) malloc(sizeof(Rule));
			int i = 0;
			Update update;
			if(std::find(line.begin(), line.end(), '|') != line.end()){
				while(std::getline(iss, s, '|')) {
					int val = atoi(s.c_str());
					if(i == 0) {
						rule->x = val;
					} else {
						rule->y = val;
					}
					i++;
				}
				rules.push_back(*rule);
			} else if (std::find(line.begin(), line.end(), ',') != line.end()) {
				while(std::getline(iss, s, ',')) {
					int val = atoi(s.c_str());
					update.push_back(val);
				}
				updates.push_back(update);
			}
			
		}
		inFile.close();
	}
	return RulesWithUpdates(rules, updates);
}
template<typename T>
std::tuple<bool, int> vContains(T *v, int x) {
	long idx = std::find(v->begin(), v->end(), x);
	if(idx != v->end()) {
		return std::tuple<bool, int>(true, idx);
	} else {
		return std::tuple<bool, int>(false, -1);
	}
}

bool testUpdate(std::vector<Rule> *rules, Update *update, Rule* rulePtr){
	bool isValid;
	for(auto rule : *rules) {
		rulePtr->x = rule.x;
		rulePtr->y = rule.y;
		int firstPage = rule.x;
		int secondPage = rule.y;
		long idxP1;
		auto turku1 = std::find(update->begin(), update->end(), firstPage);
		if (turku1 != update->end())
		{
			idxP1 = std::distance(update->begin(), turku1);
		} else 
			idxP1 = -1;
		long idxP2;
		auto turku2 = std::find(update->begin(), update->end(), secondPage);
		if (turku2 != update->end())
		{
			idxP2 = std::distance(update->begin(), turku2);
		} else 
			idxP2 = -1;
		if(idxP1 == -1 || idxP2 == -1) {
			isValid = true;
		}
		else if(idxP1 < idxP2) 
			isValid = true;
		else {
			isValid = false;
			break;
		}
	}
	return isValid;
}

std::vector<int> getValidUpdateIndices(std::vector<Update> *updates, std::vector<Rule> *rules) {
	std::vector<int> indices;
	for (auto& update : *updates) {
		Rule *rulePtr = (Rule*) malloc(sizeof(Rule));
		bool isValid = testUpdate(rules, &update, rulePtr);
		std::vector<Update>::iterator it = std::find(updates->begin(), updates->end(), update);
		long idx = std::distance(updates->begin(), it);
		if(isValid) {
			if (it != updates->end())
			{
				if(!day2) {
					indices.push_back(idx);
				}
			
			}
		} else {
			if(!day2) {
				continue;
			}
			while(!isValid) {
				long firstPage = rulePtr->x;
				long secondPage = rulePtr->y;
				Update::iterator itIdx1 = std::find(update.begin(), update.end(), firstPage);
				long idx1 = std::distance(update.begin(), itIdx1);
				Update::iterator itIdx2 = std::find(update.begin(), update.end(), secondPage);
				long idx2 = std::distance(update.begin(), itIdx2);
				for(auto kulli : update) {
						printf("%ld, ", kulli);
					}
				printf("\n");
				update.at(idx1) = secondPage;
				update.at(idx2) = firstPage;
				for(auto kulli : update) {
					printf("%ld, ", kulli);
				}
				printf("\n");
				isValid = testUpdate(rules, &update, rulePtr);
			}
			if(isValid) {
				indices.push_back(idx);
			}
		}
	}
	return indices;
}

long sumMiddleNumbers(std::vector<Update> *updates, std::vector<Rule> *rules) {
	std::vector<int> validIndices = getValidUpdateIndices(updates, rules);
	std::vector<int> middleNumbers;
	long sum = 0;
	for(auto index : validIndices) {
		Update curUpdate = updates->at(index);
		middleNumbers.push_back(curUpdate.at(curUpdate.size() / 2));
	}
	for (auto middleNum : middleNumbers) {
		sum += middleNum;
	}
	return sum;
}



int main() {
	
	RulesWithUpdates pairs = loadFile("input.txt");
	long sum = sumMiddleNumbers(&std::get<1>(pairs), &std::get<0>(pairs));
	printf("CrossMAS Mix: %ld\n", sum);
	return 0;
}