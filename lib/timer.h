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
#include <chrono>


class Timer {
	std::chrono::_V2::system_clock::time_point t_start;
	public:
		Timer() {}

		void Start() {
			t_start = std::chrono::high_resolution_clock::now();
		}

		void End() {
			auto t_end = std::chrono::high_resolution_clock::now();
			double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();
			long minutes = elapsed_time_ms / 1000 / 60;
			long seconds = (elapsed_time_ms - (minutes * 1000 * 60)) / 1000;
			std::cout << "Elapsed time: " << minutes << " minutes, " << seconds << " seconds" << std::endl;
			printf("Elapsed time ms: %lf\n", elapsed_time_ms);
		}
};