#include <vector>
#include <iostream>


// Table of Position of 64 bits at initial level: Initial Permutation Table
std::vector<size_t> initial_perm = {58, 50, 42, 34, 26, 18, 10, 2,
                60, 52, 44, 36, 28, 20, 12, 4,
                62, 54, 46, 38, 30, 22, 14, 6,
                64, 56, 48, 40, 32, 24, 16, 8,
                57, 49, 41, 33, 25, 17, 9, 1,
                59, 51, 43, 35, 27, 19, 11, 3,
                61, 53, 45, 37, 29, 21, 13, 5,
                63, 55, 47, 39, 31, 23, 15, 7};
 
// Expansion D-box Table
std::vector<size_t> exp_d = {32, 1, 2, 3, 4, 5, 4, 5,
         6, 7, 8, 9, 8, 9, 10, 11,
         12, 13, 12, 13, 14, 15, 16, 17,
         16, 17, 18, 19, 20, 21, 20, 21,
         22, 23, 24, 25, 24, 25, 26, 27,
         28, 29, 28, 29, 30, 31, 32, 1};
 
// Straight Permutation Table
std::vector<size_t> per = {16,  7, 20, 21,
       29, 12, 28, 17,
       1, 15, 23, 26,
       5, 18, 31, 10,
       2,  8, 24, 14,
       32, 27,  3,  9,
       19, 13, 30,  6,
       22, 11,  4, 25};
 
