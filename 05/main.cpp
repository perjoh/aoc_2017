#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

int count_jumps(vector<int>& jumps) 
{
	size_t i = 0;
	int counter = 0;

	while (0 <= i && i < jumps.size()) {
		int& step = jumps[i];
		i += step++;
		++counter;
	}

	return counter;
}

int main()
{
	vector<int> jumps;

	string input;
	while (getline(cin, input) && !input.empty()) {
		stringstream ss(input);
		istream_iterator<int> it(ss);
		jumps.push_back(*it);
	}

	const int num_jumps = count_jumps(jumps);
	cout << "Number of jumps: " << num_jumps << "\n";

	return 0;
}
