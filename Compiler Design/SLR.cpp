#include <iostream>
#include <vector>
#include <algorithm>

enum class ActionVal {
    SHIFT,
    REDUCE,
    ACCEPT,
    GOTO,
    Error,
};

class Action {
public:
    int value;
    ActionVal type;

    Action(int a, ActionVal t) : value{a}, type{t} {}
    Action() : value{0}, type{ActionVal::Error} {}

    void print() {
        if (type == ActionVal::ACCEPT) {
            std::cout << "ACCEPT\n";
            return;
        } else if (type == ActionVal::SHIFT) {
            std::cout << "SHIFT " << value << "\n";
            return;
        } else if (type == ActionVal::REDUCE) {
            std::cout << "REDUCE " << value << "\n";
            return;
        } else {
            std::cout << "error error \n";
        }
    }
};

std::vector<char> NTList;
std::vector<char> TList;

bool isNT(char c) {
    return (c >= 'A' && c<='Z');
}

void addIfNotPresent(std::vector<char>& v, char c) {
    if (std::count(v.begin(), v.end(), c) <= 0) {
        v.push_back(c);
    }
}

int indexOfDot(std::string s) {
    for (int i =0; i<s.length(); ++i) {
        if (s.at(i) == '.') return i;
    }
    return -1;
}

std::string moveDotForward(std::string s) {
    int i = indexOfDot(s);
    s.at(i) = s.at(i+1);
    s.at(i+1) = '.';

    return s;
}

class Grammer {
public:
    char lhs;
    std::string rhs;

    Grammer(std::string s);
    Grammer(char lhs, std::string rhs);
    Grammer generateDefaultDot();

    bool operator==(Grammer& g2) {
        return (lhs == g2.lhs && rhs.compare(g2.rhs) == 0);
    }

    void print() {
        std::cout << lhs << "->" << rhs << std::endl;
    }

    Grammer removeDot() {
        std::string newRhs = "";
        for (char c: rhs) {
            if (c != '.') {
                newRhs += c;
            }
        }
        return Grammer(lhs, newRhs);
    }
};

Grammer::Grammer(std::string s) {
    lhs = s.at(0);
    addIfNotPresent(NTList, s.at(0));

    rhs = "";
    for (int i = 3; i<s.length(); ++i) {
        rhs += s.at(i);
        if (isNT(s.at(i)))
            addIfNotPresent(NTList, s.at(i));
        else 
            addIfNotPresent(TList, s.at(i));
    }
}
Grammer::Grammer(char lhs, std::string rhs) : lhs{lhs}, rhs{rhs} {}
Grammer Grammer::generateDefaultDot() {
    std::string grhs = "." + rhs;
    return Grammer(lhs, grhs);
}

std::vector<Grammer> grammers;
std::vector<std::vector<Grammer>> closures;

bool compareClosures(std::vector<Grammer> g1, std::vector<Grammer> g2) {
    if (g1.size() != g2.size()) return false;

    for (int i = 0; i<g1.size(); ++i) {
        if (!(g1[i] == g2[i])) {
            return false;
        }
    }
    return true;
}

int getIndexOfT(char t) {
    for (int i = 0; i<TList.size(); ++i) {
        if (TList[i] == t) {
            return i;
        }
    }
    std::cout << "Error\n";
    return -1;
} 
int getIndexOfNT(char t) {
    for (int i = 0; i<NTList.size(); ++i) {
        if (NTList[i] == t) {
            return i;
        }
    }
    std::cout << "Error\n";
    return -1;
} 

int getIndexOfGrammer(Grammer g) {
    Grammer gs = g.removeDot();

    for (int i = 0; i<grammers.size(); ++i) {
        if (gs == grammers[i]) return i;
    }

    std::cout << "Error grammer compare\n";
    return -1;
}

void insertIntoClosure(std::vector<Grammer>& v, Grammer g) {
    v.push_back(g);
    int i = indexOfDot(g.rhs);
    if (i != g.rhs.length() - 1 && isNT(g.rhs.at(i+1)) && g.rhs.at(i+1) != g.lhs) {
        for (Grammer gr : grammers) {
            if (gr.lhs == g.rhs.at(i+1)) {
                insertIntoClosure(v, gr.generateDefaultDot());
            }
        }
    }
}

std::vector<Grammer> generateFirstClosure() {
    std::vector<Grammer> c;
    insertIntoClosure(c, grammers[0].generateDefaultDot());
    return c;
}

int Goto(int closureIndex, char c, int& max) {
    std::vector<Grammer> cl;
    for (Grammer g: closures[closureIndex]) {
        int index = indexOfDot(g.rhs);
        if (index + 1 < g.rhs.length() && g.rhs.at(index+1) == c) {
            std::string grhs = moveDotForward(g.rhs);
            insertIntoClosure(cl, Grammer(g.lhs, grhs));
        }
    }

    for (int i = 0; i <= max; ++i) {
        if (compareClosures(cl, closures[i])) {
            return i;
        }
    }

    max++;
    closures.push_back(cl);
    return max;
}

std::vector<std::vector<char>> AllFirst;
std::vector<std::vector<char>> AllFollow;

std::vector<char> computeFirst(char s) {
    std::vector<char> v;
    for (Grammer g: grammers) {
        if (g.lhs == s) {
            if (isNT(g.rhs[0])) {
                addIfNotPresent(NTList, g.rhs[0]);
                //std::cout << "Added into " << s.data << ": " << g.rhs[0].data << " From Grammer "; g.print(); 
            } else {
                std::vector<char> v2 = computeFirst(g.rhs[0]);
                for (char d: v2) {
                    addIfNotPresent(TList, d);
                //std::cout << "Added into "<< s.data << ": " << d.data << " From Grammer "; g.print();
                } 
            }
        }
    }
    return v;
}

