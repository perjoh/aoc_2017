#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cassert>
#include <sstream>

using namespace std; 

struct Program 
{
	int weight{0};

	string parent;
	vector<string> children;
}; 

using Program_map = map<string, Program>;

void update_parents(Program_map& programs)
{
	for (auto& pair : programs) {
		for (auto& child : pair.second.children) {
			programs[child].parent = pair.first;
		}
	}
}

std::string find_root(const Program_map& programs)
{
	for (auto& pair : programs) {
		if (pair.second.parent.empty()) {
			return pair.first;
		}
	}

	return std::string();
}

pair<string, Program> parse_input(const string& input)
{
	vector<string> names;
	static regex regex_names("([a-z]+)");
	smatch m;
	string::const_iterator input_it = input.cbegin();
	while (regex_search(input_it, input.cend(), m, regex_names)) {
		names.push_back(m[1]);
		input_it = m[0].second;
	}

	static regex regex_weight("\\(([0-9]+)\\)");
	regex_search(input, m, regex_weight); 

	Program program;
	program.weight = stoi(m[1]);
	program.children.insert(program.children.begin(), names.cbegin() + 1, names.cend()); 

	return make_pair(names[0], program);
}

int sum_weight(const Program_map& programs, const string& prg_name)
{
	auto& program = programs.find(prg_name)->second;

	int sum = program.weight;

	for (auto& child : program.children) {
		sum += sum_weight(programs, child);
	}

	return sum;
}

bool is_balanced(const Program_map& programs, const string& prg_name)
{
	auto& program = programs.find(prg_name)->second;
	if (!program.children.empty()) {
		const int weight = sum_weight(programs, program.children[0]);
		for (auto it = program.children.cbegin() + 1; it != program.children.cend(); ++it) {
			if (weight != sum_weight(programs, *it)) {
				return false;
			}
		}
	}

	return true;
}

string find_unbalanced(const Program_map& programs, const string& root_prg) 
{
	auto& program = programs.find(root_prg)->second;

	for (auto& child_prg : program.children) {
		if (!is_balanced(programs, child_prg)) {
			return find_unbalanced(programs, child_prg);
		}
	} 

	return root_prg;
}

int get_weight(const Program_map& programs, const string& prg_name)
{
    return programs.find(prg_name)->second.weight;
}
 
int find_balance_weight(const Program_map& programs, const string& prg)
{
    auto& program = programs.find(prg)->second;

	if (!program.children.empty()) {

		vector<int> weights;
		for (auto& child_prg : program.children) {
			weights.push_back(sum_weight(programs, child_prg));
		}

		sort(weights.begin(), weights.end());
		auto it = adjacent_find(weights.cbegin(), weights.cend());
		if (it != weights.cend()) {
			int common_weight = *it;

			for (auto& child_prg : program.children) {
				const int weight = sum_weight(programs, child_prg);
				if (weight != common_weight) {
					int diff = weight - common_weight;
					return get_weight(programs, child_prg) - diff;
				}
			}
		}
	}

    return 0; 
}

void test()
{
	auto program = parse_input("fwft (72) -> ktlj, cntj, xhth");
	assert(program.first == "fwft");
	assert(program.second.weight == 72);
	assert(program.second.children[0] == "ktlj");
	assert(program.second.children[1] == "cntj");
	assert(program.second.children[2] == "xhth");
}

int main()
{
	test();

	ifstream file("input/aoc_07.txt");
	if (file) {

		Program_map programs;

		string input;
		while (getline(file, input) && !input.empty()) {
			auto program = parse_input(input);
			programs[program.first] = program.second;
		}

		update_parents(programs);

		assert(count_if(programs.cbegin(), programs.cend(), [](auto& prg){ return prg.second.parent.empty(); }) == 1);

		const string& root = find_root(programs);
		cout << "Part 1: " << root << "\n";

        cout << "Part 2: " << find_balance_weight(programs, find_unbalanced(programs, root)) << "\n";
	}

	return 0;
}
