#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>
#include <algorithm>

std::pair<int, int> find_min_max(const std::vector<int>& numbers)
{
	if (!numbers.empty()) {
		int min = numbers[0], max = numbers[0];

		for (auto& number : numbers) {
			min = std::min(min, number);
			max = std::max(max, number);
		}

		return std::make_pair(min, max);
	}

	return std::make_pair(0, 0);
}

int evenly_divide(const std::vector<int>& numbers) 
{
	for (std::size_t i = 0; i < numbers.size() - 1; ++i) {
		for (std::size_t j = i + 1; j < numbers.size(); ++j) {

			if (numbers[i]%numbers[j] == 0) { 
				return numbers[i]/numbers[j];
			} else if (numbers[j]%numbers[i] == 0) {
				return numbers[j]/numbers[i];
			}
		}
	}

	assert(false);
	return 0;
}

int main()
{
	std::ifstream file("input/aoc_02.txt");
	if (file) {
		std::string row;
		int sum = 0;
		int sum2 = 0;
		while (std::getline(file, row) && !row.empty()) {
			std::stringstream ss(row); 
			std::istream_iterator<int> it_begin(ss), it_end;
			std::vector<int> row_numbers(it_begin, it_end);
			auto min_max = find_min_max(row_numbers);
			sum += min_max.second - min_max.first;

			sum2 += evenly_divide(row_numbers);
		}

		std::cout << "Part 1: " << sum << "\n";
		std::cout << "Part 2: " << sum2 << "\n";
	}
	return 0;
}
