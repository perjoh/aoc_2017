#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cassert>

using namespace std;

using Register_map = map<string, int>;

string get_str(stringstream& ss)
{
    string result;
    ss >> result;
    return result;
}

int get_int(stringstream& ss)
{
    int value;
    ss >> value;
    return value;
}

bool test(int a, int b, const string& condition)
{
    static map<string, function<bool(int, int)>> conditions_map;

    if (conditions_map.empty()) {
        conditions_map["<"] = [](int a, int b){ return a < b; };
        conditions_map[">"] = [](int a, int b){ return a > b; };
        conditions_map["<="] = [](int a, int b){ return a <= b; };
        conditions_map[">="] = [](int a, int b){ return a >= b; };
        conditions_map["=="] = [](int a, int b){ return a == b; };
        conditions_map["!="] = [](int a, int b){ return a != b; };
    }

    return conditions_map[condition](a, b);
}

void do_op(int& lhs, const string& op, int rhs)
{
    if (op == "inc") {
        lhs += rhs;
    } else if (op == "dec") {
        lhs -= rhs;
    }
}

void process_instruction(    const string& instruction, 
                            Register_map& registers, 
                            function<void(int&, const string&, int)> op_fun = &do_op)
{
    stringstream ss(instruction);

    const string target_reg = get_str(ss); 
    const string op = get_str(ss); 
    const int value = get_int(ss);
    get_str(ss); // if 
    const string comp_reg = get_str(ss); 
    const string comp = get_str(ss); 
    const int comp_value = get_int(ss); 

    if (test(registers[comp_reg], comp_value, comp)) {
        op_fun(registers[target_reg], op, value);
    }    
}

int largest_value(const Register_map& registers)
{
    if (!registers.empty()) {
        return max_element(cbegin(registers), 
                                cend(registers), 
                                [](auto& a, auto& b){ return a.second < b.second; })->second;
    }

    return 0;
}

int main()
{


    ifstream file("input/aoc_08.txt");
    if (file) {

        int max_val = 0;
        auto do_op_max = [&max_val](int& lhs, const string& op, int rhs){
            if (max_val < lhs) {
                max_val = lhs;
            }

            do_op(lhs, op, rhs);
        };

        {
            Register_map registers;
            process_instruction("b inc 5 if a > 1", registers, do_op_max);
            process_instruction("a inc 1 if b < 5", registers, do_op_max);
            process_instruction("c dec -10 if a >= 1", registers, do_op_max);
            process_instruction("c inc -20 if c == 10", registers, do_op_max);
            assert(largest_value(registers) == 1);
            assert(max_val == 10);

            max_val = 0;
        }

        Register_map registers; 
        string instruction;
        while (getline(file, instruction) && !instruction.empty()) {

            process_instruction(instruction, 
                                registers, 
                                do_op_max);
        }

        cout << "Part 1: " << largest_value(registers) << "\n";

        cout << "Part 2: " << max_val << "\n"; 
    }

    return 0;
}
