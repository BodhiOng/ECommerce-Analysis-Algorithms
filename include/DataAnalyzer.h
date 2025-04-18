#ifndef DATA_ANALYZER_H
#define DATA_ANALYZER_H

#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "Array.h"
#include "DataStructures.h"

class DataAnalyzer {
private:
    Array<Transaction> transactions;
    Array<Review> reviews;
    Array<WordFrequency> wordFrequencies;

    // Helper function to convert string to lowercase
    std::string toLowerCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // Helper function to split string into words
    Array<std::string> splitIntoWords(const std::string& text) {
        Array<std::string> words;
        std::istringstream iss(text);
        std::string word;
        
        while (iss >> word) {
            // Remove punctuation and convert to lowercase
            word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
            if (!word.empty()) {
                words.push_back(toLowerCase(word));
            }
        }
        return words;
    }

public:
    // Add transaction to the array
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

    // Add review to the array
    void addReview(const Review& review) {
        reviews.push_back(review);
    }

    // Sort transactions by date
    void sortTransactionsByDate() {
        transactions.quickSort(compareTransactionsByDate);
    }

    // Filter transactions by category
    Array<Transaction> filterByCategory(const std::string& category) {
        Array<Transaction> filtered;
        for (int i = 0; i < transactions.getSize(); i++) {
            if (toLowerCase(transactions[i].category) == toLowerCase(category)) {
                filtered.push_back(transactions[i]);
            }
        }
        return filtered;
    }

    // Filter transactions by payment method
    Array<Transaction> filterByPaymentMethod(const std::string& method) {
        Array<Transaction> filtered;
        for (int i = 0; i < transactions.getSize(); i++) {
            if (toLowerCase(transactions[i].paymentMethod) == toLowerCase(method)) {
                filtered.push_back(transactions[i]);
            }
        }
        return filtered;
    }

    // Analyze negative reviews (1-star ratings)
    void analyzeNegativeReviews() {
        // Reset word frequencies
        wordFrequencies = Array<WordFrequency>();
        
        // Process each review
        for (int i = 0; i < reviews.getSize(); i++) {
            if (reviews[i].rating == 1) {
                Array<std::string> words = splitIntoWords(reviews[i].reviewText);
                
                // Count word frequencies
                for (int j = 0; j < words.getSize(); j++) {
                    bool found = false;
                    for (int k = 0; k < wordFrequencies.getSize(); k++) {
                        if (wordFrequencies[k].word == words[j]) {
                            wordFrequencies[k].frequency++;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        wordFrequencies.push_back(WordFrequency(words[j], 1));
                    }
                }
            }
        }

        // Sort word frequencies
        wordFrequencies.quickSort(compareWordFrequency);
    }

    // Get top N frequent words
    Array<WordFrequency> getTopFrequentWords(int n) {
        Array<WordFrequency> result;
        for (int i = 0; i < n && i < wordFrequencies.getSize(); i++) {
            result.push_back(wordFrequencies[i]);
        }
        return result;
    }

    // Getters for the arrays
    const Array<Transaction>& getTransactions() const { return transactions; }
    const Array<Review>& getReviews() const { return reviews; }
};

#endif