// S-box Table
std::vector<std::vector<std::vector<size_t>>> sbox = 
       {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
         {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
         {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
         {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
 
        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
 
        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
 
        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
         {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
         {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
         {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
 
        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
         {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
 
        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
         {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
 
        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
         {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
 
        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
         {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};
 
// Final Permutation Table
std::vector<size_t> final_perm = {40, 8, 48, 16, 56, 24, 64, 32,
              39, 7, 47, 15, 55, 23, 63, 31,
              38, 6, 46, 14, 54, 22, 62, 30,
              37, 5, 45, 13, 53, 21, 61, 29,
              36, 4, 44, 12, 52, 20, 60, 28,
              35, 3, 43, 11, 51, 19, 59, 27,
              34, 2, 42, 10, 50, 18, 58, 26,
              33, 1, 41, 9, 49, 17, 57, 25};

// --parity bit drop table
std::vector<size_t> keyp = {57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};
 
 
// Number of bit shifts
std::vector<int> shift_table = {1, 1, 2, 2,
               2, 2, 2, 2,
               1, 2, 2, 2,
               2, 2, 2, 1};
 
// Key- Compression Table : Compression of key from 56 bits to 48 bits
std::vector<size_t> key_comp = {14, 17, 11, 24, 1, 5,
            3, 28, 15, 6, 21, 10,
            23, 19, 12, 4, 26, 8,
            16, 7, 27, 20, 13, 2,
            41, 52, 31, 37, 47, 55,
            30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53,
            46, 42, 50, 36, 29, 32};


std::vector<int> hexToBitArr(std::string s) {
    std::string temp = "";
    std::vector<int> output;

    for (char c: s) {
        switch(c) {
            case '0':
                temp = "0000";
                break;
            case '1':
                temp = "0001";
                break;
            case '2':
                temp = "0010";
                break;
            case '3':
                temp = "0011";
                break;
            case '4':
                temp = "0100";
                break;
            case '5':
                temp = "0101";
                break;
            case '6':
                temp = "0110";
                break;
            case '7':
                temp = "0111";
                break;
            case '8':
                temp = "1000";
                break;
            case '9':
                temp = "1001";
                break;
            case 'A':
                temp = "1010";
                break;
            case 'B':
                temp = "1011";
                break;
            case 'C':
                temp = "1100";
                break;
            case 'D':
                temp = "1101";
                break;
            case 'E':
                temp = "1110";
                break;
            case 'F':
                temp = "1111";
                break;
            default:
                break;
        }
        for (char c: temp) {
            output.push_back((c == '0') ? 0 : 1);
        }
    }
    return output;
}

std::string bitArrToHex(std::vector<int> v) {
    std::string output = "";

    for (int i = 0; i < v.size(); i += 4) {
        int temp = 0;
        for (int j = i; j < i + 4; ++j) {
            temp *= 2;
            temp += v[j];
        }
        if (temp < 10) {
            output += (char)(temp + '0');
        } else {
            output += (char)(temp - 10 + 'A');
        }
    }
    return output;
}

std::vector<int> strToVec(std::string s) {
    std::vector<int> v;
    for (char c: s) {
        v.push_back((int)(c - '0'));
    }

    return v;
}
std::string vecToStr(std::vector<int> v) {
    std::string s = "";
    for (int i : v) {
        s += (char)(i + '0');
    }

    return s;
}

std::vector<int> splitInHalf(std::vector<int> in, std::vector<int>& secondHalf) {
    std::vector<int> firstHalf;
    secondHalf.resize(0);

    for (size_t i = 0; i < in.size() / 2; ++i) {
        firstHalf.push_back(in[i]);
    }
    for (size_t i = in.size() / 2; i < in.size(); ++i) {
        secondHalf.push_back(in[i]);
    }

    return firstHalf;
}

std::vector<int> joinHalfs(std::vector<int> first, std::vector<int> second) {
    std::vector<int> output;
    for (int i: first) output.push_back(i);
    for (int i: second) output.push_back(i);

    return output;
}

std::vector<int> XOR(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> output;
    for (int i = 0; i < a.size(); ++i) {
        output.push_back(a[i] ^ b[i]);
    }

    return output;
}

std::vector<int> shiftLeft(std::vector<int> v) {
    std::vector<int> output(v.size(), 0);
    for (size_t i = 0; i < v.size() - 1; ++i) {
        output[i] = v[i + 1];
    }
    output[output.size() - 1] = v[0];

    return output;
} 

std::vector<int> permutation(const std::vector<int>& input, const std::vector<size_t>& permutationTable, int size) {
    std::vector<int> output(size, 0);
    for (size_t i = 0; i < permutationTable.size(); ++i) {
        output[i] = input[permutationTable[i] - 1];
    }

    return output;
}

std::vector<int> sBox(const std::vector<int>& input) {
    std::vector<int> output;

    for (size_t i = 0; i < 8; ++i) {
        int row = input[i * 6] * 2 + input[i * 6 + 5];
        int col = 0;
        for (int j = 1; j < 5; ++j) {
            col *= 2;
            col += input[i * 6 + j];
        }

        int sVal = sbox[i][row][col];

        for (int j = 3; j >= 0; --j) {
            output.push_back((sVal >> j) & 1);
        }
    }

    return output;
}

std::vector<std::vector<int>> keyGen(std::vector<int> key64Bit) {

    // Parity Drop
    auto key56Bit = permutation(key64Bit, keyp, 56);

    std::vector<std::vector<int>> allKeys;

    std::vector<int> firstHalf, secondHalf;
    firstHalf = splitInHalf(key56Bit, secondHalf);

    for (size_t i = 0; i < 16; ++i) {
        for (size_t j = 0; j < shift_table[i]; ++j) {
            firstHalf = shiftLeft(firstHalf);
            secondHalf = shiftLeft(secondHalf);
        } 

        auto key48Bit = joinHalfs(firstHalf, secondHalf);
        allKeys.push_back(permutation(key48Bit, key_comp, 48));
    }

    return allKeys;
}

std::vector<int> desFunction(std::vector<int> input, std::vector<int> key) {
    auto step1 = permutation(input, exp_d, 48);
    auto step2 = XOR(step1, key);
    auto step3 = sBox(step2);
    auto step4 = permutation(step3, per, 32);
    return step4;
}

std::vector<int> desRound(std::vector<int> input, std::vector<int> key) {
    std::vector<int> L, R;
    L = splitInHalf(input, R);

    std::vector<int> newR = XOR(L, desFunction(R, key));

    std::vector<int> output = joinHalfs(R, newR);

    return output;
}

int main() {
    std::string input; 
    std::cout << "Enter the plaintext: ";
    std::getline(std::cin, input);

    if (input.length() != 16) {
        std::cout << "Not 16 length input\n";
        return 0;
    }

    auto v = hexToBitArr(input);

    auto v2 = permutation(v, initial_perm, 64);

    std::string keyInput;
    std::cout << "Enter the key: ";
    std::getline(std::cin, keyInput);

    if (keyInput.length() != 16) {
        std::cout << "Not 16 length key\n";
        return 0;
    }

    auto keys = keyGen(hexToBitArr(keyInput));

    std::vector<std::vector<int>> roundOp;
    roundOp.push_back(v2);
    
    for (int i = 0; i < 16; ++i) {
        roundOp.push_back(desRound(roundOp[i], keys[i]));
        std::cout << "Round " << i + 1 << " result: " << bitArrToHex(roundOp[roundOp.size() - 1]) << "\n";
    }

    std::vector<int> R, L;
    L = splitInHalf(roundOp[16], R);
    auto f1 = joinHalfs(R, L);
    auto cyperArr = permutation(f1, final_perm, 64);

    std::cout << "Final Ciphertext: " << bitArrToHex(cyperArr) << "\n";
    return 0;
}