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
		timer.End();
		return 0;
}