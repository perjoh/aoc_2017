#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <cassert>

using string_vec = std::vector<std::string>;

template <typename Stream>
auto split_input(Stream& stream)
{
	string_vec result;
	std::string input;
	while (std::getline(stream, input, ',') && !input.empty()) {
		result.push_back(input);
	}
	return result;
}

auto split_input(const char* input)
{
	std::stringstream ss(input);
	return split_input(ss);
}

std::string spin(std::string programs, int value)
{
	std::string result(programs.cend() - value, programs.cend());
	result.insert(result.end(), programs.cbegin(), programs.cbegin() + programs.length() - value);
	return result;
}

std::string exchange(std::string programs, int a, int b)
{
	std::swap(programs[a], programs[b]);
	return programs;
}

std::string partner(std::string programs, char a, char b)
{
	auto it_a = std::find(programs.begin(), programs.end(), a);
	auto it_b = std::find(programs.begin(), programs.end(), b);

	assert(it_a != programs.end());
	assert(it_b != programs.end());

	std::swap(*it_a, *it_b);

	return programs;
}

int to_int(const std::string& s)
{
	std::stringstream ss(s);
	int value = 0;
	ss >> value;
	return value;
}

std::string dance(std::string programs, const string_vec& dance_moves)
{
	for (auto& dance_move : dance_moves) {
		static const std::regex s_regex("s(\\d+)");
		static const std::regex x_regex("x(\\d+)/(\\d+)");
		static const std::regex p_regex("p([a-p])/([a-p])");

		std::smatch match;

		if (std::regex_match(dance_move, match, s_regex)) {
			programs = spin(programs, to_int(match[1]));
		} else if (std::regex_match(dance_move, match, x_regex)) {
			programs = exchange(programs, to_int(match[1]), to_int(match[2]));
		} else if (std::regex_match(dance_move, match, p_regex)) {
			programs = partner(programs, match[1].str()[0], match[2].str()[0]);
        } else {
            assert(false);
        }
	}

	return programs;
}

void test()
{
	std::string programs("abcde");
	programs = dance(programs, split_input("s1,x3/4,pe/b"));
	assert(programs == "baedc");
}

int main()
{
	test();

	const char* filename = "input/aoc_16.txt";
	std::ifstream file(filename);
	if (file) {
		std::string programs("abcdefghijklmnop");
		const std::string org_order = programs;
		programs = dance(programs, split_input(file));
		std::cout << "Part 1: " << programs << "\n";

		programs = org_order;

		// Testing reveals that the pattern wraps every 48 iteration.
		for (int i = 0; i < 1000000000%48; ++i) {
			file.close();
			file.open(filename); 
			programs = dance(programs, split_input(file));
		}

		std::cout << "Part 2: " << programs << "\n";

	} else {
		std::cout << "No input!\n";
	}

	return 0;
}
