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

int main(int argc, char *argv[]) {
		Timer timer = Timer();
		timer.Start();
		std::string filePath = "input.txt";
		if(argc > 1) {
			filePath = argv[1];
		}
		StringVector input = Utils::loadFile(filePath);
		timer.End();
		return 0;
}