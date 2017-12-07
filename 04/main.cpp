#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <algorithm>

int main()
{
	const bool handleAnagrams = true;
	int total = 0;
	int num_invalid = 0;
	std::string input;
	while (std::getline(std::cin, input) && !input.empty()) { 
		std::set<std::string> unique_words;
		std::stringstream ss(input);
		std::string word;
		while (ss >> word) {
			if (handleAnagrams) {
				std::sort(std::begin(word), std::end(word));
			}
			if (!unique_words.insert(word).second) {
				++num_invalid;
				break;
			}
		}

		++total;
	}

	std::cout << "Valid pass phrases:" << (total - num_invalid) << "\n";
	std::cout << "Invalid pass phrases:" << num_invalid << "\n";
	std::cout << "Total pass phrases:" << total << "\n";

	return 0;
}
