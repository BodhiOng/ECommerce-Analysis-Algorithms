#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "include/ArrayDataAnalyzer.h"

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
    ArrayDataAnalyzer analyzer;

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

    // 1. How can you efficiently sort customer transactions by date and display the total number of transactions in both datasets?
    std::cout << "1. How can you efficiently sort customer transactions by date and display the total number of transactions in both datasets?" << std::endl;
    
    // Get all transactions and sort them by date
    Array<Transaction> transactions = analyzer.getTransactions();
    analyzer.quickSort(transactions, compareTransactionsByDate);

    // Display total number of transactions
    int totalTransactions = transactions.getSize();
    std::cout << "\nTotal number of transactions: " << totalTransactions << std::endl;

    // Display first 100 transactions
    std::cout << "\nFirst 100 transactions (sorted by date):" << std::endl;
    std::cout << "Date, Customer ID, Product, Category, Price, Payment Method" << std::endl;

    int displayCount = std::min(100, totalTransactions);
    for (int i = 0; i < displayCount; i++) {
        const Transaction& t = transactions[i];
        std::cout << t.date << ", " 
                  << t.customerId << ", "
                  << t.product << ", "
                  << t.category << ", "
                  << t.price << ", "
                  << t.paymentMethod << std::endl;
    }

    if (totalTransactions > 100) {
        std::cout << "\n... and " << (totalTransactions - 100) << " more transactions" << std::endl;
    }

    // 2. What percentage of purchases in the "Electronics" category were made using Credit Card payments?
    std::cout << "\n2. What percentage of purchases in the \"Electronics\" category were made using Credit Card payments?" << std::endl;
    
    int totalElectronics = 0;
    int electronicsCreditCard = 0;
    
    for (int i = 0; i < transactions.getSize(); i++) {
        const Transaction& t = transactions[i];
        if (t.category == "Electronics") {
            totalElectronics++;
            if (t.paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
    }
    
    if (totalElectronics > 0) {
        double percentage = (static_cast<double>(electronicsCreditCard) / totalElectronics) * 100;
        std::cout << "\nOut of " << totalElectronics << " Electronics purchases:" << std::endl;
        std::cout << electronicsCreditCard << " were made with Credit Card (" << std::fixed << std::setprecision(2) << percentage << "%)" << std::endl;
    } else {
        std::cout << "\nNo Electronics purchases found in the dataset." << std::endl;
    }

    // 3. Which words are most frequently used in product reviews rated 1-star?
    std::cout << "\n3. Which words are most frequently used in product reviews rated 1-star?" << std::endl;

    // Get all reviews and analyze negative reviews
    Array<Review> reviews = analyzer.getReviews();
    analyzer.analyzeNegativeReviews();
    Array<WordFrequency> topWords = analyzer.getTopFrequentWords(5);

    std::cout << "\nTop 5 frequent words in negative reviews:\n";
    for (int i = 0; i < topWords.getSize(); i++) {
        std::cout << topWords[i].word << ": " << topWords[i].frequency << " occurrences\n";
    }

    return 0;
}
