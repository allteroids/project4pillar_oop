#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

inline string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

inline bool containsHiraukanKapital(const string& text, const string& key) {
    return toLower(text).find(toLower(key)) != string::npos;
}

inline string formatRupiah(long amount) {
    string digits = to_string(amount < 0 ? -amount : amount);
    string result;
    int count = 0;
    for (int i = static_cast<int>(digits.size()) - 1; i >= 0; --i) {
        result.insert(result.begin(), digits[i]);
        if (++count % 3 == 0 && i > 0) result.insert(result.begin(), '.');
    }
    return string(amount < 0 ? "-Rp " : "Rp ") + result;
}

#endif
