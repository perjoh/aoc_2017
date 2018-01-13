#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <map>
#include <algorithm>
#include <tuple>

using namespace std;

using Point = tuple<int, int>;

Point operator+(const Point& lhs, const Point& rhs)
{
    return Point{get<0>(lhs) + get<0>(rhs), get<1>(lhs) + get<1>(rhs)};
}

Point& operator+=(Point& lhs, const Point& rhs)
{
    lhs = lhs + rhs;
    return lhs; 
}

const Point up{ 0, 1 };
const Point down{ 0, -1 };
const Point right{ 1, 0 };
const Point left{ -1, 0 };

Point to_numeric(const string& dir)
{
    if (dir == "n") return ::up + ::right;
    if (dir == "ne") return ::right;
    if (dir == "nw") return ::up;
    if (dir == "s") return ::down + ::left;
    if (dir == "se") return ::down;
    if (dir == "sw") return ::left;

    return Point();
}

int distance(const Point& pt)
{
    return max(abs(get<0>(pt)), abs(get<1>(pt)));
} 

template <typename Stream>
pair<Point, int> walk(Stream& stream)
{
    Point target;
    int max_distance = 0;

    string dir;
    while (getline(stream, dir, ',')) {
        target += to_numeric(dir);

        const int cur_distance = distance(target);
        if (max_distance < cur_distance) {
            max_distance = cur_distance;
        }
    }

    return make_pair(target, max_distance);
}

int count_steps_nearest(const string& steps)
{
    stringstream ss(steps);
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

    ifstream file("input/aoc_11.txt");
    if (file) { 
        auto result = walk(file);
        cout << "Part 1: " << distance(result.first) << "\n";
        cout << "Part 2: " << result.second << "\n";
    }

    return 0;
}
