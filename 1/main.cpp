#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

struct SortListPointers
{
    bool operator()(long lhs, long rhs) const
    {
        return lhs < rhs;
    }
};

struct LongPair {
	LongPair(): left(0), right(0) {};
	long left, right;
};

void printLists(std::vector<long> *first, std::vector<long> *second) {
	std::for_each(first->begin(), first->end(),
        [](long item)
        {
            printf("%ld\n", item);
        }
	);
	std::for_each(second->begin(), second->end(),
        [](long item)
        {
            printf("%ld\n", item);
        }
	);
}

void printPairs(std::vector<LongPair> *pairs) {
	std::for_each(pairs->begin(), pairs->end(),
        [](LongPair item)
        {
            printf("\n\nLeft: %ld\n", item.left);
			printf("Right: %ld\n", item.right);
        }
	);
}

void loadFile(std::string filePath, std::vector<long> *first, std::vector<long> *second) {
	std::ifstream inFile(filePath);
	std::string spaces("    ");

	if (inFile.is_open()) {
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		std::string line;
		bool even = true;
		while (std::getline(inFile, line)) {
			line.replace(line.find(' '), 3, " ");
			printf("%s\n", line.c_str());
			std::istringstream iss(line);
			std::string s;
			while(std::getline(iss, s, ' ')) {
				int val = atol(s.c_str());
				if(even) {
					first->push_back(val);
				} else {
					second->push_back(val);
				}
				even = !even;
			}
			
			

		}
		inFile.close();
	}
}

std::vector<LongPair> pairLists(std::vector<long> *first, std::vector<long> *second) {
	using namespace std;
	vector<long> firstList(*first);
	vector<long> secondList(*second);
	vector<LongPair> paired;
	sort(firstList.begin(), firstList.end(), SortListPointers());
	sort(secondList.begin(), secondList.end(), SortListPointers());
	for(int i = 0; i < firstList.size(); i++){
		struct LongPair *pair = (struct LongPair*)malloc(sizeof(LongPair));
		pair->left = firstList[i];
		pair->right = secondList[i];
		paired.push_back(*pair);
	}
	return paired;
}

long totalDistancesFromPairs(std::vector<LongPair> *pairs) {
	long totalDistance = 0;
	for(int i = 0; i < pairs->size(); i++) {
		long diff = std::max(pairs->at(i).left, pairs->at(i).right) - std::min(pairs->at(i).left, pairs->at(i).right);
		printf("Diff: %ld\n", diff);
		totalDistance += diff;
	}
	return totalDistance;
}

long similarityFromLists(std::vector<long> *first, std::vector<long> *second) {
	using namespace std;
	long totalSimilarity = 0;
	for(int i = 0; i < first->size(); i++) {
		long multiplier = 0;
		long item = first->at(i);
		for(int j = 0; j < second->size(); j++) {
			if(second->at(j) == item) {
				multiplier++;
			}
		}
		totalSimilarity += item * multiplier;
	}
	return totalSimilarity;
}

int main() {
	std::vector<long> firstList;
	std::vector<long> secondList;
	loadFile("input.txt", &firstList, &secondList);
	//printLists(&firstList, &secondList);
	std::vector<LongPair> pairs = pairLists(&firstList, &secondList);
	long totalDistance = totalDistancesFromPairs(&pairs);
	long totalSimilarity = similarityFromLists(&firstList, &secondList);
	//printPairs(&pairs);
	printf("Distances: %ld\n", totalDistance);
	printf("Total similarity: %ld\n", totalSimilarity);
	return 0;
}