std::vector<char> computeFollow(char s) {
    std::vector<char> v;  
    if (s == 'E') {
        addIfNotPresent(v, '$');
    }

    for (Grammer g: grammers) {
        for (int i = 0; i<g.rhs.size(); ++i) {
            if (g.rhs[i] == s) {
                bool eFlag = false;
                if (i + 1 < g.rhs.size()) {
                    char beta = g.rhs[i+1];
                    if (!isNT(beta)) {
                        addIfNotPresent(v, beta);
                    } else {
                        int index = getIndexOfNT(beta);
                        for (char s: AllFirst[index]) {
                            addIfNotPresent(v, s);
                        }
                    }
                } else {
                    eFlag = true;
                }
                if (eFlag) {
                    if (!(g.lhs == s)) {
                        std::vector<char> v2 = computeFollow(g.lhs);
                        for (char d: v2) {
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
        "S->E",
        "E->E+T",
        "E->T",
        "T->T*F",
        "T->F",
        "F->(E)",
        "F->d",
    };

    // Generate Grammer vector + NTList + TList
    for (int i = 0; i<inputs.size(); ++i) {
        grammers.push_back(Grammer(inputs[i]));
    }

    //Computing Firsts
    for (char s: NTList) {
        AllFirst.push_back(computeFirst(s));
    }
    //Computing Follow
    for (char s: NTList) {
        AllFollow.push_back(computeFollow(s));
    }

    std::cout << std::endl;
    std::cout << "FOLLOWS : \n";
    for (int i = 0; i<AllFollow.size(); ++i) {
        std::cout << "Follow(" << NTList[i] << ") : ";
        for (char s: AllFollow[i]) {
            std::cout << s << " ";
        } 
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Table
    std::vector<std::vector<Action>> Table(15, std::vector<Action>(10));

    //Accept
    TList.push_back('$');
    Table[1][getIndexOfT('$')] = Action(0, ActionVal::ACCEPT);

    closures.push_back(generateFirstClosure());
    int max = 0;
    int current = 0;
    // Gotos + Shift
    while (current <= max) {
        std::vector<char> sym;
        for (Grammer g: closures[current]) {
            int index = indexOfDot(g.rhs);
            if (index != g.rhs.length() - 1) {
                addIfNotPresent(sym, g.rhs.at(index + 1));
            }
        }

        for (char c : sym) {
            int x = Goto(current, c, max);

            int i = current;
            int j;
            if (isNT(c)) {
                j = getIndexOfNT(c) + TList.size();
                Table[i][j] = Action(x, ActionVal::GOTO);
                std::cout << "Inserted " << x << " at " << current << " " << c << std::endl;
            } else {
                j = getIndexOfT(c);
                Table[i][j] = Action(x, ActionVal::SHIFT);
                //std::cout << "Inserted " << x << " at " << current << " " << c << std::endl;
            }
        }
        current++;
    }

    

    //Reduce
    for (int i = 2; i<=max; ++i) {
        for (Grammer g: closures[i]) {
            if (g.rhs.at(g.rhs.length()-1) == '.') {
                int x = getIndexOfGrammer(g);
                for (char c: AllFollow[getIndexOfNT(g.lhs)]) {
                    int j = getIndexOfT(c);
                    Table[i][j] = Action(x, ActionVal::REDUCE);
                    //std::cout << "Inserted " << x << " at " << i << " " << c << std::endl;
                }
            }
        }
    }

    std::string ip = "d+d*d";

    std::vector<int> stack;
    std::vector<char> ipBuffer;

    stack.push_back(0);

    ipBuffer.push_back('$');
    for(int i = ip.length()-1; i >= 0; --i) {
        ipBuffer.push_back(ip.at(i));
    }

    std::cout << "STACK\t\tI/P BUFFER\t\tACTION\n";

    while(true) {
        for (int i = 0; i<stack.size(); ++i) {
            if (stack[i] < 20)
                std::cout << stack[i] << " ";
            else std::cout << (char)stack[i] << " ";
        } std::cout << "\t\t";
        for (int i = ipBuffer.size()-1; i>=0; --i) {
            std::cout << ipBuffer[i] << " ";
        } std::cout << "\t\t";

        int s1 = stack[stack.size()-1];
        char i1 = ipBuffer[ipBuffer.size()-1];

        int i = s1;
        int j = getIndexOfT(i1);

        Action action = Table[i][j];
        action.print();
        if (action.type == ActionVal::ACCEPT) {
            break;
        }
        if (action.type == ActionVal::SHIFT) {
            stack.push_back(int(i1));
            ipBuffer.pop_back();
            stack.push_back(action.value);
        } else if (action.type == ActionVal::REDUCE) {
            char lhs = grammers[action.value].lhs;
            int len = grammers[action.value].rhs.length();
            while (len != 0) {
                if (stack[stack.size() - 1] < 20) {
                    stack.pop_back();
                } else {
                    len--;
                    stack.pop_back();
                }
            }

            int i2 = stack[stack.size() - 1];
            stack.push_back((int)lhs);
            int j2 = getIndexOfNT(lhs) + TList.size();
            int next = Table[i2][j2].value;
            stack.push_back(next);

        } else {
            std::cout << "error error\n";
            break;
        }

    }

    return 0;
}