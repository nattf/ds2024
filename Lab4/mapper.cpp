#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> wordCount;

    // Read input from standard input
    std::string word;
    while (std::cin >> word) {
        // Increment the count for each word
        wordCount[word]++;
    }

    // Emit key-value pairs to standard output
    for (const auto& pair : wordCount) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }

    return 0;
}