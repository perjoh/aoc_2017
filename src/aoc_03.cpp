#include <iostream>
#include <fstream>
#include <array>
#include <numeric>
#include <map>
#include <cassert>
#include <functional>
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
			const Point directions[4] = {right, up, left, down};
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

	int find_distance(int value) 
	{
		Spiral_traverser traverser;
		traverser.traverse([&value](const Point& pt, int count)
		{ 
			if (count + 1 == value) {
				value = pt.distance_from();
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

		std::ifstream f("input/03/input.txt");
		if (f) {
			int value = 0;
			f >> value;
			std::cout << "Part 1: " << find_distance(value) << "\n";
			std::cout << "Part 2: " << find_value_greater(value) << "\n";
		}

		return 0;
	}
