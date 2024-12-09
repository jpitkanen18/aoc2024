#include <map>
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

std::string loadFile(std::string filePath) {
	std::ifstream inFile(filePath);
	std::string fileAsString;

	if (inFile.is_open()) {
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		std::string line;
		while (std::getline(inFile, line)) {
			fileAsString.append(line);
		}
		inFile.close();
	}
	return fileAsString;
}

long multiply(std::string *instruction) {
	std::regex r("\\d+");
	std::vector<long> nums;
	for(std::sregex_iterator i = std::sregex_iterator(instruction->begin(), instruction->end(), r);
                            i != std::sregex_iterator();
                            ++i )
    {
        std::smatch m = *i;
		long num = atol(m.str().c_str());
		nums.push_back(num);
        std::cout << m.str() << " at position " << m.position() << '\n';
    }
	return nums.at(0) * nums.at(1);
}

long multiplicationsSum(std::string *input) {
	std::regex r("mul\\(\\d+,\\d+\\)");
	long total = 0;
	for(std::sregex_iterator i = std::sregex_iterator(input->begin(), input->end(), r);
                            i != std::sregex_iterator();
                            ++i )
    {
        std::smatch m = *i;
		std::string instruction = m.str();
		long result = multiply(&instruction);
		total += result;
        std::cout << instruction << " at position " << m.position() << '\n';
    }
	return total;
}

long multiplicationsSumConditionals(std::string *input) {
	std::regex r("((mul\\(\\d+,\\d+\\))|(do(|n't)\\(\\)))");
	long total = 0;
	bool allowMul = true;
	for(std::sregex_iterator i = std::sregex_iterator(input->begin(), input->end(), r);
                            i != std::sregex_iterator();
                            ++i )
    {
        std::smatch m = *i;
		std::string instruction = m.str();
		if(instruction == "do()") {
			allowMul = true;
		} else if(instruction == "don't()"){
			allowMul = false;
		} else if(allowMul) {
			long result = multiply(&instruction);
			total += result;
		}
        std::cout << instruction << " at position " << m.position() << " allowMul: " << allowMul << '\n';
    }
	return total;
}

int main() {
	std::string fileAsString = loadFile("input.txt");
	long sum = multiplicationsSum(&fileAsString);
	long sumCond = multiplicationsSumConditionals(&fileAsString);
	printf("Total: %ld\n", sum);
	printf("Total with conditionals: %ld\n", sumCond);
	return 0;
}