#include <iostream>
#include <vector>
#include <fstream>

class Expression {
    public:
    char lhs;
    std::string rhs;
    Expression(std::string s) {
        lhs = s.at(0);
        rhs = s.substr(2, s.length() - 2);
    }
    Expression(char l, std::string r) {
        lhs = l;
        rhs = r;
    }
}
;
int main() {
    std::vector<Expression> threeac = {
        Expression('a', "9"),
        Expression('b', "c+d"),
        Expression('e', "c+d"),
        Expression('f', "b+e"),
        Expression('r', "f"),
    };

    // Dead code elimination
    std::vector<Expression> withoutDeadCode;
    for (int i = 0; i<threeac.size()-1; ++i) {
        bool used = false;
        for (int j = i+1; j<threeac.size(); ++j) {
            for (char c: threeac[j].rhs) {
                if (c == threeac[i].lhs) {
                    used = true;
                    break;
                }
                if (used) break;
            }
            if (used) break;
        }
        if (used) {
            withoutDeadCode.push_back(threeac[i]);
        } 
    }
    withoutDeadCode.push_back(threeac[threeac.size()-1]);

    std::cout << "After Dead code elimination: \n";
    for (Expression e: withoutDeadCode) {
        std::cout << e.lhs << "=" << e.rhs << "\n";
    }
    std::cout << std::endl;

    // common subexpression
    std::vector<Expression> withoutSubExp = withoutDeadCode;
    for (int i = 0; i<withoutDeadCode.size(); ++i) {
        for (int j = i+1; j<withoutDeadCode.size(); ++j) {
            if (withoutDeadCode[i].rhs == withoutDeadCode[j].rhs) {
                withoutSubExp[j].rhs = "";
                withoutSubExp[j].rhs += withoutDeadCode[i].lhs;
            }
        }
    }

    // Simplifying
    std::vector<Expression> withoutcommon = withoutSubExp;
    std::vector<int> toRemove;
    for (int i = 0; i<withoutSubExp.size()-1; ++i) {
        if (withoutSubExp[i].rhs.length() == 1) {
            for (int j = i+1; j<withoutSubExp.size(); ++j) {
                for (int k = 0; k< withoutcommon[j].rhs.length(); ++k) {
                    if (withoutcommon[j].rhs.at(k) == withoutSubExp[i].lhs) {
                        withoutcommon[j].rhs.at(k) = withoutSubExp[i].rhs.at(0);
                    }
                }
            }
            toRemove.push_back(i);
        }
    }
    std::vector<Expression> final;
    for (int i = 0; i<withoutcommon.size(); ++i) {
        bool flag = false;
        for (int j : toRemove) {
            if (i == j) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            final.push_back(withoutcommon[i]);
        }
    }

    std::cout << "Final: \n";
    for (Expression e: final) {
        std::cout << e.lhs << "=" << e.rhs << "\n";
    }
    std::cout << std::endl;

    return 0;
}