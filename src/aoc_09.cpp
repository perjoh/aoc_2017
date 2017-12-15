#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <iterator>

template <typename Char_it>
Char_it skip_garbage(Char_it char_it, int& garbage_count)
{
    assert(*char_it == '<');
    ++char_it;

	while (*char_it != '>') {
		if (*char_it == '!') {
			++char_it;
		} else {
			++garbage_count;
		}

		++char_it;
	}

	return char_it;
}

template <typename Char_it>
int count_garbage(Char_it char_it)
{
	int garbage_count = 0;
	skip_garbage(char_it, garbage_count);
	return garbage_count;
}

template <typename Char_it>
Char_it group_score_internal(	Char_it char_it, 
								int& total_score, 
								int& garbage_count,
								int depth = 1)
{
    total_score += depth;

    assert(*char_it == '{');
    ++char_it;

	while (char_it != Char_it() && *char_it != '}') {
		if (*char_it == '{') {
			char_it = group_score_internal(	char_it, 	
											total_score, 
											garbage_count, 
											depth + 1); 
		} else {
			if (*char_it == '<') {
				int garb_count = 0;
				char_it = skip_garbage(char_it, garb_count);
				garbage_count += garb_count;
			}
		}

		++char_it;
	}

	return char_it;
}

int dummy = 0;

template <typename Char_it>
int group_score(Char_it char_it, int& garbage_count = dummy)
{
    int score = 0;
	garbage_count = 0;
    group_score_internal(char_it, score, garbage_count);
    return score;
}

int main()
{
	// Part 1
	assert(group_score("{}") == 1);
	assert(group_score("{{{}}}") == 6);
	assert(group_score("{{},{}}") == 5);
	assert(group_score("{{{},{},{{}}}}") == 16);
	assert(group_score("{<a>,<a>,<a>,<a>}") == 1);
	assert(group_score("{{<ab>},{<ab>},{<ab>},{<ab>}}") == 9);
	assert(group_score("{{<!!>},{<!!>},{<!!>},{<!!>}}") == 9);
	assert(group_score("{{<a!>},{<a!>},{<a!>},{<ab>}}") == 3);

	// Part 2
	assert(count_garbage("<>") == 0);
	assert(count_garbage("<random characters>") == 17);
	assert(count_garbage("<<<<>") == 3);
	assert(count_garbage("<{!>}>") == 2);
	assert(count_garbage("<!!>") == 0);
	assert(count_garbage("<!!!>>") == 0);
	assert(count_garbage("<{o\"i!a,<{i<a>") == 10);

	std::ifstream file("input/aoc_09.txt");
	if (file) { 

        std::istream_iterator<char> characters(file);
		int garbage_count = 0;
        std::cout << "Part 1: " << group_score(characters, garbage_count) << "\n"; 
        std::cout << "Part 2: " << garbage_count << "\n"; 

	} else {
		std::cout << "No input!\n";
	}

	return 0;
}
