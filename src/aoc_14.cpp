#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <fstream>
#include <cassert> 
#include <iterator>

using namespace std;

using Byte = unsigned char;
using Bytes = vector<Byte>;

auto knot_hash(const string& input)
{
    Bytes input_buf(reinterpret_cast<const Byte*>(&input[0]), 
                    reinterpret_cast<const Byte*>(&input[0] + input.length()));

    input_buf.insert(input_buf.end(), {17, 31, 73, 47, 23});

    Bytes sparse_hash(256);
    generate(sparse_hash.begin(), sparse_hash.end(), [n = 0]() mutable { return static_cast<Byte>(n++); });

    size_t pos = 0;
    size_t skip_size = 0;

	size_t num_rounds = 64;
    while (num_rounds--) {
        for (auto& reverse_length : input_buf) {

			const size_t size = sparse_hash.size();
            Bytes work_buf(reverse_length);

            for (size_t i = 0; i < work_buf.size(); ++i) { 
                work_buf[i] = sparse_hash[(pos + i) % size];
            }

            reverse(work_buf.begin(), work_buf.end());

            for (size_t i = 0; i < work_buf.size(); ++i) {
                sparse_hash[(pos + i) % size] = work_buf[i];
            } 

            pos = (pos + reverse_length + skip_size) % size;
            ++skip_size;
        }
    }

    const size_t block_size = 16;
    Bytes dense_hash(block_size);

	generate_n(dense_hash.begin(), block_size, [it_sparse = sparse_hash.begin(), block_size]() mutable {
        auto result = accumulate(	it_sparse + 1, 
                                    it_sparse + block_size, 
                                    *it_sparse, 
                                    [](Byte a, Byte b) -> Byte { return a^b; });
        it_sparse += block_size;
        return result;
	});

    return dense_hash;
}

string hexify(const Bytes& dense_hash)
{
    string result;
    for (auto c : dense_hash) {
        const char* hex_chars = "0123456789abcdef";
        result.push_back(hex_chars[(c>>4)&0xf]);
        result.push_back(hex_chars[c&0xf]);
    }

    return result;
}

auto expand_bits(Byte byte)
{
    Bytes result;
    result.push_back((byte&128) ? '#' : '.');
    result.push_back((byte&64) ? '#' : '.');
    result.push_back((byte&32) ? '#' : '.');
    result.push_back((byte&16) ? '#' : '.');
    result.push_back((byte&8) ? '#' : '.');
    result.push_back((byte&4) ? '#' : '.');
    result.push_back((byte&2) ? '#' : '.');
    result.push_back((byte&1) ? '#' : '.');
    return result;
}

template <typename T>
vector<T>& operator+=(vector<T>& a, const vector<T>& b)
{
    a.insert(a.end(), b.begin(), b.end());
    return a; 
}

auto expand_hash(const Bytes& hash)
{ 
    Bytes result;

    for (auto& byte : hash) { 
        result += expand_bits(byte);
    }

    assert(result.size() == 128);

    return result;
}

auto make_block(const string& input)
{ 
    Bytes block;
    for (int i = 0; i < 128; ++i) {
        block += expand_hash(knot_hash(input + "-" + to_string(i)));
    }

    return block;
}

int count_used(const Bytes& input)
{
    return count_if(input.cbegin(), input.cend(), [](Byte byte) { return byte == '#'; });
}

void flood_fill_if(Bytes& block, size_t x = 0, size_t y = 0)
{
    if (0 <= y && y < 128) {
        if (0 <= x && x < 128) {
            size_t pos = x + y * 128;
            if (block[pos] == '#') {
                block[pos] = '.';
                flood_fill_if(block, x + 1, y);
                flood_fill_if(block, x - 1, y);
                flood_fill_if(block, x, y + 1);
                flood_fill_if(block, x, y - 1);
            }
        }
    }
}

int count_regions(Bytes block)
{
    assert(block.size() == 128 * 128);

    int num_regions = 0;

    for (;;) {
        auto it = find(block.cbegin(), block.cend(), '#'); 
        if (it == block.cend()) {
            break;
        }

        const size_t offset = it - block.cbegin();
        flood_fill_if(block, offset%128, offset/128);

        ++num_regions;
    }

    return num_regions;
}
 
void test()
{
    assert(hexify(knot_hash(string())) == string("a2582a3a0e66e6e86e3812dcb672a272"));
    auto block = make_block("flqrgnkx");
    assert(count_used(block) == 8108);
    assert(count_regions(block) == 1242);
}

int main()
{
    test();

    ifstream file("input/aoc_14.txt");
    if (file) {
        const string input = *istream_iterator<string>(file);
        auto block = make_block(input);

        cout << "Part 1: " << count_used(block) << "\n"; 
        cout << "Part 2: " << count_regions(block) << "\n";
    }

    return 0;
}
