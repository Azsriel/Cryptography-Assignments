#include <iostream>
#include <vector>
#include <algorithm>

std::vector<char> NTList;
std::vector<char> TList;

enum class Action {
    Shift,
    Reduce,
    Accept,
    Empty,
};
class ActionValue {
public:
    Action a;
    int n;

    ActionValue(Action a, int n) : a{a}, n{n} {}
    ActionValue() {a = Action::Empty; n = 0;}
};


bool isNT(char c) {
    return (c >= 'A' && c<='Z');
}

void addIfNotPresent(char c) {
    if (isNT(c)) {
        if (std::count(NTList.begin(), NTList.end(), c) <= 0) {
            NTList.push_back(c);
        }   
    } else {
        if (std::count(TList.begin(), TList.end(), c) <= 0) {
            TList.push_back(c);
        }
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
};

Grammer::Grammer(std::string s) {
    lhs = s.at(0);
    addIfNotPresent(s.at(0));

    rhs = "";
    for (int i = 3; i<s.length(); ++i) {
        rhs += s.at(i);
        addIfNotPresent(s.at(i));
    }
}
Grammer::Grammer(char lhs, std::string rhs) : lhs{lhs}, rhs{rhs} {}
Grammer Grammer::generateDefaultDot() {
    std::string grhs = "." + rhs;
    return Grammer(lhs, grhs);
}

std::vector<Grammer> grammers;
std::vector<std::vector<Grammer>> closures;

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

std::vector<Grammer> generateClosure(int closureIndex, char c) {
    std::vector<Grammer> cl;
    for (Grammer g: closures[closureIndex]) {
        int index = indexOfDot(g.rhs);
        if (index + 1 < g.rhs.length() && g.rhs.at(index+1) == c) {
            std::string grhs = moveDotForward(g.rhs);
            insertIntoClosure(cl, Grammer(g.lhs, grhs));
        }
    }

    return cl;
}

std::vector<Grammer> generateFirstClosure() {
    std::vector<Grammer> c;
    insertIntoClosure(c, grammers[0].generateDefaultDot());
    return c;
}

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
    return -1;
} 
int getIndexOfNT(char t) {
    for (int i = 0; i<NTList.size(); ++i) {
        if (NTList[i] == t) {
            return i;
        }
    }
    return -1;
} 

std::vector<std::vector<ActionValue>> ActionTable;
std::vector<std::vector<int>> GotoTable;

void insertIntoTablesOld(int index, char c, int value) {
    if (isNT(c)) {
        GotoTable[index][getIndexOfNT(c)] = value;
    } else {
        ActionTable[index][getIndexOfT(c)] = ActionValue(Action::Shift, value);
    }
}

void insertIntoTablesNew(int index, char c, int value) {
    insertIntoTablesOld(index, c, value);

    std::vector<ActionValue> row;
        for (int j = 0; j < TList.size(); ++j) {
            row.push_back(ActionValue());
        }
    ActionTable.push_back(row);

    std::vector<int> row2;
        for (int j = 0; j < NTList.size(); ++j) {
            row2.push_back(0);
        }
    GotoTable.push_back(row2);
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

    // Generate Closures and fill tables
    //Generate First Closure and first rows of action/goto
    closures.push_back(generateFirstClosure());

    std::vector<ActionValue> row;
        for (int j = 0; j < TList.size(); ++j) {
            row.push_back(ActionValue());
        }
    ActionTable.push_back(row);

    std::vector<int> row2;
        for (int j = 0; j < NTList.size(); ++j) {
            row2.push_back(0);
        }
    GotoTable.push_back(row2);

    // GOTOs
    int max = 0;
    int index = 0;
    while (index <= max) {
        std::vector<Grammer> closure = closures[index];
        for (Grammer g: closure) {
            int iOfDot = indexOfDot(g.rhs);
            if (iOfDot != g.rhs.length() - 1) {
                std::vector<Grammer> newClosure = generateClosure(index, g.rhs.at(iOfDot+1));
                bool flag = false;
                int value;
                for (int i = 0; i<closures.size(); ++i) {
                    if (compareClosures(newClosure, closures[i])) {
                        flag = true;
                        insertIntoTablesOld(index, g.rhs.at(iOfDot+1), i);
                        break;
                    }
                }
                if (!flag) {
                    closures.push_back(newClosure);
                    max++;

                    insertIntoTablesNew(index, g.rhs.at(iOfDot+1), max);
                }
                
            }
        }
        index++;
    }

    //Accept 
    TList.push_back('$');
    ActionTable[1][getIndexOfT('$')].a = Action::Accept;

    // //Debug1 -- closures
    // for (int i = 0; i<closures.size(); ++i) {
    //     std::cout << "I" << i << std::endl;
    //     for (Grammer g: closures[i]) {
    //         g.print();
    //     }
    //     std::cout << std::endl;
    // }

    //Debug2 -- shift / goto
    for (int i = 0; i<ActionTable.size(); ++i) {
        for(int j = 0; j<ActionTable[i].size(); ++i) {
            if (ActionTable[i][j].a != Action::Empty) {
                std::cout << "State " << i << " Action " << TList[j] << " = ";
                if (ActionTable[i][j].a == Action::Accept) {
                    std::cout << "Accept\n";
                } else if (ActionTable[i][j].a == Action::Shift) {
                    std::cout << "S" << ActionTable[i][j].n << std::endl;
                }
            }
        }
    }

    //Fill in reduce actions

    //Parse the string



    return 0;
}