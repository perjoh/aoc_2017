#include <iostream>
#include <iostream>
#include <fstream>
#include <map>
#include <deque>
#include <regex>
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;

struct Op
{
    char write_value{0};
    int step{0};
    char next_state{' '};
};

struct State
{
    Op op0;
    Op op1;
};

using Blueprint = map<char, State>;
using Tape = deque<char>; 
using Blueprint_result = tuple<char, int, Blueprint>;

template <typename Stream>
string get_value(Stream& stream, string pattern_str)
{
    string input;
    if (getline(stream, input)) {
        regex pattern(pattern_str);
        smatch m;
        if (regex_search(input, m, pattern)) {
            return m[1].str();
        }
    }

    assert(false);
    return string();
}

template <typename Stream>
void get_empty(Stream& stream)
{
    string dummy;
    getline(stream, dummy);
    assert(dummy.empty());
}

template <typename Stream>
void read_op(Stream& stream, Op& op)
{
    op.write_value = static_cast<char>(stoi(get_value(stream, "- Write the value ([0-9]).")));
    op.step = get_value(stream, "- Move one slot to the ([a-z]+).") == "left" ? -1 : 1;
    op.next_state = get_value(stream, "- Continue with state ([A-Z]).")[0]; 
}

template <typename Stream>
Blueprint_result read_blueprint(Stream& stream)
{
    Blueprint_result result;

    get<0>(result) = get_value(stream, "Begin in state ([A-Z]).")[0];
    get<1>(result) = stol(get_value(stream, "Perform a diagnostic checksum after ([0-9]+) steps."));

    while (stream) {
        get_empty(stream);

        if (stream) {
            const char state_name = get_value(stream, "In state ([A-Z]):")[0];

            State state;

            const char* pattern = "If the current value is ([0-9]+):";
            int value_0 = stoi(get_value(stream, pattern));
            assert(value_0 == 0);

            read_op(stream, state.op0);

            int value_1 = stoi(get_value(stream, pattern));
            assert(value_1 == 1);

            read_op(stream, state.op1);

            get<2>(result)[state_name] = state;
        }
    } 

    return result;
}

class Blueprint_runner
{
public :
    void run_blueprint(const Blueprint& blueprint, char begin_state, size_t num_iterations)
    {
        cursor_ = 0; 
        char current_state = begin_state;

        tape_.clear();
        tape_.push_back(0);

        while (num_iterations--) {
            const State& state = blueprint.find(current_state)->second;

            char value = tape_[cursor_];

            const Op& op = (value == 0) ? state.op0 : state.op1;

            tape_[cursor_] = op.write_value;

            if (op.step < 0) {
                tape_.push_front(0); // Implicit --cursor_;
            } else if (op.step > 0) {
                ++cursor_;
                tape_.push_back(0);
            }

            current_state = op.next_state;
        }
    }

    size_t diagnostic_checksum() const
    {
        return count(tape_.cbegin(), tape_.cend(), 1);
    }

private :
    int cursor_{0};
    Tape tape_;
};

void test()
{
    const char* blueprint_str = "Begin in state A.\n"
                                "Perform a diagnostic checksum after 6 steps.\n"
                                "\n"
                                "In state A:\n"
                                "  If the current value is 0:\n"
                                "    - Write the value 1.\n"
                                "    - Move one slot to the right.\n"
                                "    - Continue with state B.\n"
                                "  If the current value is 1:\n"
                                "    - Write the value 0.\n"
                                "    - Move one slot to the left.\n"
                                "    - Continue with state B.\n"
                                "\n"
                                "In state B:\n"
                                "  If the current value is 0:\n"
                                "    - Write the value 1.\n"
                                "    - Move one slot to the left.\n"
                                "    - Continue with state A.\n"
                                "  If the current value is 1:\n"
                                "    - Write the value 1.\n"
                                "    - Move one slot to the right.\n"
                                "    - Continue with state A.\n";

    stringstream ss(blueprint_str);
    auto blueprint_result = read_blueprint(ss);
    Blueprint_runner runner;
    runner.run_blueprint(get<2>(blueprint_result), get<0>(blueprint_result), get<1>(blueprint_result));
    assert(runner.diagnostic_checksum() == 3);
}

int main()
{
    test();

    ifstream file("input/aoc_25.txt");
    if (file) {
        auto blueprint_result = read_blueprint(file);

        Blueprint_runner runner;
        runner.run_blueprint(get<2>(blueprint_result), get<0>(blueprint_result), get<1>(blueprint_result));

        cout << "Part 1: " << runner.diagnostic_checksum() << "\n";
    }

    return 0;
}
