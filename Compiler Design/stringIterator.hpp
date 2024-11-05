#pragma once
#include <string>

class StringIterator {
    int count;
    int end;
    std::string s;
public:
    StringIterator(std::string s, int begin = 0) : s{s}, count{begin}, end{s.length()} {}

    bool advance() {
        count++;
        return count != end;
    }

    char peek() {
        return s.at(count);
    }
};