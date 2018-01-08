#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <functional>
#include <cassert>
#include <array>

using namespace std;

bool is_char(char c)
{
    return c <= 'z' && 'a' <= c; 
}

struct Instruction 
{
	enum Opcode { set_, set_reg, sub, sub_reg, mul, mul_reg, jnz, jnz_reg };

    Instruction(const string& instr)
    {
        stringstream ss(instr);

		string op;
        ss >> op;
		string arg0;
        ss >> arg0;
		string arg1;
        ss >> arg1; 

        if (op == "jnz") {

            if (is_char(arg0[0])) {
                opcode = jnz_reg;
                reg = arg0[0] - 'a';
            }
            else {
                opcode = jnz;
                reg = stol(arg0);
            } 

            regval = stoll(arg1);
        } else {
            if (is_char(arg1[0])) {
                op += "_reg";
                regval = arg1[0] - 'a';
            } else {
                regval = stoll(arg1);
            }

            reg = arg0[0] - 'a';

            assign_op_if(op, "set", set_);
            assign_op_if(op, "set_reg", set_reg);
            assign_op_if(op, "sub", sub);
            assign_op_if(op, "sub_reg", sub_reg);
            assign_op_if(op, "mul", mul);
            assign_op_if(op, "mul_reg", mul_reg);
        } 
    } 

	void assign_op_if(const string& op_lhs, const char* op_rhs, Opcode opcode_index)
	{
		if (op_lhs == op_rhs) { 
			opcode = opcode_index;
		}
	}

	Opcode opcode{set_}; 
	int reg{0};
	long long regval{0};
};

using Registers = array<long long, 'z' - 'a' + 1>;

int run_program(const vector<Instruction>& program, Registers& registers)
{
    int mul_count = 0;
    size_t pc = 0;
    while (pc < program.size()) {
        const Instruction& i = program[pc];
        switch (i.opcode) {
            case Instruction::set_:
                registers[i.reg] = i.regval;
                break;
            case Instruction::set_reg:
                registers[i.reg] = registers[static_cast<size_t>(i.regval)];
                break;
            case Instruction::sub:
                registers[i.reg] -= i.regval;
                break;
            case Instruction::sub_reg:
                registers[i.reg] -= registers[static_cast<size_t>(i.regval)];
                break;
            case Instruction::mul:
                registers[i.reg] *= i.regval;
                ++mul_count;
                break;
            case Instruction::mul_reg:
                registers[i.reg] *= registers[static_cast<size_t>(i.regval)];
                ++mul_count;
                break;
            case Instruction::jnz:
                if (i.reg != 0) {
                    pc += static_cast<size_t>(i.regval) - 1;
                }
                break;
            case Instruction::jnz_reg: 
                if (registers[i.reg] != 0) {
                    pc += static_cast<size_t>(i.regval) - 1;
                }
                break;
        }

        ++pc;
    } 

    return mul_count;
} 

template <typename Stream>
vector<Instruction> read_program(Stream& stream)
{
    vector<Instruction> program;
    string input;
    while (getline(stream, input) && !input.empty()) {
        Instruction instruction(input);
        program.push_back(instruction);
    }
    return program;
}

constexpr int calc_h()
{
    // Hardcoded for specific input.

	int h = 0;

	for (int b = 108100; b <= 125100; b += 17) {
        for (int d = 2; d < b/2; ++d) {
            if (b%d == 0) {
                ++h;
                break;
            }
        }
	}

    return h;
}


int main()
{
    ifstream file("input/aoc_23.txt");
    if (file) { 
        auto program = read_program(file);

        Registers registers{ 0 };
        auto mul_count = run_program(program, registers); 
        cout << "Part 1: " << mul_count << "\n"; 
        cout << "Part 2: " << calc_h() << "\n";
    }

    return 0;
}
