#include <vector>
#include <iostream>
#include <stdexcept>

int multiplicativeInverse(int n, int base) {
    for (int i = 1; i < n; ++i) {
        if (((n * i) % base) == 1) return i;
    }
    std::cout << "N: " << n << " Base: " << base << "\n";
    throw std::runtime_error("Wrong number to inverse");
}

class Matrix {
public:
    std::vector<std::vector<double>> data;
    size_t rows, cols;

    Matrix(size_t rows, size_t cols) : rows{rows}, cols{cols} {
        data.resize(rows, std::vector<double>(cols, 0));
    }

    std::vector<double>& operator[](int i) {
        if (i < 0 || i >= data.size()) {
            throw std::out_of_range("Row index out of range");
        }

        return data[i];
    }

    const std::vector<double>& operator[](int i) const {
        if (i < 0 || i >= data.size()) {
            throw std::out_of_range("Row index out of range");
        }

        return data[i];
    }

    Matrix operator*(const Matrix& other) {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix sizes do not match for multiplication");
        }

        Matrix result(rows, other.cols); 
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result[i][j] += data[i][k] * other[k][j];
                }
            }
        }

        return result;
    }

    Matrix operator*(int n) {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] * n;
            }
        }

        return result;
    }

    double determinant() {
        if (rows != cols) {
            throw std::invalid_argument("Wrong Matrix");
        }

        if (rows == 1) {
            return data[0][0];
        }

        if (rows == 2) {
            return data[0][0] * data[1][1] - data[0][1] * data[1][0];
        }

        double d = 0;
        for (size_t i = 0; i < cols; ++i) {
            Matrix subMatrix(rows-1, cols-1);
            for (size_t j = 1; j < rows; ++j) {
                size_t coli = 0;
                for (size_t k = 0; k < cols; ++k) {
                    if (k == i) continue;
                    subMatrix[j-1, coli++, data[j][k]];
                }
            }
            d += (i % 2 == 0 ? 1 : -1) * data[0][i] * subMatrix.determinant();
        }

        return d;
    }

    Matrix adjoint() {
        if (rows != cols) {
            throw std::invalid_argument("Wrong Matrix");
        }

        Matrix result(rows, cols);
        if (rows == 1) {
            result[0][0] = 1;
            return result;
        }

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                Matrix subMatrix(rows - 1, cols - 1);
                size_t rowIndex = 0;
                for (size_t k = 0; k < rows; ++k) {
                    if (k == i) continue;
                    size_t colIndex = 0;
                    for (size_t l = 0; l < cols; ++l) {
                        if (l == j) continue;
                        subMatrix[rowIndex][colIndex] =  data[k][l];
                        colIndex++;
                    }
                    rowIndex++;
                }
                result[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * subMatrix.determinant();
            }
        }

        return result;
    }

    Matrix inverse() {
        if (rows != cols) {
            throw std::invalid_argument("Finding Inverse of wrong matrix");
        }

        double detK = determinant();
        int mi = multiplicativeInverse((int)detK, 26);

        Matrix result = (adjoint() * mi);
        result = result % 26;

        return result;
    }

    Matrix operator%(int value) {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = (((int)data[i][j] % value) + value) % value;
            }
        }
        return result;
    }

    void print() {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

Matrix generateKey(std::string key, int n) {
    Matrix k(n, n);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            k[i][j] = (int)(key.at(i * n + j) - 'a');
        }
    }

    return k;
}

std::vector<Matrix> generateVectors(std::string input, int n) {
    for (int i = 0; i < input.length() % n; ++i) {
        input += 'a';
    }

    std::vector<Matrix> result(input.length() / n, Matrix(n, 1));

    for (size_t i = 0; i < input.length() / n; i ++) {
        for (size_t j = 0; j < n; ++j) {
            result[i][j][0] = (int)(input.at(i * n + j) - 'a');
        }
    }

    return result;
}

std::string vectorToString(std::vector<Matrix> vecs) {
    std::string s = "";
    for (Matrix m: vecs) {
        for (size_t i = 0; i < m.rows; ++i) {
            s += (char)(m[i][0] + 'a');
        }
    }

    return s;
}

std::string encrypt(Matrix key, std::vector<Matrix> data, int n) {
    std::vector<Matrix> outputVecs(data.size(), Matrix(n, 1));

    for (size_t i = 0; i < data.size(); ++i) {
        outputVecs[i] = (key * data[i]) % 26;
    }

    return vectorToString(outputVecs);
} 

std::string decrypt(Matrix key, std::string cyphertext, int n) {
    std::vector<Matrix> data = generateVectors(cyphertext, n);
    Matrix newKey = key.inverse();
    std::string decryptedText = encrypt(newKey, data, n);
    return decryptedText;
}

void HillCypher(std::string input, std::string key, int n) {
    Matrix k = generateKey(key, n);
    std::vector<Matrix> data = generateVectors(input, n);

    std::string encryptedText = encrypt(k, data, n);

    std::cout << "Encrypted Text: " << encryptedText << "\n";

    std::cout << "Plaintext: " << decrypt(k, encryptedText, n) << "\n";
}

int main() {
    std::string key, input;
    int n; std::cout << "Enter number of characters to be encrypted at a time: "; std::cin >> n;
    
    std::cout << "Enter the KEY: "; std::cin >> key;
    if (key.length() != n * n) {
        throw std::runtime_error("Key does not have required length");
    }

    std::cout << "Enter the INPUT: "; std::cin >> input;

    HillCypher(input, key, n);
    return 0;
}