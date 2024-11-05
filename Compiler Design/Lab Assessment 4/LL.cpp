#include <iostream>
#include <vector>
#include <algorithm>

enum class SymbolType {
    NT,
    T,
    EPSILON,
    Error,
};

SymbolType getType(char c) {
    if (c >= 'A' && c <= 'Z') {
        return SymbolType::NT;
    } else {
        if (c == 'e') {
            return SymbolType::EPSILON;
        } else {
            return SymbolType::T;
        }
    }
    std::cout << "Error";
    return SymbolType::Error;
}

class Symbol {
public:
    char data;
    SymbolType type;
    Symbol(char c, SymbolType t): data{c}, type{t} {}
    Symbol(){}

    bool operator==(const Symbol& s) {
        return data == s.data;
    }
};

std::vector<Symbol> NTList;
std::vector<Symbol> TList;

int getIndex(std::vector<Symbol> v, Symbol s) {
    for (int i = 0; i< v.size(); ++i) {
        if (v[i] == s) {
            return i;
        }
    }
    return -1;
}

void addIfNotPresent(std::vector<Symbol>& v, Symbol c) {
    if (std::count(v.begin(), v.end(), c) <= 0) {
        v.push_back(c);
    }
}

class Grammer {
    public:
    Symbol lhs;
    std::vector<Symbol> rhs;

    Grammer(std::string s) {
        lhs = Symbol(s.at(0), SymbolType::NT);

        std::string rhs_string = s.substr(3, s.length() - 3);

        addIfNotPresent(NTList, lhs);

        for (char c : rhs_string) {
            Symbol sy = Symbol(c, getType(c));
            rhs.push_back(sy);
            if (sy.type == SymbolType::NT) addIfNotPresent(NTList, sy);
            else if (sy.type == SymbolType::T) addIfNotPresent(TList, sy);
        }
    } 

    Grammer() {
        lhs = Symbol('?', SymbolType::Error);
    }

    void print() {
        if (lhs.type == SymbolType::Error) {
            std::cout << "\t\t\n";
            return;
        }
        std::cout << lhs.data << " -> ";
        for (Symbol s: rhs) {
            std::cout << s.data << " ";
        }
        std::cout << std::endl;
    }
};

std::vector<Grammer> grammers;
std::vector<std::vector<Symbol>> AllFirst;
std::vector<std::vector<Symbol>> AllFollow;

std::vector<Symbol> computeFirst(Symbol s) {
    std::vector<Symbol> v;
    for (Grammer g: grammers) {
        if (g.lhs == s) {
            if (g.rhs[0].type != SymbolType::NT) {
                addIfNotPresent(v, g.rhs[0]);
                //std::cout << "Added into " << s.data << ": " << g.rhs[0].data << " From Grammer "; g.print(); 
            } else {
                std::vector<Symbol> v2 = computeFirst(g.rhs[0]);
                for (Symbol d: v2) {
                    addIfNotPresent(v, d);
                //std::cout << "Added into "<< s.data << ": " << d.data << " From Grammer "; g.print();
                } 
            }
        }
    }
    return v;
}

std::vector<Symbol> computeFollow(Symbol s) {
    std::vector<Symbol> v;  
    if (s.data == 'E') {
        addIfNotPresent(v, Symbol('$', SymbolType::T));
    }

    for (Grammer g: grammers) {
        for (int i = 0; i<g.rhs.size(); ++i) {
            if (g.rhs[i] == s) {
                bool eFlag = false;
                if (i + 1 < g.rhs.size()) {
                    Symbol beta = g.rhs[i+1];
                    if (beta.type == SymbolType::T) {
                        addIfNotPresent(v, beta);
                    } else {
                        int index = getIndex(NTList, beta);
                        for (Symbol s: AllFirst[index]) {
                            if (s.type == SymbolType::EPSILON) {
                                eFlag = true;
                            } else{
                                addIfNotPresent(v, s);
                            }
                        }
                    }
                } else {
                    eFlag = true;
                }
                if (eFlag) {
                    if (!(g.lhs == s)) {
                        std::vector<Symbol> v2 = computeFollow(g.lhs);
                        for (Symbol d: v2) {
                            addIfNotPresent(v, d);
                        }
                    }
                }
            }
        }
    }
    return v;
}

