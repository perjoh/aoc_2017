#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <functional>
#include <cassert>

using namespace std;

using Registers = map<string, long long>;

struct Instruction 
{
    Instruction(const string& instr)
    {
        stringstream ss(instr);

        ss >> op;
        ss >> arg0;
        ss >> arg1;
    }

    string op;
    string arg0, arg1;
};

class Cpu
{
public :
    Cpu()
    {
        using placeholders::_1;
        using placeholders::_2; 

        instruction_set["snd"] = bind(&Cpu::snd, this, _1, _2);
        instruction_set["set"] = bind(&Cpu::set, this, _1, _2);
        instruction_set["add"] = bind(&Cpu::add, this, _1, _2);
        instruction_set["mul"] = bind(&Cpu::mul, this, _1, _2);
        instruction_set["mod"] = bind(&Cpu::mod, this, _1, _2);
        instruction_set["rcv"] = bind(&Cpu::rcv, this, _1, _2);
        instruction_set["jgz"] = bind(&Cpu::jgz, this, _1, _2); 
    }

    void connect(Cpu* other_, long long id)
    {
        other = other_;
        registers["p"] = id;
    }

    int run_program(const vector<Instruction>& program)
    {
        auto check_pc = [&program](auto pc) -> bool { return 0 <= pc && static_cast<size_t>(pc) < program.size();  };
        jmp = 1;
        int num_exec = 0;
        while (check_pc(pc) && !waiting()) {
            const auto& p = program[pc];
            jmp = 1;
            instruction_set[p.op](p.arg0, p.arg1);
            if (jmp != 0) {
                pc += jmp;
                ++num_exec;
            }
        }

        if (!check_pc(pc)) {
            terminate();
        }

        return num_exec;
    }

    long long freq_recovered() const 
    {
        return freq_playing;
    }

    bool waiting() const 
    {
        return jmp == 0;
    }

    bool terminated() const
    {
        return pc < 0;
    }

    int num_sends() const
    {
        return num_sends_;
    }

private :
    using Instruction_set = map<string, function<void(const string&, const string&)>>;
    Instruction_set instruction_set;

    int pc{0};
    int jmp{1};
    Registers registers;

    long long freq_playing{0};
    int num_sends_{0};

    Cpu* other{nullptr};
    deque<long long> queue;

private :
    void snd(const string& arg0, const string&)
    {
        if (other) { 
            other->queue.push_back(value(arg0));
            ++num_sends_;
        } else {
            freq_playing = value(arg0);
        } 
    }

    void set(const string& arg0, const string& arg1)
    {
        registers[arg0] = value(arg1);
    }

    void add(const string& arg0, const string& arg1)
    {
        registers[arg0] += value(arg1);
    }

    void mul(const string& arg0, const string& arg1)
    {
        registers[arg0] *= value(arg1);
    }

    void mod(const string& arg0, const string& arg1)
    {
        registers[arg0] %= value(arg1);
    }

    void rcv(const string& arg0, const string&)
    {
        if (other) {
            if (!queue.empty()) {
                registers[arg0] = queue.front();
                queue.pop_front();
            } else {
                jmp = 0; // waiting
            }
        } else {

            if (value(arg0) != 0) {
                if (freq_playing != 0) {
                    terminate();
                }
            }
        }
    }

    void jgz(const string& arg0, const string& arg1)
    { 
        if (value(arg0) > 0) {
            jmp = static_cast<int>(value(arg1));
        }
    }

    void terminate()
    {
        pc = -2;
    }

private :
    long long value(const string& arg) {
        if ('a' <= arg[0] && arg[0] <= 'z') {
            return registers[arg];
        } 

        return stoi(arg);
    } 
}; 

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

void test()
{
    const char* source =     "set a 1\n"
                            "add a 2\n"
                            "mul a a\n"
                            "mod a 5\n"
                            "snd a\n"
                            "set a 0\n"
                            "rcv a\n"
                            "jgz a -1\n"
                            "set a 1\n"
                            "jgz a -2\n";
    stringstream ss(source); 
    auto program = read_program(ss);
    
    Cpu cpu;
    cpu.run_program(program);
    assert(cpu.freq_recovered() == 4);
}

int main()
{
    test();

    ifstream file("input/aoc_18.txt");
    if (file) { 
        auto program = read_program(file);

        Cpu cpu;
        cpu.run_program(program);

        cout << "Part 1: " << cpu.freq_recovered() << "\n";

        Cpu cpu0; 
        Cpu cpu1; 

        cpu0.connect(&cpu1, 0);
        cpu1.connect(&cpu0, 1);

        bool is_deadlock = false;
        while (!is_deadlock) {
            cpu0.run_program(program);
            cpu1.run_program(program);

            if (cpu0.waiting() && cpu1.waiting()) {
                const int num_inst0 = cpu0.run_program(program);
                const int num_inst1 = cpu1.run_program(program); 

                if (num_inst0 == 0 && num_inst1 == 0) {
                    is_deadlock = true;
                }
            }

            if ((cpu0.waiting() && cpu1.terminated()) || (cpu1.waiting() && cpu0.terminated())) {
                is_deadlock = true;
            }
        }

        cout << "Part 2: " << cpu1.num_sends() << "\n";
    }

    return 0;
}
