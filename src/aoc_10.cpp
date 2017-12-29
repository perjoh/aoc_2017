#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>

using namespace std;

vector<unsigned char> split_str(const string& s)
{
    vector<unsigned char> result;
    stringstream ss(s); 
    string input;
    while (getline(ss, input, ',')) {
        result.push_back(static_cast<unsigned char>(stoi(input)));
    }

    return result;
}

auto start_buffer()
{
    vector<unsigned char> buf(256);
    generate(buf.begin(), buf.end(), [n = 0]() mutable { return static_cast<unsigned char>(n++); });
    return buf;
}

auto process_list(const vector<unsigned char>& lengths, vector<unsigned char> target_buf = start_buffer(), size_t num_rounds = 1)
{
    size_t pos = 0;
    size_t skip_size = 0;

    const size_t size = target_buf.size();

    while (num_rounds--) {
        for (auto& length : lengths) {

            vector<unsigned char> work_buf(length);

            for (size_t i = 0; i < work_buf.size(); ++i) { 
                work_buf[i] = target_buf[(pos + i) % size];
            }

            reverse(work_buf.begin(), work_buf.end());

            for (size_t i = 0; i < work_buf.size(); ++i) {
                target_buf[(pos + i) % size] = work_buf[i];
            } 

            pos = (pos + length + skip_size) % size;
            ++skip_size;
        }
    }

    return target_buf;
}

int calc_checksum(const vector<unsigned char>& circular_list)
{
    if (circular_list.size() > 1) {
        return circular_list[0] * circular_list[1];
    }

    return 0;
}

auto append_suffix(vector<unsigned char> list)
{
    vector<unsigned char> suffix{17, 31, 73, 47, 23};
    list.insert(list.end(), suffix.begin(), suffix.end());
    return list;
}

auto append_suffix(const string& s)
{
    vector<unsigned char> buf(reinterpret_cast<const unsigned char*>(&s[0]), reinterpret_cast<const unsigned char*>(&s[0] + s.length()));
    return append_suffix(buf);
}

template <typename Iterator>
unsigned char reduce_row(Iterator beg, Iterator end)
{
    return accumulate(	beg + 1, 
                        end, 
                        *beg, 
                        [](unsigned char a, unsigned char b) -> unsigned char { return a^b; });

}

auto reduce_hash(vector<unsigned char> sparse_hash)
{
    assert(sparse_hash.size() == 256);

    const size_t block_size = 16;
    vector<unsigned char> dense_hash(block_size);

    auto it_sparse = sparse_hash.begin();
    for (size_t i = 0; i < block_size; ++i) { 
        dense_hash[i] = reduce_row(it_sparse, it_sparse + block_size);
        it_sparse += block_size;
    }

    return dense_hash;
}

string hexify(const vector<unsigned char>& dense_hash)
{
    string result;
    for (auto c : dense_hash) {
        const char* hex_chars = "0123456789abcdef";
        result.push_back(hex_chars[(c>>4)&0xf]);
        result.push_back(hex_chars[c&0xf]);
    }

    return result;
}

void test()
{
    vector<unsigned char> circular_list{0, 1, 2, 3, 4};
    const vector<unsigned char> lengths{3, 4, 1, 5};
    assert(calc_checksum(process_list(lengths, circular_list)) == 12);

    assert(hexify(reduce_hash(process_list(append_suffix(string("")), start_buffer(), 64))) == string("a2582a3a0e66e6e86e3812dcb672a272"));
    assert(hexify(reduce_hash(process_list(append_suffix(string("AoC 2017")), start_buffer(), 64))) == string("33efeb34ea91902bb2f59c9920caa6cd"));
    assert(hexify(reduce_hash(process_list(append_suffix(string("1,2,3")), start_buffer(), 64))) == string("3efbe78a8d82f29979031a4aa0b16a9d"));
    assert(hexify(reduce_hash(process_list(append_suffix(string("1,2,4")), start_buffer(), 64))) == string("63960835bcdc130f0b66d7ff4f6a5a8e")); 
}

int main()
{
    test();

    ifstream file("input/aoc_10.txt");
    if (file) {
        string input;
        if (getline(file, input)) { 
            auto lengths = split_str(input);

            vector<unsigned char> circular_list(256);
            generate(circular_list.begin(), circular_list.end(), [n = 0]() mutable { return static_cast<unsigned char>(n++); });

            cout << "Part 1: " << calc_checksum(process_list(lengths)) << "\n";

            vector<unsigned char> lengths2(input.begin(), input.end());
            auto new_list = process_list(append_suffix(lengths2), start_buffer(), 64); 
            cout << "Part 2: " << hexify(reduce_hash(new_list)) << "\n";
        }
    }

    return 0;
}
