#include <iostream>
#include <cassert>
#include <regex>
#include <fstream>
#include <sstream>

long long produce(long long prev_value, long long factor)
{
	return prev_value*factor%2147483647;
}

int count_part1(long long value_a, 
				long long factor_a, 
				long long value_b, 
				long long factor_b)
{
	int count = 0;

	for (int i = 0; i < 40000000; ++i) {
		value_a = produce(value_a, factor_a); 
		value_b = produce(value_b, factor_b); 

		if ((value_a&0xffff) == (value_b&0xffff)) {
			++count;
		} 
	}

	return count;
}

long long produce_a(long long value, long long factor)
{
	do {
		value = produce(value, factor);
	} while ((value&3) != 0);
	
	return value;
}

long long produce_b(long long value, long long factor)
{
	do {
		value = produce(value, factor);
	} while ((value&7) != 0);

	return value;
}

int count_part2(long long value_a,
				long long factor_a,
				long long value_b,
				long long factor_b)
{
	int num_matches = 0;
	int counter = 0;

	const int num_iterations = 5000000;
	while (counter < num_iterations) {

		value_a = produce_a(value_a, factor_a);
		value_b = produce_b(value_b, factor_b);

		if ((value_a&0xffff) == (value_b&0xffff)) {
			++num_matches;
		}

		++counter;
	}

	return num_matches;
}

int to_int(const std::string& value)
{
	std::stringstream ss(value);
	int result = 0;
	ss >> result;
	return result;
}

int main()
{
	assert(count_part1(65, 16807, 8921, 48271) == 588);
	assert(count_part2(65, 16807, 8921, 48271) == 309);

	{
		long long value_a = 65;
		long long value_b = 8921;

		long long expected_a[5] = { 1352636452, 1992081072, 530830436, 1980017072, 740335192 };
		long long expected_b[5] = { 1233683848, 862516352, 1159784568, 1616057672, 412269392 };

		for (int i = 0; i < 5; ++i) {
			value_a = produce_a(value_a, 16807);
			value_b = produce_b(value_b, 48271);
			assert(value_a == expected_a[i]);
			assert(value_b == expected_b[i]);
		}
	}

	std::ifstream file("input/aoc_15.txt");
	if (file) {
		
		long long value_a = 0;
		long long value_b = 0;

		std::string input;
		while (std::getline(file, input)) {
			static std::regex input_regex("Generator (A|B) starts with (\\d+)");
			std::smatch match;
			if (std::regex_search(input, match, input_regex)) {
				assert(match.size() == 3);
				if (match[1] == 'A') {
					value_a = to_int(match[2]);
				} else if (match[1] == 'B') {
					value_b = to_int(match[2]);
				}
			}
		} 
		std::cout << "Part 1: " << count_part1(value_a, 16807, value_b, 48271) << "\n";
		std::cout << "Part 2: " << count_part2(value_a, 16807, value_b, 48271) << "\n";
	} 

	return 0;
}
