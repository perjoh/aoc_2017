#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <map>
#include <algorithm>
#include "point.hpp" 

Point to_numeric(const std::string& dir)
{
	if (dir == "n") return Point::up() + Point::right();
	if (dir == "ne") return Point::right();
	if (dir == "nw") return Point::up();
	if (dir == "s") return Point::down() + Point::left();
	if (dir == "se") return Point::down();
	if (dir == "sw") return Point::left();

	return Point();
}

int distance(const Point& pt)
{
	return std::max(std::abs(pt.x), std::abs(pt.y));
} 

template <typename Stream>
std::pair<Point, int> walk(Stream& stream)
{
	Point target;
	int max_distance = 0;

	std::string dir;
	while (std::getline(stream, dir, ',')) {
		target += to_numeric(dir);

		const int cur_distance = distance(target);
		if (max_distance < cur_distance) {
			max_distance = cur_distance;
		}
	}

	return std::make_pair(target, max_distance);
}

int count_steps_nearest(const std::string& steps)
{
	std::stringstream ss(steps);
	const Point target = walk(ss).first; 
	return distance(target);
}

int main()
{
/*ne,ne,ne is 3 steps away.
ne,ne,sw,sw is 0 steps away (back where you started).
ne,ne,s,s is 2 steps away (se,se).
se,sw,se,sw,sw is 3 steps away (s,s,sw).*/

	assert(count_steps_nearest("ne,ne,ne") == 3);
	assert(count_steps_nearest("ne,ne,sw,sw") == 0);
	assert(count_steps_nearest("ne,ne,s,s") == 2);
	assert(count_steps_nearest("se,sw,se,sw,sw") == 3);

	std::ifstream file("input/aoc_11.txt");
	if (file) { 
		auto result = walk(file);
		std::cout << "Part 1: " << distance(result.first) << "\n";
		std::cout << "Part 2: " << result.second << "\n";
	} else {
		std::cout << "No input!\n";
	}

	return 0;
}
