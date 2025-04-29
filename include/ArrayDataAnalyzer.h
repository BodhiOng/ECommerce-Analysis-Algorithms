#ifndef ARRAY_DATA_ANALYZER_H
#define ARRAY_DATA_ANALYZER_H

#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stack>
#include "Array.h"
#include "DataStructures.h"

class ArrayDataAnalyzer {
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
    // Quick sort implementation
    template<typename T>
    void quickSort(Array<T>& array, int (*compare)(const T&, const T&)) {
        if (array.getSize() <= 1) return;
        
        std::stack<std::pair<int, int>> stack;
        stack.push({0, array.getSize() - 1});

        while (!stack.empty()) {
            int low = stack.top().first;
            int high = stack.top().second;
            stack.pop();

            if (low < high) {
                T pivot = array[high];
                int i = low - 1;

                for (int j = low; j < high; j++) {
                    if (compare(array[j], pivot) <= 0) {
                        i++;
                        std::swap(array[i], array[j]);
                    }
                }
                std::swap(array[i + 1], array[high]);

                int pi = i + 1;
                
                // Push right subarray first, then left
                stack.push({pi + 1, high});
                stack.push({low, pi - 1});
            }
        }
    }

    // Add transaction to the array
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

    // Add review to the array
    void addReview(const Review& review) {
        reviews.push_back(review);
    }

    // Linear search for word frequency counting
    void linearSearch(const Array<std::string>& words, Array<WordFrequency>& frequencies) {
        for (int j = 0; j < words.getSize(); j++) {
            bool found = false;
            for (int k = 0; k < frequencies.getSize(); k++) {
                if (frequencies[k].word == words[j]) {
                    frequencies[k].frequency++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                frequencies.push_back(WordFrequency(words[j], 1));
            }
        }
    }

    // Analyze negative reviews (1-star ratings)
    void analyzeNegativeReviews() {
        // Process each review
        for (int i = 0; i < reviews.getSize(); i++) {
            if (reviews[i].rating == 1) {
                Array<std::string> words = splitIntoWords(reviews[i].reviewText);
                linearSearch(words, wordFrequencies);
            }
        }

        // Sort word frequencies
        quickSort(wordFrequencies, compareWordFrequency);
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
