#include <iostream>

struct coord {
    int i;
    int j;
};


char FILLER;
char REPLACE;
char IJEDGE;
char alphabets[26];
std::string KEY;

char matrix[25];

void fillAlphabets() {
    for (int i = 'a'; i<='z'; ++i) {
        alphabets[i - 'a'] = i;
    }
}

std::string getInput() {
    std::string s;
    std::cout << "Enter the key: ";
    std::getline(std::cin, KEY);
    std::cout << "Enter the plaintext input: ";
    std::getline(std::cin, s);
    return s;
}

std::string removeWhitespace(std::string s) {
    std::string st = "";
    for (char c: s) {
        if (c != ' ') st += c;
    }
    return st;
} 

bool contains(std::string s, char c) {
    for (char ch: s) {
        if (ch == c) return true;
    }

    return false;
} 

void fillChars(std::string input) {
    // get FILLER char and REPLACE chars
    int count = 3;

    for (char a: alphabets) {
        if (count == 0) break;
        if (!contains(input, a) && !contains(KEY, a)) {
            switch(count--) {
                case 3:
                    FILLER = a;
                case 2:
                    REPLACE = a;
                case 1:
                    IJEDGE = a;
            }
        }
    }
}

void constructMatrix() {
    int j = 0;
    for (int i = 0; i<KEY.length(); ++i) {
        if (KEY.at(i) == 'i' || KEY.at(i) == 'j') {
            matrix[j++] = '$';
        } else {
            matrix[j++] = KEY.at(i);
        } 
    }

    for (int i = 0; i<26; ++i) {
        if (!contains(KEY, alphabets[i])) {
            if (alphabets[i] == 'i' || alphabets[i] == 'j') {
                matrix[j++] = '$';
                if (alphabets[i] == 'i') i++;
            } else {
                matrix[j++] = alphabets[i];
            }
        }
    }
}

std::string convertToDiagram(std::string os) {
    if (contains(os, 'i') && contains(os, 'j')) {
        for (int i = 0; i<os.length(); ++i) {
            if (os.at(i) == 'j') {
                os.at(i) = IJEDGE;
            }
        }
    }
    std::string ns = "";
    for (int i = 0; i<os.length(); ++i) {
        if (i == os.length()-1) {
            ns += os.at(i);
            ns += FILLER;
        } else if (os.at(i) == os.at(i+1)) {
            ns += os.at(i);
            ns += FILLER;
        } else {
            ns += os.at(i);
            ns += os.at(i+1);
            i++;
        }
    }
    return ns;
}

coord getCoords(char c) {
    char cha = (c == 'i' || c == 'j') ? '$' : c;
    for (int i = 0; i<25; ++i)  {
        if (cha == matrix[i]) {
            return {(int)(i/5), i%5};
        }
    }
    return {-1, -1};
}

void printMatrix() {
    for (int i = 0; i<5; ++i) {
        for (int j = 0; j<5; ++j) {
            std::cout << matrix[i * 5 + j] << " ";
        }
        std::cout << "\n";
    }
}

std::string encrypt(std::string input) {
    std::string cypherText = "";
    for (int i = 0; i<input.length(); i += 2) {
        coord a = getCoords(input.at(i));
        coord b = getCoords(input.at(i+1));

        if (a.i == b.i) {
            cypherText += matrix[(a.i * 5) + ((a.j + 1) % 5)];
            cypherText += matrix[(a.i * 5) + ((b.j + 1) % 5)];
        } else if (a.j == b.j) {
            cypherText += matrix[(((a.i + 1) % 5) * 5) + a.j];
            cypherText += matrix[(((b.i + 1) % 5) * 5) + a.j];
        } else {
            cypherText += matrix[a.i * 5 + b.j];
            cypherText += matrix[b.i * 5 + a.j];
        }
    }

    for (int i = 0; i<cypherText.length(); ++i) {
        if (cypherText.at(i) == '$') {
            cypherText.at(i) = REPLACE;
        }
    }

    return cypherText;
}

std::string PlayFairCypherEncrypt(std::string input) {
    
    fillChars(input);
    constructMatrix();
    input = removeWhitespace(input);
    input = convertToDiagram(input);
    std::string e = encrypt(input);

    return e;
}

std::string decrypt(std::string input) {
    for (int i = 0; i<input.length(); ++i) {
        if (input.at(i) == REPLACE) {
            input.at(i) = '$';
        }
    }

    std::string plainText = "";
    for (int i = 0; i<input.length(); i += 2) {
        coord a = getCoords(input.at(i));
        coord b = getCoords(input.at(i+1));

        if (a.i == b.i) {
            plainText += matrix[(a.i * 5) + ((a.j - 1) % 5)];
            plainText += matrix[(a.i * 5) + ((b.j - 1) % 5)];
        } else if (a.j == b.j) {
            plainText += matrix[(((a.i - 1) % 5) * 5) + a.j];
            plainText += matrix[(((b.i - 1) % 5) * 5) + a.j];
        } else {
            plainText += matrix[a.i * 5 + b.j];
            plainText += matrix[b.i * 5 + a.j];
        }
    } 


}

int main() {
    fillAlphabets();

    std::string input = getInput();

    std::string encryptedText = PlayFairCypherEncrypt(input);
    
    std::cout << "Filler: " << FILLER << "\n";
    printMatrix();

    std::cout << encryptedText << "\n";




    return 0;
}