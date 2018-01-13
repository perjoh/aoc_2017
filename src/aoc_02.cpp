#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

pair<int, int> find_min_max(const vector<int>& numbers)
{
    if (!numbers.empty()) {
        int min = numbers[0], max = numbers[0];

        for (auto& number : numbers) {
            min = min(min, number);
            max = max(max, number);
        }

        return make_pair(min, max);
    }

    return make_pair(0, 0);
}

int evenly_divide(const vector<int>& numbers) 
{
    for (size_t i = 0; i < numbers.size() - 1; ++i) {
        for (size_t j = i + 1; j < numbers.size(); ++j) {

            if (numbers[i]%numbers[j] == 0) { 
                return numbers[i]/numbers[j];
            } else if (numbers[j]%numbers[i] == 0) {
                return numbers[j]/numbers[i];
            }
        }
    }

    assert(false);
    return 0;
}

int main()
{
    ifstream file("input/aoc_02.txt");
    if (file) {
        string row;
        int sum = 0;
        int sum2 = 0;
        while (getline(file, row) && !row.empty()) {
            stringstream ss(row); 
            istream_iterator<int> it_begin(ss), it_end;
            vector<int> row_numbers(it_begin, it_end);
            auto min_max = find_min_max(row_numbers);
            sum += min_max.second - min_max.first;

            sum2 += evenly_divide(row_numbers);
        }

        cout << "Part 1: " << sum << "\n";
        cout << "Part 2: " << sum2 << "\n";
    }
    return 0;
}
