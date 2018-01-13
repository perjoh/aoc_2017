#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>
#include <cassert>

using namespace std;

using string_vec = vector<string>;

template <typename Stream>
auto split_input(Stream& stream)
{
    string_vec result;
    string input;
    while (getline(stream, input, ',') && !input.empty()) {
        result.push_back(input);
    }
    return result;
}

auto split_input(const char* input)
{
    stringstream ss(input);
    return split_input(ss);
}

string spin(string programs, int value)
{
    string result(programs.cend() - value, programs.cend());
    result.insert(result.end(), programs.cbegin(), programs.cbegin() + programs.length() - value);
    return result;
}

string exchange(string programs, int a, int b)
{
    swap(programs[a], programs[b]);
    return programs;
}

string partner(string programs, char a, char b)
{
    auto it_a = find(programs.begin(), programs.end(), a);
    auto it_b = find(programs.begin(), programs.end(), b);

    assert(it_a != programs.end());
    assert(it_b != programs.end());

    swap(*it_a, *it_b);

    return programs;
}

int to_int(const string& s)
{
    stringstream ss(s);
    int value = 0;
    ss >> value;
    return value;
}

string dance(string programs, const string_vec& dance_moves)
{
    for (auto& dance_move : dance_moves) {
        static const regex s_regex("s(\\d+)");
        static const regex x_regex("x(\\d+)/(\\d+)");
        static const regex p_regex("p([a-p])/([a-p])");

        smatch match;

        if (regex_match(dance_move, match, s_regex)) {
            programs = spin(programs, to_int(match[1]));
        } else if (regex_match(dance_move, match, x_regex)) {
            programs = exchange(programs, to_int(match[1]), to_int(match[2]));
        } else if (regex_match(dance_move, match, p_regex)) {
            programs = partner(programs, match[1].str()[0], match[2].str()[0]);
        } else {
            assert(false);
        }
    }

    return programs;
}

void test()
{
    string programs("abcde");
    programs = dance(programs, split_input("s1,x3/4,pe/b"));
    assert(programs == "baedc");
}

int main()
{
    test();

    const char* filename = "input/aoc_16.txt";
    ifstream file(filename);
    if (file) {
        string programs("abcdefghijklmnop");
        const string org_order = programs;
        programs = dance(programs, split_input(file));
        cout << "Part 1: " << programs << "\n";

        programs = org_order;

        // Testing reveals that the pattern wraps every 48 iteration.
        for (int i = 0; i < 1000000000%48; ++i) {
            file.close();
            file.open(filename); 
            programs = dance(programs, split_input(file));
        }

        cout << "Part 2: " << programs << "\n";
    }

    return 0;
}
