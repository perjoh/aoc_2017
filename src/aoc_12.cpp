#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <iterator>
#include <cassert>
#include <fstream>

using namespace std;

// 0 <-> 122, 874, 1940
vector<int> split(const string& s)
{
    stringstream ss(s);
    using is_iter = istream_iterator<string>;
    vector<string> result_str((is_iter(ss)), is_iter());

    assert(result_str.size() >= 3);

    result_str.erase(result_str.begin() + 1);

    vector<int> result;
    for (auto& str : result_str) {
        ss.clear();
        ss.str(str);
        istream_iterator<int> it(ss);
        result.push_back(*it);
    } 

    return result;
}

// Recursive
set<int> populate_group(const vector<vector<int>>& graph, int i, set<int> group = set<int>())
{
    for (auto& value : graph[i]) { 
        if (group.count(value) == 0) {
            group.insert(value);
            if (value != i) {
                auto sub_set = populate_group(graph, value, group);
                group.insert(cbegin(sub_set), cend(sub_set));
            }
        }
    }

    return group;
}    

bool in_any_of(const vector<set<int>>& groups, int value)
{
    for (auto& group : groups) {
        if (group.count(value) != 0) {
            return true;
        }
    }

    return false;
}

int main()
{
    {
        auto values = split("0 <-> 122, 874, 1940");
        assert(values[0] == 0);
        assert(values[1] == 122);
        assert(values[2] == 874);
        assert(values[3] == 1940); 
    }

    const char* filename = "input/aoc_12.txt";
    ifstream file(filename);
    if (file) {

        vector<vector<int>> graph;

        string input;
        while (getline(file, input)) {
            graph.push_back(split(input));
        }
        
        vector<set<int>> groups;

        for (auto& values : graph) {
            auto value = values[0];
            if (!in_any_of(groups, value)) {
                auto group = populate_group(graph, value);
                groups.push_back(group);
            }
        }

        cout << "Part 1: " << groups[0].size() << "\n";
        cout << "Part 2: " << groups.size() << "\n";
    }

    return 0;
}
