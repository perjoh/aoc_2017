#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

class Square
{
	friend class Square_view;

public :
	Square() = default;

	Square(size_t size)
		: size_(size)
		, data_(size*size, '.')
	{ 
	}

	Square(string s)
	{
		stringstream ss(s);
		getline(ss, s);

		ss.clear();
		ss.str(s);

		string input;
		while (getline(ss, input, '/')) {
			size_ = input.length();
			data_.insert(data_.end(), input.begin(), input.end());
		}
	}

public :

	using Replacement_map = map<Square, Square>;

	Square split(const Replacement_map& lookup) const
	{ 
		const size_t new_tile_size = (tile_size() == 2) ? 3 : (tile_size() == 3 ? 4 : 0);

		if (new_tile_size != 0)
		{
			Square new_square(new_tile_size*num_tiles_side());

			for (size_t y = 0; y < num_tiles_side(); ++y) {
				for (size_t x = 0; x < num_tiles_side(); ++x) {
					auto tile_it = lookup.find(get_tile(x, y));
					assert(tile_it != lookup.end());
					new_square.set_tile(tile_it->second, x, y);
				}
			}

			return new_square;
		}

		return Square(0);
	}

	Square iterate_split(const Replacement_map& lookup, size_t num_iterations)
	{ 
		Square result(*this);
		while (num_iterations--) { 
			result = result.split(lookup);
		}

		return result;
	}

	size_t tile_size() const
	{
		if (size_ % 2 == 0) {
			return 2;
		}
		else if (size_ % 3 == 0) {
			return 3;
		}

		return 0;
	}

	size_t num_tiles_side() const
	{
		return size_ / tile_size(); 
	}

	void set_tile(const Square& other, size_t tile_x, size_t tile_y)
	{
		const size_t tile_size = other.size_;

		for (size_t y = 0; y < tile_size; ++y) {
			for (size_t x = 0; x < tile_size; ++x) {
				set_pixel(tile_x*tile_size + x, tile_y*tile_size + y, other.get_pixel(x, y));
			}
		}
	}

	void set_pixel(size_t x, size_t y, char pixel)
	{
		data_[get_pixel_offset(x, y)] = pixel;
	}

	char get_pixel(size_t x, size_t y) const
	{
		return data_[get_pixel_offset(x, y)];
	}

	bool operator<(const Square& other) const
	{
		return data_ < other.data_;
	}

	size_t count_pixels(char pixel = '#')
	{
		return count_if(data_.begin(), data_.end(), [pixel](char p) {return p == pixel; });
	}

public :
	Square rotate_cw() const
	{
		Square result(size_);

		for (size_t y = 0; y < size_; ++y) {
			for (size_t x = 0; x < size_; ++x) { 
				result.set_pixel(size_ - y - 1, x, get_pixel(x, y));
			}
		}

		return result;
	}

	Square rotate_ccw() const
	{
		Square result(*this);
		return result.rotate_cw().rotate_cw().rotate_cw();
	}

	Square flip_horizontal() const
	{
		Square result(size_);

		for (size_t y = 0; y < size_; ++y) {
			for (size_t x = 0; x < size_; ++x) { 
				result.set_pixel(size_ - x - 1, y, get_pixel(x, y));
			}
		}

		return result;
	} 

private :
	Square get_tile(size_t tile_x, size_t tile_y) const
	{
		Square result(tile_size());

		for (size_t y = 0; y < tile_size(); ++y) {
			for (size_t x = 0; x < tile_size(); ++x) {
				result.set_pixel(x, y, get_pixel(tile_size()*tile_x + x, tile_size()*tile_y + y));
			}
		}

		return result;
	}

	size_t get_pixel_offset(size_t x, size_t y) const
	{
		return x + y*size_;
	}

private :
	size_t size_{0};
	vector<char> data_;
};

template <typename Stream>
auto read_squares(Stream& stream)
{
	Square::Replacement_map lookup;

	string row;
	while (getline(stream, row)) {
		stringstream ss(row);

		string pattern_a;
		ss >> pattern_a;
		Square square_a(pattern_a);

		string pattern_b;
		ss >> pattern_b; // =>
		ss >> pattern_b;

		Square square_b(pattern_b); 

		lookup[square_a] = square_b;
		lookup[square_a.rotate_cw()] = square_b;
		lookup[square_a.rotate_ccw()] = square_b;
		lookup[square_a.rotate_cw().rotate_cw()] = square_b;
		lookup[square_a.flip_horizontal()] = square_b;
		lookup[square_a.flip_horizontal().rotate_cw()] = square_b;
		lookup[square_a.flip_horizontal().rotate_ccw()] = square_b;
		lookup[square_a.flip_horizontal().rotate_cw().rotate_cw()] = square_b; 
	}

	return lookup;
} 

void test()
{
	stringstream ss("../.# => ##./#../...\n"
					".#./..#/### => #..#/..../..../#..#\n");

	auto replacements = read_squares(ss);

	Square square(".#./..#/###");
	auto new_square = square.split(replacements);
	new_square = new_square.split(replacements);
	assert(new_square.count_pixels() == 12);
}

int main()
{
	test();

	ifstream file("input/aoc_21.txt");
	if (file) { 
		auto replacements = read_squares(file);

		Square square(".#./..#/###");

		cout << "Part 1: " << square.iterate_split(replacements, 5).count_pixels() << "\n";
		cout << "Part 2: " << square.iterate_split(replacements, 18).count_pixels() << "\n";
	}

	return 0;
}
