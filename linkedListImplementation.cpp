#include <iostream>
#include <fstream>
#include <iomanip>
#include "include/LinkedListDataAnalyzer.h"

// Helper function to split CSV line
LinkedList<std::string> splitCSV(const std::string& line) {
    LinkedList<std::string> result;
    std::string item;
    size_t start = 0;
    size_t end = 0;
    
    while ((end = line.find(',', start)) != std::string::npos) {
        item = line.substr(start, end - start);
        // Remove quotes if present
        if (item.front() == '"' && item.back() == '"') {
            item = item.substr(1, item.length() - 2);
        }
        result.add(item);
        start = end + 1;
    }
    // Add the last item
    item = line.substr(start);
    if (item.front() == '"' && item.back() == '"') {
        item = item.substr(1, item.length() - 2);
    }
    result.add(item);
    return result;
}

int main() {
    LinkedListDataAnalyzer analyzer;

    // Read transactions
    std::ifstream transFile("transactions_cleaned.csv");
    std::string line;
    
    // Skip header
    std::getline(transFile, line);
    
    while (std::getline(transFile, line)) {
        LinkedList<std::string> fields = splitCSV(line);
        int size = 0;
        Node<std::string>* current = fields.begin();
        while (current) {
            size++;
            current = current->next;
        }
        
        if (size >= 6) {
            current = fields.begin();
            std::string customerId = current->data;
            current = current->next;
            std::string product = current->data;
            current = current->next;
            std::string category = current->data;
            current = current->next;
            double price = std::stod(current->data);
            current = current->next;
            std::string date = current->data;
            current = current->next;
            std::string paymentMethod = current->data;
            
            Transaction trans(customerId, product, category, price, date, paymentMethod);
            analyzer.addTransaction(trans);
        }
    }

    // Read reviews
    std::ifstream reviewFile("reviews_cleaned.csv");
    
    // Skip header
    std::getline(reviewFile, line);
    
    while (std::getline(reviewFile, line)) {
        LinkedList<std::string> fields = splitCSV(line);
        int size = 0;
        Node<std::string>* current = fields.begin();
        while (current) {
            size++;
            current = current->next;
        }
        
        if (size >= 4) {
            current = fields.begin();
            std::string productId = current->data;
            current = current->next;
            std::string customerId = current->data;
            current = current->next;
            int rating = std::stoi(current->data);
            current = current->next;
            std::string reviewText = current->data;
            
            Review review(productId, customerId, rating, reviewText);
            analyzer.addReview(review);
        }
    }

    // 1. How can you efficiently sort customer transactions by date and display the total number of transactions in both datasets?
    std::cout << "1. How can you efficiently sort customer transactions by date and display the total number of transactions in both datasets?" << std::endl;
    
    // Get all transactions and sort them by date
    LinkedList<Transaction> transactions = analyzer.getTransactions();
    Node<Transaction>* sortedHead = analyzer.mergeSort(transactions.begin(), compareTransactionsByDate);
    
    // Count total transactions
    int totalTransactions = 0;
    Node<Transaction>* current = sortedHead;
    while (current) {
        totalTransactions++;
        current = current->next;
    }
    
    // Update the transactions list with sorted nodes
    transactions = LinkedList<Transaction>();  // Clear the list
    current = sortedHead;
    while (current) {
        transactions.add(current->data);
        current = current->next;
    }

    std::cout << "\nTotal number of transactions: " << totalTransactions << std::endl;

    // Display first 100 transactions
    std::cout << "\nFirst 100 transactions (sorted by date):" << std::endl;
    std::cout << "Date, Customer ID, Product, Category, Price, Payment Method" << std::endl;

    int displayCount = 0;
    current = sortedHead;
    while (current && displayCount < 100) {
        const Transaction& t = current->data;
        std::cout << t.date << ", " 
                  << t.customerId << ", "
                  << t.product << ", "
                  << t.category << ", "
                  << t.price << ", "
                  << t.paymentMethod << std::endl;
        current = current->next;
        displayCount++;
    }

    if (totalTransactions > 100) {
        std::cout << "\n... and " << (totalTransactions - 100) << " more transactions" << std::endl;
    }

    // 2. What percentage of purchases in the "Electronics" category were made using Credit Card payments?
    std::cout << "\n2. What percentage of purchases in the \"Electronics\" category were made using Credit Card payments?" << std::endl;
    
    int totalElectronics = 0;
    int electronicsCreditCard = 0;
    
    current = sortedHead;
    while (current) {
        const Transaction& t = current->data;
        if (t.category == "Electronics") {
            totalElectronics++;
            if (t.paymentMethod == "Credit Card") {
                electronicsCreditCard++;
            }
        }
        current = current->next;
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

    // Analyze negative reviews
    analyzer.analyzeNegativeReviews();
    LinkedList<WordFrequency> topWords = analyzer.getTopFrequentWords(5);

    std::cout << "\nTop 5 frequent words in negative reviews:\n";
    Node<WordFrequency>* wordNode = topWords.begin();
    while (wordNode) {
        std::cout << wordNode->data.word << ": " << wordNode->data.frequency << " occurrences\n";
        wordNode = wordNode->next;
    }

    return 0;
}
