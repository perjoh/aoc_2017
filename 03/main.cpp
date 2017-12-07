#include <iostream>
#include <array>
#include <numeric>
#include <map>
#include <cassert>

struct Point 
{
	Point()
		: x(0)
		, y(0)
	{ }

	Point(int x_, int y_)
		: x(x_)
		, y(y_)
	{ }

	Point& operator+=(const Point& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Point operator+(const Point& other) const {
		Point result(*this);
		result += other;
		return result; 
	}

	Point& operator-=(const Point& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Point operator-(const Point& other) const {
		Point result(*this);
		result -= other;
		return result;
	}

	Point operator*(int value) const {
		return Point(x*value, y*value);
	}

	int distance_from(const Point& pt = Point()) const {
		auto pt_diff = *this - pt;
		return std::abs(pt_diff.x) + std::abs(pt_diff.y);
	}

	bool operator<(const Point& other) const {
		return x < other.x || (x == other.x && y < other.y);
	}

	int x, y; 
};

const Point up(0, 1);
const Point down(0, -1);
const Point right(1, 0);
const Point left(-1, 0);

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
	return	get_sum(sum_map, pt + up) +
			get_sum(sum_map, pt + up + right) +
			get_sum(sum_map, pt + right) +
			get_sum(sum_map, pt + right + down) +
			get_sum(sum_map, pt + down) +
			get_sum(sum_map, pt + down + left) +
			get_sum(sum_map, pt + left) +
			get_sum(sum_map, pt + left + up);
}

class Spiral_traverser 
{
public :
	template <typename Fun>
	static void traverse(Fun f) 
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
		const Point directions[4] = {right, up, left, down};
		p += directions[num_turns_%4];

		++step_counter_;

		if (0 == --num_steps_turn_) {
			++num_turns_;

			if (num_turns_&1) {
				++num_steps_;
			}

			num_steps_turn_ = num_steps_;
		} 
	}

private :
	int num_steps_{1};
	int num_steps_turn_{num_steps_};
	int num_turns_{0};
	int step_counter_{0};
};

int find_distance(int value) 
{
	Spiral_traverser traverser;
	traverser.traverse([&value](const Point& pt, int count)
	{ 
		if (count == value) {
			value = pt.dist
			return false;
		}

		return true;
	});
}

int find_distance(int value) { 
	
	Point pt;

	int counter = 1;
	int sum = 1;
	int num_steps = 1; 
	int num_turns = 0; 

	Sum_map sum_map;
	sum_map[Point()] = 1;

	while (counter < value) { 

		const Point directions[4] = {right, up, left, down};

		num_steps = std::min(num_steps, value - counter); 
		const Point& dir = directions[num_turns%4];

		for (int i = 0; i < num_steps; ++i) {
			pt += dir;

			const int sum = sum_points(sum_map, pt);

			if (sum > value) {
				return sum;
			}

			sum_map[pt] = sum;
			++counter;
		}
 
		// Number of steps increases by one every second turn taken in the spiral.
		if (num_turns&1) {
			++num_steps; 
		}

		++num_turns;
	}

	return pt.length();
}

int main()
{
	int value = 0;
	std::cin >> value;
	std::cout << find_distance(value) << "\n";

	return 0;
}
