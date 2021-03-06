#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

int captcha_sum(const string& numbers, size_t offset = 1)
{
    int sum = 0;

    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] == numbers[(i + offset)%numbers.size()]) {
            sum += numbers[i] - '0';
        }
    }

    return sum; 
}

void test()
{
    assert(captcha_sum("1122") == 3);
    assert(captcha_sum("1111") == 4);
    assert(captcha_sum("1234") == 0);
    assert(captcha_sum("91212129") == 9);

    assert(captcha_sum("1212", 2) == 6);
    assert(captcha_sum("1221", 2) == 0);
    assert(captcha_sum("123425", 3) == 4);
    assert(captcha_sum("123123", 3) == 12);
    assert(captcha_sum("12131415", 4) == 4); 
}

int main()
{
    test();

    ifstream file("input/aoc_01.txt");
    if (file) {
        string input;
        if (getline(file, input)) {
            cout << "Part 1: " << captcha_sum(input) << "\n";
            cout << "Part 2: " << captcha_sum(input, input.size()/2) << "\n";
        }
    }

    return 0;
}
