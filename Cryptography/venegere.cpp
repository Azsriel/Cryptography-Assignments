#include <iostream>

int main() {
    std::string k, input;    
    std::cout << "Enter the KEY: "; std::cin >> k;
    std::cout << "Enter the INPUT: "; std::cin >> input;

    int n = input.length() / k.length();
    int m = input.length() % k.length();

    std::string key = "";
    for (size_t i = 0; i < n; ++i) {
        key += k;
    }
    key += k.substr(0, m);

    std::string cypherText = "";

    for (size_t i = 0; i < input.length(); ++i) {
        int one = input.at(i) - 'a';
        int two = key.at(i) - 'a';
        int sum = (one + two) % 26;
        cypherText += (char)(sum + 'a');
    }

    std::cout << "Cyphertext: " << cypherText << "\n";

    std::string plainText = "";

    for (size_t i = 0; i < cypherText.length(); ++i) {
        plainText += (char)((((int)cypherText.at(i) - (int)key.at(i) + 26)%26) + 'a');
    }

    std::cout << "Plaintext: " << plainText << "\n";
}