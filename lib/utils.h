#include <regex>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>

using StringVector = std::vector<std::string>;

class Utils {
	public:
		static StringVector loadFile(std::string filePath) {
			std::ifstream test(filePath);
			if(!test)
				throw std::exception();
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
};