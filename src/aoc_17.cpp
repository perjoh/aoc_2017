#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cassert>

using namespace std;

vector<size_t> process_vortex(size_t num_iterations, size_t step)
{
    vector<size_t> memory;
    memory.reserve(num_iterations + 1);
    memory.push_back(0);

    size_t current_pos = 0;

    for (size_t i = 0; i < num_iterations; ++i) {
        current_pos = (current_pos + step)%memory.size() + 1;
        memory.insert(memory.begin() + current_pos, i + 1);
    }

    return memory;
}

size_t process_vortex_part2(size_t num_iterations, size_t step)
{
    size_t pos = 0;
    size_t value = 0;

    for (size_t i = 0; i < num_iterations; ++i) {
        const size_t container_size = i + 1;
        pos = (pos + step)%container_size + 1;
        if (pos == 1) {
            value = container_size;
            std::cout << value << "\n";
        }
    }

    return value;
}

size_t find_value_after(const vector<size_t>& memory, size_t value)
{
    auto it = std::find(memory.cbegin(), memory.cend(), value);
    if (it != memory.cend()) {
        size_t offset = it - memory.cbegin();
        return memory[(offset + 1)%memory.size()];
    }

    return 0;
}

void test()
{
    auto memory = process_vortex(2017, 3);
    assert(find_value_after(memory, 2017) == 638);
    size_t value = process_vortex_part2(2017, 3);
    assert(value == memory[1]);
}

int main()
{
    test();

    ifstream file("input/aoc_17.txt");
    if (file) {
        istream_iterator<int> it(file);
        const int step_value = *it;

        auto memory = process_vortex(2017, step_value); 
        cout << "Part 1: " << find_value_after(memory, 2017) << "\n";

        cout << "Part 2: " << process_vortex_part2(50'000'000, step_value) << "\n";
    }
    
    return 0;
}
