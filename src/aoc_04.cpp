#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <string>
#include <algorithm>
#include <vector>
#include <cassert>
#include <iterator>
#include <functional>

using namespace std;

auto split(const string& passphrase)
{
    stringstream ss(passphrase); 
    const istream_iterator<string> beg(ss), end;
    vector<string> result(beg, end);
    return result;
}

bool check_passphrase(const string& passphrase)
{
    auto words = split(passphrase);
    set<string> unique_words(begin(words), end(words));
    return unique_words.size() == words.size();
}

bool check_passphrase_anagram(const string& passphrase)
{
    auto words = split(passphrase);
    for_each(words.begin(), words.end(), [](string& s){ sort(s.begin(), s.end()); });
    set<string> unique_words(words.begin(), words.end());
    return unique_words.size() == words.size();
}

int count_valid_passphrases(istream& stream, function<bool(const string&)> passphrase_checker)
{
    int num_valid = 0;

    string passphrase;
    while (getline(stream, passphrase) && !passphrase.empty()) { 
        if (passphrase_checker(passphrase)) {
            ++num_valid;
        } 
    }

    return num_valid;
}

int main()
{
    assert(check_passphrase("aa bb cc dd ee"));
    assert(!check_passphrase("aa bb cc dd aa"));
    assert(check_passphrase("aa bb cc dd aaa")); 

    assert(check_passphrase_anagram("abcde fghij"));
    assert(!check_passphrase_anagram("abcde xyz ecdab"));
    assert(check_passphrase_anagram("a ab abc abd abf abj"));
    assert(check_passphrase_anagram("iiii oiii ooii oooi oooo"));
    assert(!check_passphrase_anagram("oiii ioii iioi iiio"));

    const char* filename = "input/aoc_04.txt";
    ifstream file(filename);
    if (file) { 
        cout << "Part 1: " << count_valid_passphrases(file, &check_passphrase) << "\n"; 

        file.close();
        file.open(filename); 

        cout << "Part 2: " << count_valid_passphrases(file, &check_passphrase_anagram) << "\n"; 

    } else {
        cout << "No input!\n";
    }

    return 0;
}
