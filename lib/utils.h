#pragma once
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
#include "./types.h"

class Utils {
	public:
		static StringVector loadFile(std::string filePath) {
			std::ifstream test(filePath);
			if(!test) {
				printf("Did you forget to pass the filepath? ;D\n");
				throw std::exception();
			}
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

		static Vec2 CoordsFromSmatch(std::smatch &m) {
			std::regex r("-{0,1}\\d+");
			std::string input = m.str();
			Vec2 *vec = (Vec2*)malloc(sizeof(Vec2));
			int j = 0;
			for(std::sregex_iterator i = std::sregex_iterator(input.begin(), input.end(), r);
																i != std::sregex_iterator();
																++i )
			{
					std::smatch m = *i;
					long coord = atol(m.str().c_str());
					if(j == 0) {
						vec->x = coord;
					} else {
						vec->y = coord;
					}
					j++;
			}
			return *vec;
		}

		static Vec2 Vec2FromRegex(std::string &input, std::regex &r) {
			Vec2 *vec = (Vec2*)malloc(sizeof(Vec2));
			for(std::sregex_iterator i = std::sregex_iterator(input.begin(), input.end(), r);
																i != std::sregex_iterator();
																++i )
			{
					std::smatch m = *i;
					*vec = CoordsFromSmatch(m);
			}
			return *vec;
		}
};