int main() {
    std::vector<std::string> inputs = {
        "E->TA",
        "A->+TA",
        "A->e",
        "T->FB",
        "B->*FB",
        "B->e",
        "F->(E)",
        "F->d"
    };

    // Getting Grammers
    for (std::string s: inputs) {
        grammers.push_back(Grammer(s));
    }

    // Debug 1 ------------------------------
    std::cout << "Grammers: \n";
    for (Grammer g: grammers) {
        std::cout << g.lhs.data << " -> ";
        for (Symbol s: g.rhs) {
            std::cout << s.data << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "NTLIST: \n";
    for (Symbol s: NTList) {
        std::cout << s.data << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "TLIST: \n";
    for (Symbol s: TList) {
        std::cout << s.data << " ";
    }
    std::cout << std::endl;
    //END Debug 1 ----------------------------

    //Computing Firsts
    for (Symbol s: NTList) {
        AllFirst.push_back(computeFirst(s));
    }

    // Debug 2 ----------------------------------------------
    std::cout << std::endl;
    std::cout << "FIRSTS : \n";
    for (int i = 0; i<AllFirst.size(); ++i) {
        std::cout << "First(" << NTList[i].data << ") : ";
        for (Symbol s: AllFirst[i]) {
            std::cout << s.data << " ";
        } 
        std::cout << std::endl;
    }
    std::cout << std::endl;
    // END Debug 2 -------------------------------------------

    TList.push_back(Symbol('$', SymbolType::T));

    //Computing Follow
    for (Symbol s: NTList) {
        AllFollow.push_back(computeFollow(s));
    }

    // Debug 2 ----------------------------------------------
    std::cout << std::endl;
    std::cout << "FOLLOWS : \n";
    for (int i = 0; i<AllFollow.size(); ++i) {
        std::cout << "Follow(" << NTList[i].data << ") : ";
        for (Symbol s: AllFollow[i]) {
            std::cout << s.data << " ";
        } 
        std::cout << std::endl;
    }
    std::cout << std::endl;
    // END Debug 2 -------------------------------------------

    // Filling in Table
    std::vector<std::vector<Grammer>> Table(NTList.size(), std::vector<Grammer>(TList.size()));

    for (Grammer g: grammers) {
        Symbol first = g.rhs[0];
        int i = getIndex(NTList, g.lhs);
        switch (first.type) {
            case SymbolType::T :
            {    
                int j = getIndex(TList, first);
                Table[i][j] = g;
                std::cout << "Added to " << NTList[i].data << " " << TList[j].data << " Grammer "; g.print();
            }
                break;
            case SymbolType::EPSILON :
            {
                for (Symbol s: AllFollow[i]) {
                    int j = getIndex(TList, s);
                    Table[i][j] = g;
                    std::cout << "Added to " << NTList[i].data << " " << TList[j].data << " Grammer "; g.print();

                }
            }
                break;
            case SymbolType::NT :
            {
                for (Symbol s: AllFirst[getIndex(NTList, first)]) {
                    int j = getIndex(TList, s);
                    Table[i][j] = g;
                    std::cout << "Added to " << NTList[i].data << " " << TList[j].data << " Grammer "; g.print();
                }
            }
                break;
        }
    }
    std::cout << std::endl;


    //Parse the string
    std::string ip = "d+d*d";

    //Init
    std::vector<Symbol> stack;
    std::vector<Symbol> ipBuffer;

    stack.push_back(Symbol('$', SymbolType::T));
    stack.push_back(Symbol('E', SymbolType::NT));

    ipBuffer.push_back(Symbol('$', SymbolType::T));
    for(int i = ip.length()-1; i >= 0; --i) {
        ipBuffer.push_back(Symbol(ip.at(i), SymbolType::T));
    }

    std::cout << "STACK\t\tI/P BUFFER\t\tACTION\n";

    while (true) {
        //Print stack, ipBuffer
        for (int i = stack.size()-1; i>=0; --i) {
            std::cout << stack[i].data << " ";
        } std::cout << "\t\t";
        for (int i = ipBuffer.size()-1; i>=0; --i) {
            std::cout << ipBuffer[i].data << " ";
        } std::cout << "\t\t";

        Symbol s1 = stack[stack.size()-1];
        Symbol i1 = ipBuffer[ipBuffer.size()-1];

        if (s1.data == '$' && i1.data == '$') {
            std::cout << "Matched\n";
            break;
        } else 
        if (s1.type == SymbolType::T && s1 == i1) {
            std::cout << "Pop " << s1.data << "\n";
            stack.pop_back();
            ipBuffer.pop_back();
        } else 
        if (s1.type == SymbolType::NT) {
            int i = getIndex(NTList, s1);
            int j = getIndex(TList, i1);

            Grammer g = Table[i][j];
            g.print();
            stack.pop_back();
            for (int x = g.rhs.size() - 1; x >= 0; --x) {
                if (!(g.rhs[x].type == SymbolType::EPSILON))
                    stack.push_back(g.rhs[x]);
            }
        } else {
            std::cout << "Error Error\n";
            break;
        }
    }

    return 0;
}

