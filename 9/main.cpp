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

std::vector<std::string> unpack(std::string *input) {
	std::map<int, int> files; //files with IDs
	std::vector<int> freeSpaces;
	std::string unpacked;
	std::vector<std::string> mapped;
	bool forf = true;
	for(long i = 0; i < input->size(); i++) {
		if(forf) {
			int file = input->at(i) - '0';
			for(long j = 0; j < file; j++) {
				long id = static_cast<long>(std::round(i / 2));
				unpacked.append(std::to_string(id));
				mapped.push_back(std::to_string(id));
			}
		} else {
			int freeSpace =  input->at(i) - '0';
			for(long j = 0; j < freeSpace; j++) {
				unpacked.append(".");
				mapped.push_back(".");
			}
		}
		forf = !forf;
	}
	return mapped;
}

std::vector<std::string> remap(std::vector<std::string>*unpacked) {
	std::vector<std::string> remapped(*unpacked);
	for(long i = remapped.size() - 1; i >= 0; i--) {
		if(remapped.at(i) != ".") {
			std::string num = remapped.at(i);
			for(long j = 0; j < remapped.size(); j++) {
				if(i <= j) {
					return remapped;
				}
				if(remapped.at(j) == "." && i > j) {
					remapped.at(j) = num;
					remapped.at(i) = '.';
					break;
				}
			}
		}
	 }
	 return remapped;
}

long checksum(std::vector<std::string> *remapped) {
	long chcksum = 0;
	for(long i = 0; i < remapped->size(); i++) {
		if(remapped->at(i) != ".") {
			long id = atol(remapped->at(i).c_str());
			chcksum += id * i;
		}
	}
	return chcksum;
}

std::vector<std::string> remapFiles(std::vector<std::string>*unpacked) {
	std::vector<std::string> remapped(*unpacked);
	std::vector<long> handledIndices;
	for(long i = remapped.size() - 1; i >= 0; i--) {
		bool handled = false;
		for(auto h : handledIndices) {
			if(h == i)
				handled = true;
		}
		if(handled) {
			continue;
		}
		if(remapped.at(i) != ".") {
			std::string num(remapped.at(i));
			std::vector<long> positions;
			positions.push_back(i);
			handledIndices.push_back(i);
			for(int j = i -1; j >= 0; j--) {
				if(remapped.at(j) == num) {
					positions.push_back(j);
					handledIndices.push_back(j);
				} else {
					break;
				}
			}
			std::vector<long> freePositions;
			for(long j = 0; j < remapped.size(); j++) {
				if(remapped.at(j) == "." && i > j) {
					freePositions.push_back(j);
					for(int k = j + 1; k < remapped.size(); k++) {
						if(freePositions.size() == positions.size()) {
							for(int x = 0; x < freePositions.size(); x++) {
								remapped.at(freePositions.at(x)) = remapped.at(positions.at(x));
								remapped.at(positions.at(x)) = ".";
							}
							break;
						}
						if(remapped.at(k) == ".") {
							freePositions.push_back(k);
						} else {
							freePositions.clear();
							break;
						}
					}
				}
				// for(auto l : remapped) {
				// printf("%s", l.c_str());
				// }
				// printf("\n");
			}
		}
		
	 }
	 return remapped;
}

int main(void) {
	StringVector lines = loadFile("input.txt");
	std::string input = lines.at(0); //Single line input
	std::vector<std::string> unpacked = unpack(&input);
	std::vector<std::string> remapped = remap(&unpacked);
	std::vector<std::string> remappedP2 = remapFiles(&unpacked);
	long chcksum = checksum(&remapped);
	long chcksum2 = checksum(&remappedP2);
	// printf("Unmapped: %s\n", unpacked.c_str());
	// printf("Remapped: %s\n", remapped.c_str()); 
	printf("Checksum: %ld\n", chcksum);
	printf("Checksum P2: %ld\n", chcksum2);
	return 0;
}