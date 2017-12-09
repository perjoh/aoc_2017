#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <fstream>
#include <cassert>
#include <functional>

using namespace std;

int inc_part2(int value)
{
	if (value < 3) {
		return value + 1;
	}

	return value - 1;
}

int count_jumps(vector<int> jumps, 
				const std::function<int(int)>& inc = [](int v){ return v+1; }) 
{
	size_t i = 0;
	int counter = 0;

	while (0 <= i && i < jumps.size()) {
		int& step = jumps[i];
		i += step;
		step = inc(step);
		++counter;
	}

	return counter;
}

int main()
{
	assert(count_jumps({0, 3, 0, 1, -3}) == 5);
	assert(count_jumps({0, 3, 0, 1, -3}, &inc_part2) == 10);

	ifstream file("input/05/input.txt");
	if (file) { 
		istream_iterator<int> file_begin(file), file_end;
		vector<int> maze(file_begin, file_end);
		cout << "Part 1: " << count_jumps(maze) << "\n"; 
		cout << "Part 2: " << count_jumps(maze, &inc_part2) << "\n"; 
	} else {
		cout << "No input!\n";
	}

	return 0;
}
