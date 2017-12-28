#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

struct Layer
{
    Layer(int depth_, int range_)
        : depth(depth_)
        , range(range_)
    { 
    }

    int depth{0};
    int range{0};
};

template <typename Stream>
auto read_layers(Stream& stream)
{
    vector<Layer> layers;

    string input;
    while (getline(stream, input) && !input.empty()) {
        stringstream ss(input);
        int depth = 0; ss >> depth;

        {string colon; ss >> colon;}

        int range = 0; ss >> range;

        layers.push_back(Layer(depth, range));
    }

    return layers;
}

bool is_caught(const Layer& layer, int delay = 0)
{
    return (layer.depth + delay) % ((layer.range - 1) * 2) == 0;
}

int calc_score(vector<Layer> layers)
{
    int score = 0; 
    for (auto& layer : layers) { 
        if (is_caught(layer)) {
            score += layer.depth * layer.range; 
        }
    }

    return score; 
}

int find_delay(const vector<Layer>& layers)
{
    int delay = 1;

    for (;;) { 
        auto it = find_if(layers.cbegin(), layers.cend(), [delay](const Layer& layer) { return is_caught(layer, delay); });
        if (it == layers.cend()) {
            return delay;
        }

        ++delay;
    }

    return 0;
}

void test()
{
    stringstream ss("0: 3\n"
                    "1: 2\n"
                    "4: 4\n"
                    "6: 4\n");
    auto layers = read_layers(ss);

    assert(calc_score(layers) == 24);
    assert(find_delay(layers) == 10);
}

int main()
{
    test();

    ifstream file("input/aoc_13.txt");
    if (file) { 
        auto layers = read_layers(file);
        cout << "Part 1: " << calc_score(layers) << "\n";
        cout << "Part 2: " << find_delay(layers) << "\n";
    }

    return 0;
}
