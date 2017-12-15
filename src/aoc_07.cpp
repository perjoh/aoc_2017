#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Program 
{
	int weight{0};

	std::string parent;
	std::vector<std::string> children;
}; 

void update_parents(std::map<std::string, Program>& programs)
{
	for (auto& pair : programs) {
		for (auto& child : pair.second.children) {
			programs[child].parent = pair.first;
		}
	}
}

std::string find_root(const std::map<std::string, Program>& programs)
{
	for (auto& pair : programs) {
		if (pair->second.parent.empty()) {
			return pair->first;
		}
	}

	return std::string();
}

int main()
{
	std::map<std::string, Program> programs;


	return 0;
}
