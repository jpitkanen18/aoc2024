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

struct SortListPointers
{
    bool operator()(long* lhs, long* rhs) const
    {
        return *lhs < *rhs;
    }
};

using StringVector = std::vector<std::string>;

StringVector loadFile(std::string filePath) {
	std::ifstream inFile(filePath);
	StringVector fileAsString;

	if (inFile.is_open()) {
		inFile.clear();
		inFile.seekg(0, std::ios::beg);
		std::string line;
		while (std::getline(inFile, line)) {
			printf("%ld\n", line.length());
			fileAsString.push_back(line);
		}
		inFile.close();
	}
	return fileAsString;
}

void addReverseLines(StringVector *temp) {
	long tempSize = temp->size();
	for(long i = 0; i < tempSize; i++) {
		printf("Iteration: %ld\n", i);
		std::string line(temp->at(i));
		std::reverse(line.begin(), line.end());
		temp->push_back(line);
		printf("size: %ld\n", temp->size());
	}
}

void addVerticalLines(StringVector *input, StringVector *temp) {
	for(long i = 0; i < input->at(0).length(); i++) {
		std::string line;
		for(long j = 0; j < input->size(); j++) {
			line += input->at(j).at(i);
			
		}
		temp->push_back(line);
	}
	
}

void addLeftDiagonalLines(StringVector *input, StringVector *temp) {
	std::string line;
	int maxlen = input->at(0).length();
	for(long i = 0; i < input->size() / maxlen; i++) {
		std::string line;
		for(long j = 0; j < maxlen; j++) {
			char letter = input->at(j).at(j);
		}
		temp->push_back(line);
	}
}


void addRightDiagonalLines(StringVector *input, StringVector *temp) {
	std::string line;
	long maxlen = input->at(0).length();
	for(long i = 0; i < input->size() / maxlen; i++) {
		std::string line;
		for(long j = maxlen - 1; j >= 0; j--) {
			char letter = input->at(j).at(j);
		}
		temp->push_back(line);
	}
}

std::vector<StringVector> addDiagonalsSeparately(StringVector *input) {
	std::vector<StringVector> turku;
	StringVector topLeftDiagonals;
	StringVector topRightDiagonals;
	for(long i = 0; i < input->size(); i++) {
		std::string topLeftLower;
		std::string topLeftUpper;
		std::string topRightLower;
		std::string topRightUpper;
		long x = 0;
		for(long y = i; y < input->size(); y++) {
			topLeftLower += input->at(y).at(x);
			topRightLower += input->at(y).at(input->size() - 1 - x);
			if(i > 0) {
				topRightUpper += input->at(x).at(input->size() - 1 - y);
				topLeftUpper += input->at(x).at(y);
			}
			x++;
		}
		topLeftDiagonals.push_back(topLeftLower);
		if(topLeftUpper.size() > 0)
			topLeftDiagonals.insert(topLeftDiagonals.begin(), topLeftUpper);
		topRightDiagonals.push_back(topRightLower);
		if(topRightUpper.size() > 0)
			topRightDiagonals.insert(topRightDiagonals.begin(), topRightUpper);
	}
	turku.push_back(topLeftDiagonals);
	turku.push_back(topRightDiagonals);
	return turku;
}

void addDiagonals(StringVector *input, StringVector *temp) {
	for(long i = 0; i < input->size(); i++) {
		std::string topLeftLower;
		std::string topLeftUpper;
		std::string topRightLower;
		std::string topRightUpper;
		long x = 0;
		for(long y = i; y < input->size(); y++) {
			topLeftLower += input->at(y).at(x);
			topRightLower += input->at(y).at(input->size() - 1 - x);
			if(i > 0) {
				topRightUpper += input->at(x).at(input->size() - 1 - y);
				topLeftUpper += input->at(x).at(y);
			}
			x++;
		}
		temp->push_back(topLeftLower);
		if(topLeftUpper.size() > 0)
			temp->push_back(topLeftUpper);
		temp->push_back(topRightLower);
		if(topRightUpper.size() > 0)
			temp->push_back(topRightUpper);
		
	}
	
}

