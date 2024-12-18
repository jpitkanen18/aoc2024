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
#include <thread>
#include <queue>
#include <set>
#include <chrono>
#include "../lib/utils.h"
#include "../lib/timer.h"

using StringVector = std::vector<std::string>;

class Pebbles {
	std::string Input;
	std::vector<long> Engravings;
	public:
		Pebbles(std::string filePath = "input.txt") {
			Input = Utils::loadFile(filePath).at(0); // Input is a single line
			Engravings = MapEngravings();
		}

		 std::vector<long> MapEngravings() {
			std::vector<long> e;
			std::stringstream iss(Input);
			std::string s;
			while(getline(iss, s, ' ')) {
				long eng = s[0] - 48;
				s.erase(0, 1);
				for(auto c : s) {
					eng = eng * 10 + (c - 48);
				}
				e.push_back(eng);
			}
			return e;
		}

		 static std::vector<long> Blink(std::vector<long> *stones, int iterations = 0) {
			if(iterations <= 0)
				return *stones;
			else {
				std::vector<long> temp;
				for(auto stone : *stones) {
					std::vector<long> newStones = BlinkLong(stone);
					for(auto newStone : newStones) {
						temp.push_back(newStone);
					}
				}
				return Blink(&temp, iterations - 1);
			} 
		}

		static std::vector<long> BlinkLong(long stone) {
			std::vector<long> e;
			std::string seng(std::to_string(stone));
			if(stone == 0L) {
				e.push_back(1);
			} else if(seng.size() % 2 == 0) {
				std::string left(seng.begin(), seng.begin() + seng.size() / 2);
				std::string right(seng.begin() + (seng.size() / 2), seng.end());
				e.push_back(atol(left.c_str()));
				e.push_back(atol(right.c_str()));
			} else {
				e.push_back(stone * 2024);
			}
			return e;
		}

		static long ProcessRecursively(long stone, std::shared_ptr<std::map<std::string, long>> cache,  long *iterator, int iterations) {
			//printf("Iteration: %ld\n", *iterator);
			std::vector<long> stoneV;
			stoneV.push_back(stone);
			std::vector<long> newStones = Blink(&stoneV, 5);
			*iterator += 1;
			if(iterations <= 5) {
				return newStones.size();
			} else {
				long sum = 0;
				for(auto n_stone : newStones) {
					std::string search(std::to_string(n_stone));
					search.append("-");
					search.append(std::to_string(iterations));
					std::map<std::string, long>::iterator it = cache->find(search);
					if(it != cache->end()) {
						sum += it->second;
					} else {
						long result = ProcessRecursively(n_stone, cache, iterator, iterations - 5);
						cache->insert(std::pair(search, result));
						sum += result;
					}
				}
				return sum;
			}
		}

		static void ProcessRow(long num, long blinks, std::shared_ptr<long> count, std::shared_ptr<std::map<std::string, long>> cache) {
			long iterator = 0;
			long temp = ProcessRecursively(num, cache,  &iterator, blinks);
			*count += temp;
		}
	
		long StonesCount(long blinks) {
			std::vector<long> e(Engravings);
			std::shared_ptr<long> count = std::make_shared<long>(0);
			std::vector<std::thread> threads;
			std::shared_ptr<std::map<std::string,long>> cache = std::make_shared<std::map<std::string, long>>();
			for(auto num : e) {
				threads.push_back(std::thread(ProcessRow, num, blinks, count, cache));
			}
			for(auto& t : threads) {
				t.join();
			}
			return *count;
		}
};

int main(int argc, char *argv[]) {
	Timer timer = Timer();
	timer.Start();
	std::string filePath = "input.txt";
	long blinks = 25;
	if(argc > 1) {
		filePath = argv[1];
	}
	if(argc > 2) {
		blinks = atol(argv[2]);
	}
	Pebbles p = Pebbles(filePath);
	long count = p.StonesCount(blinks);
	timer.End();
	printf("Stones count: %ld\n", count);
	return 0;
}