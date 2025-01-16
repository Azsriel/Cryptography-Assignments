#include <iostream>
#include <vector>
#include <iomanip>

// s-box lut
unsigned char sbox[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// rcon for key expansion
unsigned char rcon[10] = 
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

// Permutation Box for ShiftRows
size_t shrow[16] = {0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};

// Matrix for mix cols
unsigned char mixcol[16] = {0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x03, 0x01, 0x01, 0x02};

void printMatrix(const unsigned char* charArray) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(charArray[i*4+j]) << " ";
        }
        std::cout << std::endl;
    }
}

std::string matrixToString(const unsigned char charArray[16]) {
    std::string result = "";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result += charArray[i * 4 + j];
        }
    }
    return result;
}

std::string matrixToHex(unsigned char charArray[16]) {
    std::string result = "";
    for (int i = 0; i < 16; ++i) {
        result +=  (static_cast<int>(charArray[i]) < 16 ? "0" : "") + std::to_string(static_cast<int>(charArray[i]));
        result += " ";
    }

    return result;
}

void gFunction(unsigned char* g, int roundIndex) {
    // Circular Left Shift
    unsigned char temp = g[0];
    g[0] = g[1];
    g[1] = g[2];
    g[2] = g[3];
    g[3] = temp;

    // Sbox
    g[0] = sbox[g[0]];
    g[1] = sbox[g[1]];
    g[2] = sbox[g[2]];
    g[3] = sbox[g[3]];

    // rcon
    g[0] ^= rcon[roundIndex];
}

void keyGen(unsigned char allKeys[176], unsigned char masterKey[16]) {
    for (int i = 0; i < 16; ++i){
        allKeys[i] = masterKey[i];
    }

    int totalBytesDone = 16;
    int rconI = 0;
    unsigned char tmpWord[4];

    while (totalBytesDone < 176) {
        for (int i = 0; i < 4; ++i) {
            tmpWord[i] = allKeys[i + totalBytesDone - 4];
        }

        if (totalBytesDone % 16 == 0) {
            gFunction(tmpWord, rconI++);
        }

        for (int i = 0; i < 4; ++i) {
            allKeys[totalBytesDone] = allKeys[totalBytesDone - 16] ^ tmpWord[i];
            totalBytesDone++;
        }
    }
}   

void addRoundKey(unsigned char* input, unsigned char* key) {
    for (int i = 0; i < 16; ++i) {
        input[i] ^= key[i];
    }
}

void subBytes(unsigned char* input) {
    for (int i = 0; i < 16; ++i) {
        input[i] = sbox[input[i]];
    }
}

void shiftRows(unsigned char* input) {
    unsigned char temp[16];
    for (int i = 0; i < 16; ++i) {
        temp[i] = input[shrow[i]];
    }

    for (int i = 0; i < 16; ++i) {
        input[i] = temp[i];
    }
}

unsigned char multiply(unsigned char a, unsigned char b) {
    if (a == 0x01) return b;

    // for the x^8 case
    bool x8 = (b & 0x80) != 0;

    unsigned char output = b << 1;
    if (a == 0x03) {
        output ^= b;
    }
    if (x8) {
        // 0x1B is x4 + x3 + x + 1
        output ^= 0x1B;
    }
    return output;
}

void mixColumns(unsigned char* input) {
    unsigned char temp[16];

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            unsigned char val = multiply(mixcol[j * 4], input[i * 4]);
            val ^= multiply(mixcol[j * 4 + 1], input[i * 4 + 1]);
            val ^= multiply(mixcol[j * 4 + 2], input[i * 4 + 2]);
            val ^= multiply(mixcol[j * 4 + 3], input[i * 4 + 3]);

            temp[i * 4 + j] = val;
        }
    }

    for (int i = 0; i < 16; ++i) {
        input[i] = temp[i];
    }
}

void Round(unsigned char* input, unsigned char* roundKey) {
    subBytes(input);
    shiftRows(input);
    mixColumns(input);
    addRoundKey(input, roundKey);
}

std::string AESEncrypt(unsigned char input[16], unsigned char allKeys[176]) {
    // Initial Round
    addRoundKey(input, allKeys);

    // Rounds 1-9
    for (int i = 1; i <= 9; ++i) {
        Round(input, allKeys + (16 * i));
    }

    // Round 10
    subBytes(input);
    shiftRows(input);
    addRoundKey(input, allKeys + 160);

    printMatrix(input);
    return matrixToHex(input);
}

int main() {
    std::string input;
    std::cout << "Enter a 16-character string: ";
    std::getline(std::cin, input);

    if (input.length() != 16) {
        std::cout << "Not 16 length input\n";
        return 0;
    }

    unsigned char textMatrix[16];
    for (int i = 0; i < 16; ++i) {
        textMatrix[i] = input[i];
    }

    std::string masterKey;
    std::cout << "Enter a 16-character KEY: ";
    std::getline(std::cin, masterKey);

    if (masterKey.length() != 16) {
        std::cout << "Not 16 length KEY\n";
        return 0;
    }

    unsigned char keyMatrix[16];
    for (int i = 0; i < 16; ++i) {
        keyMatrix[i] = masterKey[i];
    }

    // printMatrix(textMatrix);

    // std::cout << matrixToString(textMatrix) << "\n";

    // unsigned char test[16] = {0x0F, 0x15, 0x71, 0xC9, 0x47, 0xD9, 0xE8, 0x59, 0x0C, 0xB7, 0xAD, 0xD6, 0xAF, 0x7F, 0x67, 0x98};

    unsigned char roundKeys[176];
    keyGen(roundKeys, keyMatrix);

    // for (int i = 0; i < 11; ++i) {
    //     for (int j = i * 16; j < i * 16 + 16; ++j) {
    //         std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(roundKeys[j]) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // printMatrix(roundKeys);

    // printMatrix(roundKeys + 16);

    std::string encryptedHex = AESEncrypt(textMatrix, roundKeys);

    //std::cout << "Output: " << encryptedHex << "\n";

    return 0;
}
