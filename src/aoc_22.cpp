#include <iostream>
#include <tuple>
#include <set>
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

using Point = tuple<int, int>;
using Point_set = set<Point>; 

struct Virus_carrier
{ 
    Point position{0, 0};
    Point direction{0, -1}; 
};

Point& operator+=(Point& a, const Point& b)
{
    get<0>(a) += get<0>(b);
    get<1>(a) += get<1>(b);
    return a;
}

Point turn_right(Point direction)
{
    // -y, x
    return make_tuple(-get<1>(direction), get<0>(direction));
}

Point turn_left(Point direction)
{
    return turn_right(turn_right(turn_right(direction)));
}

// Returns true if an infection is caused.
bool update_carrier(Point_set& grid, Virus_carrier& carrier)
{ 
    auto it = grid.find(carrier.position);
    if (it != grid.end()) {
        // infected
        grid.erase(it);
        carrier.direction = turn_right(carrier.direction);
    } else {
        grid.insert(carrier.position);
        carrier.direction = turn_left(carrier.direction);
    }

    carrier.position += carrier.direction;

    return it == grid.end();
}

enum Node_state { CLEAN, WEAKENED, INFECTED, FLAGGED };

Point update_direction(Point direction, Node_state node_state)
{
    switch (node_state) {
        case CLEAN:
            return turn_left(direction);
        case INFECTED:
            return turn_right(direction);
        case FLAGGED:
            return turn_right(turn_right(direction));
        case WEAKENED:
            return direction;
    };

    return direction;
}

template <typename Stream>
Point_set parse_grid(Stream& stream)
{ 
    Point_set grid;

    int y = 0;
    string input;
    while (getline(stream, input)) {
        int width = input.length();
        for (int x = 0; x < width; ++x) {
            if (input[x] == '#') { 
                // assume square
                grid.insert(make_tuple(x - width/2, y - width/2));
            }
        }

        ++y;
    }

    return grid;
}

int count_infections(Point_set grid, size_t iterations)
{
    Virus_carrier carrier;

    size_t num_infections = 0;
    while (iterations--) {
        if (update_carrier(grid, carrier)) {
            ++num_infections;
        } 
    }

    return num_infections;
}

int count_infections_part_deux(Point_set grid, size_t iterations)
{
    Virus_carrier carrier;

    Point_set infected(grid);
    Point_set flagged;
    Point_set weakened;

    size_t num_infections = 0;
    while (iterations--) {

        Node_state prev_state = CLEAN;

        auto it = weakened.find(carrier.position);
        if (it != weakened.end()) {
            weakened.erase(it);
            infected.insert(carrier.position);
            prev_state = WEAKENED;
            ++num_infections;
        } else {
            it = flagged.find(carrier.position);
            if (it != flagged.end()) {
                flagged.erase(it);
                prev_state = FLAGGED;
            } else {
                it = infected.find(carrier.position);
                if (it != infected.end()) {
                    infected.erase(it);
                    flagged.insert(carrier.position);
                    prev_state = INFECTED;
                } else {
                    weakened.insert(carrier.position);
                    prev_state = CLEAN;
                }
            }
        } 

        carrier.direction = update_direction(carrier.direction, prev_state);
        carrier.position += carrier.direction;
    }

    return num_infections;
}

void test()
{
    stringstream ss("..#\n"
                    "#..\n"
                    "...\n");
    auto grid = parse_grid(ss);
    assert(grid.size() == 2);
    assert(count_infections(grid, 10000) == 5587); 
    //assert(count_infections_part_deux(grid, 100) == 26);
    //cout << count_infections_part_deux(grid, 10000000) << '\n';
}

int main()
{
    test();

    ifstream file("input/aoc_22.txt");
    if (file) {
        auto grid = parse_grid(file);
        cout << "Part 1: " << count_infections(grid, 10000) << '\n';
        cout << "Part 1: " << count_infections_part_deux(grid, 10000000) << '\n';
    } 

    return 0;
}
