#include <iostream>
#include <vector>

std::vector<char> stack = {'~'};
int main() {
    int state = 0;
    char c;
    char action;
    while (std::cin >> c) {
        if (c == '/') break;
        if (c == ',') {
            state = 0;
            continue;
        }
        if (c == ' ') continue;
        switch(state) {
            case 0:
                action = c;
                state = 1;
                break;
            case 1:
                switch(action) {
                    case '1':
                        stack.push_back(c);
                        break;
                    case '2':
                        stack.erase(stack.end() - (c - '0'));
                        break;
                    case '3':
                        std::cout << stack[(c-'0')] << std::endl;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    return 0;
}
                                                                                                                            