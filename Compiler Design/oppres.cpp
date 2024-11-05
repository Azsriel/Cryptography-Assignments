#include <iostream>
#include "stringIterator.hpp"
#include <vector>
#include <algorithm>

std::vector<char> NTList;
std::vector<char> TList;

void addIfNotPresent(std::vector<char>& v, char c) {
    if (std::count(v.begin(), v.end(), c) <= 0) {
        v.push_back(c);
    }
}

bool isNT(char c) {
    return (c >= 'A' && c <= 'Z');
}

class Grammer {
    public:
    char lhs;
    std::string rhs;

    Grammer(std::string s) {
        lhs = s.at(0);

        rhs = s.substr(3, s.length() - 3);

        addIfNotPresent(NTList, lhs);

        for (char c : rhs) {
            if (isNT(c)) addIfNotPresent(NTList, c);
            else addIfNotPresent(TList, c);
        }

    } 
};

std::vector<Grammer> grammers;

bool checkValid() {
    for (Grammer g: grammers) {
        StringIterator iter = StringIterator(g.rhs);
        while(true) {
            if(isNT(iter.peek())) {
                if (iter.advance()) {
                    if (isNT(iter.peek())) {
                        return false;
                    }
                } else {
                    break;
                }
            } else {
                if (!iter.advance()) {
                    break;
                }
            }
            
        }
    }
}

int main() {
    std::vector<std::string> inputs = {
        "S->E",
        "E->E+T",
        "E->T",
        "T->T*F",
        "T->F",
        "F->(E)",
        "F->d",
    };

    for (std::string s: inputs) {
        grammers.push_back(Grammer(s));
    }

    if (!checkValid()) {
        std::cout << "The operator grammer is not valid\n";
        exit(1);
    }

    std::cout << "Verified\n";

    // computeLeading();
    // computeTrailing();

    // initTable();
    // fillTable();



    return 0;
}