#include <iostream>
#include <fstream>
#include <array>
#include <numeric>
#include <map>
#include <cassert>
#include <functional>
#include <cassert>
#include "point.hpp"

using Sum_map = std::map<Point, int>;

int get_sum(const Sum_map& sum_map, const Point& pt)
{
	auto it = sum_map.find(pt);
	if (it != sum_map.end()) {
		return it->second;
	}

	return 0;
}

int sum_points(const Sum_map& sum_map, const Point& pt)
{ 
	return	get_sum(sum_map, pt + Point::up()) +
			get_sum(sum_map, pt + Point::up() + Point::right()) +
			get_sum(sum_map, pt + Point::right()) +
			get_sum(sum_map, pt + Point::right() + Point::down()) +
			get_sum(sum_map, pt + Point::down()) +
			get_sum(sum_map, pt + Point::down() + Point::left()) +
			get_sum(sum_map, pt + Point::left()) +
			get_sum(sum_map, pt + Point::left() + Point::up());
}

class Spiral_traverser 
{
public :
	static void traverse(std::function<bool(const Point&, int)> f) 
	{
		Point pt;
		Spiral_traverser traverser;

		while (f(pt, traverser.steps_taken())) {
			traverser.step(pt);
		}
	}

	int steps_taken() const {
		return step_counter_;
	}

private :
	void step(Point& p) 
	{ 
		const Point directions[4] = {Point::right(), Point::up(), Point::left(), Point::down()};
		p += directions[num_turns_%4];

		++step_counter_;

		if (0 == --num_steps_turn_) {

			if (num_turns_&1) {
				++num_steps_;
			}

			++num_turns_;

			num_steps_turn_ = num_steps_;
		} 
	}

private :
	int num_steps_{1};
	int num_steps_turn_{num_steps_};
	int num_turns_{0};
	int step_counter_{0};
};

int manhattan_distance(const Point& pt)
{
	return std::abs(pt.x) + std::abs(pt.y);
}

int find_distance(int value) 
{
	Spiral_traverser traverser;
	traverser.traverse([&value](const Point& pt, int count)
	{ 
		if (count + 1 == value) {
			value = manhattan_distance(pt);
			return false;
		}

		return true;
	});

	return value;
}

int find_value_greater(int value) 
{
	Sum_map sum_map;

	Spiral_traverser traverser;
	traverser.traverse([&value, &sum_map](const Point& pt, int count)
	{
		if (count == 0) {
			sum_map[pt] = 1;
		} else {
			const int new_value = sum_points(sum_map, pt);

			if (value < new_value) {
				value = new_value;
				return false; 
			}
			
			sum_map[pt] = new_value;
		}

		return true;
	});

	return value;
}

int main()
{
	assert(find_distance(1) == 0);
	assert(find_distance(12) == 3);
	assert(find_distance(23) == 2);
	assert(find_distance(1024) == 31);

	assert(find_value_greater(1) == 2);
	assert(find_value_greater(2) == 4);
	assert(find_value_greater(400) == 747);
	assert(find_value_greater(800) == 806);

	std::ifstream f("input/aoc_03.txt");
	if (f) {
		int value = 0;
		f >> value;
		std::cout << "Part 1: " << find_distance(value) << "\n";
		std::cout << "Part 2: " << find_value_greater(value) << "\n";
	}

	return 0;
}
