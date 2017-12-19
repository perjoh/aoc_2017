#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <vector>

using namespace std;

using Maze = vector<string>;

template <typename Stream>
Maze read_maze(Stream& strm)
{
	Maze maze;

	string input;
	while (getline(strm, input) && !input.empty()) {
		maze.push_back(input);
	}

	const size_t len = maze[0].length();
	const size_t num_uniform = count_if(maze.begin(), maze.end(), [len](const string& s){ return s.length() == len; }); 
	assert(maze.size() == num_uniform);

	return maze;
}

using Pos = tuple<int, int>;

Pos operator+(const Pos& a, const Pos& b)
{
	return Pos(get<0>(a) + get<0>(b), get<1>(a) + get<1>(b));
}

bool inside(const Maze& maze, const Pos& pos)
{
	return	(0 <= get<0>(pos) && get<0>(pos) < static_cast<int>(maze[0].length())) &&
			(0 <= get<1>(pos) && get<1>(pos) < static_cast<int>(maze.size()));
}

char get_tile(const vector<string>& maze, const Pos& pos)
{
	if (inside(maze, pos)) {
		return maze[get<1>(pos)][get<0>(pos)];
	}

	return ' ';
}

Pos rotate_cw(const Pos& pos)
{
	return make_tuple(-get<1>(pos), get<0>(pos));
}

Pos rotate_ccw(const Pos& pos)
{
	return rotate_cw(rotate_cw(rotate_cw(pos)));
}

char inverse_tile(char tile)
{
	if (tile == '|') {
		return '-';
	} else if (tile == '-') {
		return '|';
	}

	return tile;
}

pair<string, int> traverse_maze(const vector<string>& maze)
{
	string result;

	char expected_tile = '|';
	Pos pos(find(maze[0].begin(), maze[0].end(), expected_tile) - maze[0].begin(), 0);
	Pos dir(0, 1); // down
	int num_steps = 0;

	while (inside(maze, pos)) {
		const char tile = get_tile(maze, pos);
		if (tile == '+') {
			char right_tile = get_tile(maze, pos + rotate_cw(dir));
			char left_tile = get_tile(maze, pos + rotate_ccw(dir));

			if (left_tile == ' ' || right_tile == inverse_tile(expected_tile) || ('A' <= right_tile && right_tile <= 'Z')) {
				dir = rotate_cw(dir); // go right
			} else {
				dir = rotate_ccw(dir); // go left
			} 

			expected_tile = inverse_tile(expected_tile);

		} else if ('A' <= tile && tile <= 'Z') {
			result.push_back(tile);
		} else if (tile == ' ') {
			break;
		}

		pos = pos + dir;
		++num_steps;
	}

	return make_pair(result, num_steps);
}

void test()
{
	const char* maze_data = "     |          \n"
							"     |  +--+    \n"
							"     A  |  C    \n"
							" F---|----E|--+ \n"
							"     |  |  |  D \n"
							"     +B-+  +--+ \n";

	stringstream ss(maze_data);
	auto maze = read_maze(ss);
	assert(traverse_maze(maze).first == string("ABCDEF"));
}

int main()
{
	test();

	ifstream file("input/aoc_19.txt");
	if (file) {
		auto maze = read_maze(file);

		auto result = traverse_maze(maze);
		cout << "Part 1: " << result.first << "\n";
		cout << "Part 2: " << result.second << "\n";
	} 

	return 0;
}