StringVector getLines(StringVector *input) {
	StringVector temp(*input);
	printf("Got here Here 1\n");
	addVerticalLines(input, &temp);
	printf("Got here Here 2\n");
	addDiagonals(input, &temp);
	printf("Got here Here 3\n");
	printf("size: %ld\n", temp.size());
	printf("Got here Here 4\n");
	return temp;
}

std::string vecToString(StringVector *input) {
	std::string result;
	for(long i = 0; i < input->size(); i++){
		result.append(input->at(i));
	}
	return result;
}


long xmasRegex(StringVector *input, bool xmasOnly = false) {
	long count = 0;
	for(int i = 0; i < input->size(); i++) {
		std::regex r("XMAS");
		std::string turunMuoto(input->at(i));
		for(std::sregex_iterator i = std::sregex_iterator(turunMuoto.begin(), turunMuoto.end(), r);
                            i != std::sregex_iterator();
                            ++i )
		{
			std::smatch m = *i;
			count++;
			std::cout << m.str() << " at position " << m.position() << " count: " << count << '\n';
		}
		if(!xmasOnly) {
			r ="SAMX";
			for(std::sregex_iterator i = std::sregex_iterator(turunMuoto.begin(), turunMuoto.end(), r);
									i != std::sregex_iterator();
									++i )
			{
				std::smatch m = *i;
				count++;
				std::cout << m.str() << " at position " << m.position() << " count: " << count << '\n';
			}
		}
	}
	return count;
}

void turunPrinttaus(StringVector *lines) {
	std::for_each(lines->begin(), lines->end(),
        [](std::string item)
        {
            printf("%s\n", item.c_str());
        }
	);
}

struct Position {
	int x, y;
};

void countMatches(std::regex r, std::string* input, long *counter, std::vector<Position> *pos, int *j) {
	for(std::sregex_iterator i = std::sregex_iterator(input->begin(), input->end(), r);
									i != std::sregex_iterator();
									++i )
		{
			std::smatch m = *i;
			Position* p = (Position*) malloc(sizeof(Position));
			p->x = m.position();
			p->y = (long)*j;
			pos->push_back(*p);
			std::cout << m.str() << " at position " << m.position() << " count: " << *counter << '\n';
		}
}

long matchesCrossMAS(std::vector<StringVector> *input) {
	long centerIndex = input->at(0).size() / 2;
	std::regex mas("MAS");
	std::regex sam("SAM");	
	long count = 0;
	for(int i = 0; i < input->at(0).size(); i++) {
		std::vector<Position> pos;
		std::string diagonal = input->at(0).at(i);
		countMatches(mas, &diagonal, &count, &pos, &i);
		countMatches(sam, &diagonal, &count, &pos, &i);

		for(int j = 0; j < pos.size(); j++){
			long center = pos.at(j).x + 1;
			long trIndex = abs(centerIndex - i) + (2 * center);
			std::vector<Position> trPos;
			std::string diagonal = input->at(1).at(trIndex);
			countMatches(mas, &diagonal, &count, &trPos, &i);
			countMatches(sam, &diagonal, &count, &trPos, &i);
			for(int k = 0; k < trPos.size(); k++){
					long center2 = trPos.at(k).x + 1;
					long tlIndex = abs(centerIndex - trIndex) + (2 * center2);
					if(tlIndex == i)
						count++;
			}

			
		}
	}
	return count;

}

int main() {
	StringVector input = loadFile("input.txt");
	StringVector lines = getLines(&input);
	std::vector<StringVector> diagonals = addDiagonalsSeparately(&input);
	long crossMatches = matchesCrossMAS(&diagonals);
	long matches = xmasRegex(&lines);
	for(int i = 0; i < lines.size(); i++) {
		printf("%s\n", lines.at(i).c_str());
	}
	printf("inputlines: %ld\n", input.size());
	printf("Lines: %ld\n", lines.size());
	printf("X-MAS Matches: %ld\n", crossMatches);
	printf("Matches: %ld\n", matches);
	return 0;
}