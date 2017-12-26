#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <iterator>
#include <cassert>
#include <fstream>

// 0 <-> 122, 874, 1940
std::vector<int> split(const std::string& s)
{
    std::stringstream ss(s);
    using is_iter = std::istream_iterator<std::string>;
    std::vector<std::string> result_str((is_iter(ss)), is_iter());

    assert(result_str.size() >= 3);

    result_str.erase(result_str.begin() + 1);

    std::vector<int> result;
    for (auto& str : result_str) {
        ss.clear();
        ss.str(str);
        std::istream_iterator<int> it(ss);
        result.push_back(*it);
    } 

    return result;
}

// Recursive
std::set<int> populate_group(const std::vector<std::vector<int>>& graph, int i, std::set<int> group = std::set<int>())
{
    for (auto& value : graph[i]) { 
        if (group.count(value) == 0) {
            group.insert(value);
            if (value != i) {
                auto sub_set = populate_group(graph, value, group);
                group.insert(std::cbegin(sub_set), std::cend(sub_set));
            }
        }
    }

    return group;
}    

bool in_any_of(const std::vector<std::set<int>>& groups, int value)
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
    std::ifstream file(filename);
    if (file) {

        std::vector<std::vector<int>> graph;

        std::string input;
        while (std::getline(file, input)) {
            graph.push_back(split(input));
        }
        
        std::vector<std::set<int>> groups;

        for (auto& values : graph) {
            auto value = values[0];
            if (!in_any_of(groups, value)) {
                auto group = populate_group(graph, value);
                groups.push_back(group);
            }
        }


        /*std::set<int> group;
        group.insert(0);

        for (int i = 0; i < 20; ++i) {
            std::string input;
            while (std::getline(file, input)) {
                auto values = split(input);
                for (auto& value : values ) {
                    if (group.count(value) == 1) {
                        group.insert(values.begin(), values.end());
                        break;
                    }
                }
            }

            file.close();
            file.open(filename);
        }*/

        std::cout << "Part 1: " << groups[0].size() << "\n";
        std::cout << "Part 2: " << groups.size() << "\n";
    } else {
        std::cout << "No input!\n";
    }

    return 0;
}
