#include <iostream>
#include <fstream>
#include <tuple>
#include <cassert>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;
using Component = tuple<int, int>;

struct Node 
{
    Component c{0, 0};
    vector<shared_ptr<Node>> sub_nodes;
};

template <typename Stream>
auto read_components(Stream& stream)
{
    vector<Component> result;

    string input;
    while (getline(stream, input)) {
        stringstream ss(input);

        Component c;
        ss >> get<0>(c);
        { char dummy; ss >> dummy; }
        ss >> get<1>(c);

        result.push_back(c);
    }

    return result;
}

shared_ptr<Node> build_tree(shared_ptr<Node> parent, const vector<Component>& comps, int port)
{
    for (auto it = comps.begin(); it != comps.end(); ++it) { 
        const Component c = *it;
        if (get<0>(c) == port || get<1>(c) == port) { 
            vector<Component> comps_copy(comps);
            comps_copy.erase(comps_copy.begin() + (it - comps.begin()));

            auto new_node = make_shared<Node>();
            new_node->c = c;
            const int new_port = (get<0>(c) == port) ? get<1>(c) : get<0>(c); 
            build_tree(new_node, comps_copy, new_port);
            parent->sub_nodes.push_back(new_node);
        } 
    }

    return parent;
}

int strength(const Component& c)
{ 
    return get<0>(c) + get<1>(c);
}

int find_max_strength(const shared_ptr<Node>& node)
{ 
    if (node) {
        int max_strength = 0;
        for (auto& sub_node : node->sub_nodes) {
            const int strength = find_max_strength(sub_node);
            if (max_strength < strength) {
                max_strength = strength;
            } 
        }

        return max_strength + strength(node->c);
    }

    return 0;
}

tuple<int, int> operator+(const tuple<int, int>& a, const tuple<int, int>& b)
{
    return tuple<int, int>(get<0>(a) + get<0>(b), get<1>(a) + get<1>(b));
}

tuple<int, int> find_strength_longest(const shared_ptr<Node>& node, int depth = 0)
{ 
    if (node) {
        tuple<int, int> max{0, 0};
        for (const auto& sub_node : node->sub_nodes) {
            auto result = find_strength_longest(sub_node, depth + 1);
            if (max < result) {
                max = result;
            }
        }

        return max + tuple<int, int>(depth, strength(node->c));
    } 

    return { 0, 0 };
}

void test()
{
    stringstream ss("0/2\n"
                    "2/2\n"
                    "2/3\n"
                    "3/4\n"
                    "3/5\n"
                    "0/1\n"
                    "10/1\n"
                    "9/10\n");

    auto comps = read_components(ss);
    auto root = build_tree(make_shared<Node>(), comps, 0);
    assert(find_max_strength(root) == 31); 
    assert(get<1>(find_strength_longest(root)) == 19);
}

int main()
{
    test();

    ifstream file("input/aoc_24.txt");
    if (file) {
        auto comps = read_components(file);
        auto root = build_tree(make_shared<Node>(), comps, 0);
        cout << "Part 1: " << find_max_strength(root) << '\n';
        cout << "Part 2: " << get<1>(find_strength_longest(root)) << '\n';
    }

    return 0;
}
