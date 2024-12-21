#include <regex>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <climits>
#include <mutex>
#include <thread>
#include <queue>
#include <set>
#include <chrono>
#include "../lib/utils.h"
#include "../lib/timer.h"
#include <atomic>

using Prize = std::tuple<Vec2, Vec2, Vec2>;

class ClawGenius {
	StringVector Lines;
	std::vector<Prize> Prizes;
	bool Part2 = false;
	public:
		ClawGenius(StringVector &input, bool part2) {
			Lines = input;
			Part2 = part2;
			ParseInput(Part2);
			//PrintInput();
		}

		Vec2 CoordsFromSmatch(std::smatch &m) {
			std::regex r("\\d+");
			std::string input = m.str();
			Vec2 *vec = (Vec2*)malloc(sizeof(Vec2));
			int j = 0;
			for(std::sregex_iterator i = std::sregex_iterator(input.begin(), input.end(), r);
																i != std::sregex_iterator();
																++i )
			{
					std::smatch m = *i;
					int coord = atoi(m.str().c_str());
					if(j == 0) {
						vec->x = coord;
					} else {
						vec->y = coord;
					}
					j++;
			}
			return *vec;
		}

		Vec2 Vec2FromRegex(std::string &input, std::regex &r) {
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
		 
		void ParseInput(bool part2) {
			std::vector<Vec2> temp;
			std::array<std::regex, 3> regexes = {
				std::regex("A: X\\+\\d+, Y\\+\\d+"),
				std::regex("B: X\\+\\d+, Y\\+\\d+"),
				std::regex("X=\\d+, Y=\\d+")
			};
			int regexIter = 0;
			for(auto s : Lines) {
				if(s.size() == 0) {
					if(part2) {
						temp[2].x += 10000000000000;
						temp[2].y += 10000000000000;
					}
					Prizes.push_back(std::tuple(temp[0], temp[1], temp[2]));
					regexIter = 0;
					temp.clear();
				} else {
					temp.push_back(Vec2FromRegex(s, regexes[regexIter]));
					regexIter++;
				}
			}
			if(part2) {
				temp[2].x += 10000000000000;
				temp[2].y += 10000000000000;
			}
			Prizes.push_back(std::tuple(temp[0], temp[1], temp[2]));
		}

		static void ProcessPrize(Prize *p, bool part2, std::atomic<long> &minTokens) {
			long mt = part2 ? MinTokensToWinP2(p) : MinTokensToWin(p);
			if(mt > 0) {
					minTokens.fetch_add(mt, std::memory_order_relaxed);
			}
		}

		long CountTokens() {
			bool part2 = Part2;
			std::atomic<long> minTokens(0);
			std::vector<std::thread> threads;
			for(auto &p : Prizes) {
				threads.push_back(std::thread(ProcessPrize, &p, part2, std::ref(minTokens)));
			}
			for(auto &t : threads) {
				t.join();
			}
			return minTokens.load();
		}

		static long MinTokensToWin(Prize *prize) {
			long minCost = LONG_MAX;
			Vec2 A = std::get<0>(*prize);
			Vec2 B = std::get<1>(*prize);
			Vec2 P = std::get<2>(*prize);
			for(int x = 0; x <= 100; x++) {
				for(int y = 0; y <= 100; y++) {
					if(((A.x * x) + (B.x * y) == P.x) && ((A.y * x) + (B.y * y) == P.y)) {
						long tokens = (3 * x + y);
						minCost = std::min(minCost, tokens);
					}
				}
			}
			return (minCost == LONG_MAX) ? -1 : minCost;
		}

		static long MinTokensToWinP2(Prize *prize) {
			Vec2 A = std::get<0>(*prize);
			Vec2 B = std::get<1>(*prize);
			Vec2 P = std::get<2>(*prize);
			long divider = (A.x * B.y) - (B.x * A.y);
			long a = ((P.x * B.y) - (B.x * P.y)) / divider;
			long b = ((A.x * P.y) - (P.x * A.y)) / divider;
			if((A.x * a + B.x * b) != P.x || (A.y * a + B.y * b) != P.y)
				return -1;
			return (a * 3 + b);
		}

		void PrintInput() {
			for(auto p : Prizes) {
				printf("A: X+%ld, Y+%ld\n", std::get<0>(p).x, std::get<0>(p).y);
				printf("B: X+%ld, Y+%ld\n", std::get<1>(p).x, std::get<1>(p).y);
				printf("Prize: X=%ld, Y=%ld\n\n", std::get<2>(p).x, std::get<2>(p).y);
			}
			printf("%s\n", Lines[Lines.size() - 1].c_str());
			printf("Prize: X=%ld, Y=%ld\n\n", std::get<2>(Prizes[Prizes.size() - 1]).x, std::get<2>(Prizes[Prizes.size() - 1]).y);
		}

};

int main(int argc, char *argv[]) {
		Timer timer = Timer();
		timer.Start();
		std::string filePath = "testinput.txt";
		bool part2 = false;
		if(argc > 1) {
			filePath = argv[1];
		}
		if(argc > 2 && strcmp(argv[2], "part2") == 0) {
			part2 = true;
		}
		StringVector input = Utils::loadFile(filePath);
		ClawGenius genius = ClawGenius(input, part2);
		long mintokens = genius.CountTokens();
		printf("Mintokens: %ld\n", mintokens);
		timer.End();
		return 0;
}