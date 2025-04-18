#include <iostream>
#include <fstream>
#include <sstream>
#include "include/DataAnalyzer.h"

// Helper function to split CSV line
Array<std::string> splitCSV(const std::string& line) {
    Array<std::string> result;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, ',')) {
        // Remove quotes if present
        if (item.front() == '"' && item.back() == '"') {
            item = item.substr(1, item.length() - 2);
        }
        result.push_back(item);
    }
    return result;
}

int main() {
    DataAnalyzer analyzer;

    // Read transactions
    std::ifstream transFile("transactions_cleaned.csv");
    std::string line;
    
    // Skip header
    std::getline(transFile, line);
    
    while (std::getline(transFile, line)) {
        Array<std::string> fields = splitCSV(line);
        if (fields.getSize() >= 6) {
            Transaction trans(
                fields[0], // Customer ID
                fields[1], // Product
                fields[2], // Category
                std::stod(fields[3]), // Price
                fields[4], // Date
                fields[5]  // Payment Method
            );
            analyzer.addTransaction(trans);
        }
    }

    // Read reviews
    std::ifstream reviewFile("reviews_cleaned.csv");
    
    // Skip header
    std::getline(reviewFile, line);
    
    while (std::getline(reviewFile, line)) {
        Array<std::string> fields = splitCSV(line);
        if (fields.getSize() >= 4) {
            Review review(
                fields[0], // Product ID
                fields[1], // Customer ID
                std::stoi(fields[2]), // Rating
                fields[3]  // Review Text
            );
            analyzer.addReview(review);
        }
    }

    std::cout << "\nAnalyzing negative reviews...\n";
    analyzer.analyzeNegativeReviews();
    Array<WordFrequency> topWords = analyzer.getTopFrequentWords(5);
    std::cout << "Top 5 frequent words in negative reviews:\n";
    for (int i = 0; i < topWords.getSize(); i++) {
        std::cout << topWords[i].word << ": " << topWords[i].frequency << " occurrences\n";
    }

    return 0;
}
