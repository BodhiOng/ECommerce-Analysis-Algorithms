#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <string>
#include <ctime>
#include "Array.h"

struct Review {
    std::string productId;
    std::string customerId;
    int rating;
    std::string reviewText;

    Review() = default;
    Review(const std::string& pid, const std::string& cid, int r, const std::string& text)
        : productId(pid), customerId(cid), rating(r), reviewText(text) {}
};

struct Transaction {
    std::string customerId;
    std::string product;
    std::string category;
    double price;
    std::string date;
    std::string paymentMethod;

    Transaction() = default;
    Transaction(const std::string& cid, const std::string& prod, const std::string& cat,
               double p, const std::string& d, const std::string& pm)
        : customerId(cid), product(prod), category(cat), price(p), date(d), paymentMethod(pm) {}
};

// Comparison functions for sorting
inline int compareTransactionsByDate(const Transaction& a, const Transaction& b) {
    // Convert date strings to comparable format (assuming MM/DD/YYYY format)
    std::string aDate = a.date.substr(6,4) + a.date.substr(0,2) + a.date.substr(3,2);
    std::string bDate = b.date.substr(6,4) + b.date.substr(0,2) + b.date.substr(3,2);
    return aDate.compare(bDate);
}

// Word frequency counter for review analysis
struct WordFrequency {
    std::string word;
    int frequency;

    WordFrequency(const std::string& w = "", int f = 0) : word(w), frequency(f) {}
};

inline int compareWordFrequency(const WordFrequency& a, const WordFrequency& b) {
    return b.frequency - a.frequency; // Sort in descending order
}

#endif
