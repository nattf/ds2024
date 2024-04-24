#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> wordCount;

    // Read key-value pairs from standard input
    std::string word;
    int count;
    while (std::cin >> word >> count) {
        // Increment the count for each word
        wordCount[word] += count;
    }

    // Emit the final word count to standard output
    for (const auto& pair : wordCount) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }

    return 0;
}