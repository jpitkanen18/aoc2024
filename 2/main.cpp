#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

using Record = std::vector<long>;

enum Safety {
	SAFE,
	UNSAFE
};

enum Direction {
	Increasing,
	Descreasing
};

std::vector<Record> loadFile(std::string filePath) {
	std::ifstream inFile(filePath);
	std::vector<Record> records;

	if (inFile.is_open()) {
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		std::string line;
		while (std::getline(inFile, line)) {
			printf("Record: %s\n", line.c_str());
			std::istringstream iss(line);
			std::string s;
			Record record;
			while(std::getline(iss, s, ' ')) {
				record.push_back(atol(s.c_str()));
			}
			records.push_back(record);
		}
		inFile.close();
	}
	return records;
}

Safety getRecordSafety(Record *record) {
	using namespace std;
	Safety safety = SAFE;
	Direction mainDir;
	bool tolerateLevel = true;
	for(int i = 1; i < record->size(); i++) {
		long diff = record->at(i) - record->at(i - 1);
		printf("\nDiff %ld", diff);
		Direction localDir;
		if (diff >= -3 && diff < 0) {
			localDir = Descreasing;
		}
		if (diff <= 3 && diff > 0) {
			localDir = Increasing;
		}
		if(mainDir != Increasing && mainDir != Descreasing) {
			mainDir = localDir;
		}
		if(diff < -3 || diff > 3 || diff == 0 || (mainDir != localDir)) {
			if(tolerateLevel) {
				tolerateLevel = false;
			} else {
				safety = UNSAFE;
			}
			continue;
		}
		mainDir = localDir;
	}
	return safety;
}

int main() {
	std::vector<Record> records = loadFile("input.txt");
	std::vector<Record> safeRecords;
	std::vector<Record> unsafeRecords;
	for(int i = 0; i < records.size(); i++) {
		Record record = records.at(i);
		if(getRecordSafety(&record) != SAFE) {
			unsafeRecords.push_back(record);
		} else {
			safeRecords.push_back(record);
		}
	}

	std::for_each(records.begin(), records.end(),
        [](Record item)
        {
				std::for_each(item.begin(), item.end(),
					[](long item2)
					{
						printf("%ld ", item2);
					}
				);
			printf("\nSafety: %d \n", getRecordSafety(&item));
        }
	);

	printf("Safe records: %ld\n", safeRecords.size()); 

	return 0;
}