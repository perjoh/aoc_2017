#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>
#include <iterator>

using namespace std;

void redistribute(vector<int>& mem_banks)
{
    auto it_max = max_element(begin(mem_banks), end(mem_banks));
    size_t i = it_max - begin(mem_banks);

    int value = *it_max;
    *it_max = 0;

    const int div = value/mem_banks.size();
    int rest = value%mem_banks.size();

    for (size_t it = 0; it < mem_banks.size(); ++it) {
        size_t ptr = (i + it + 1)%mem_banks.size();
        mem_banks[ptr] += div;

        if (rest != 0) {
            ++mem_banks[ptr];
            --rest;
        }
    } 
}

pair<int, int> count_redistributions(vector<int> mem_banks)
{
    set<vector<int>> mem_banks_prev;
    
    int count = 0;
    while (mem_banks_prev.insert(mem_banks).second) {
        redistribute(mem_banks); 
        ++count;
    }    

    mem_banks_prev.clear();

    int count_cycle = 0;
    while (mem_banks_prev.insert(mem_banks).second) { 
        redistribute(mem_banks); 
        ++count_cycle;
    }    

    return make_pair(count, count_cycle);
}

void test()
{
    vector<int> mem_banks{0, 2, 7, 0};
    assert(count_redistributions(mem_banks).first == 5);
}

int main()
{
    test();

    ifstream file("input/aoc_06.txt");
    if (file) {
        string input;
        if (getline(file, input)) {
            stringstream ss(input);
            //istream_iterator<int> it_begin(ss), it_end();
            vector<int> mem_banks((istream_iterator<int>(ss)), istream_iterator<int>()); 

            auto p = count_redistributions(mem_banks);
            cout << "Part 1: " << p.first << "\n";
            cout << "Part 2: " << p.second << "\n";
        }
    }

    return 0;
}
