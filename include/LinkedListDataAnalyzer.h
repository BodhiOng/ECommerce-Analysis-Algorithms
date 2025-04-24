#ifndef LINKED_LIST_DATA_ANALYZER_H
#define LINKED_LIST_DATA_ANALYZER_H

#include <string>
#include <cctype>
#include <algorithm>
#include "LinkedList.h"
#include "DataStructures.h"

class LinkedListDataAnalyzer {
private:
    LinkedList<Transaction> transactions;
    LinkedList<Review> reviews;
    LinkedList<WordFrequency> wordFrequencies;

    // Helper function to convert string to lowercase
    std::string toLowerCase(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    // Helper function to split string into words
    LinkedList<std::string> splitIntoWords(const std::string& text) {
        LinkedList<std::string> words;
        std::string word;
        size_t start = 0;
        
        // Custom word splitting without using stringstream
        for (size_t i = 0; i <= text.length(); i++) {
            if (i == text.length() || std::isspace(text[i])) {
                if (i > start) {
                    word = text.substr(start, i - start);
                    // Remove punctuation manually
                    std::string clean;
                    for (char c : word) {
                        if (!std::ispunct(c)) {
                            clean += std::tolower(c);
                        }
                    }
                    if (!clean.empty()) {
                        words.add(clean);
                    }
                }
                start = i + 1;
            }
        }
        return words;
    }

    // Helper function to get list size
    template<typename T>
    int getListSize(const LinkedList<T>& list) {
        int size = 0;
        Node<T>* current = list.begin();
        while (current) {
            size++;
            current = current->next;
        }
        return size;
    }

    // Helper function to get nth node
    template<typename T>
    Node<T>* getNode(LinkedList<T>& list, int n) {
        Node<T>* current = list.begin();
        for (int i = 0; i < n && current; i++) {
            current = current->next;
        }
        return current;
    }

    // Merge two sorted lists
    template<typename T>
    Node<T>* merge(Node<T>* left, Node<T>* right, int (*compare)(const T&, const T&)) {
        if (!left) return right;
        if (!right) return left;

        Node<T>* result = nullptr;

        if (compare(left->data, right->data) <= 0) {
            result = left;
            result->next = merge(left->next, right, compare);
        } else {
            result = right;
            result->next = merge(left, right->next, compare);
        }

        return result;
    }

    // Get middle node of list
    template<typename T>
    Node<T>* getMiddle(Node<T>* head) {
        if (!head) return head;
        
        Node<T>* slow = head;
        Node<T>* fast = head->next;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

public:
    // Merge sort implementation
    template<typename T>
    Node<T>* mergeSort(Node<T>* head, int (*compare)(const T&, const T&)) {
        if (!head || !head->next) return head;

        Node<T>* middle = getMiddle(head);
        Node<T>* nextToMiddle = middle->next;

        middle->next = nullptr;

        Node<T>* left = mergeSort(head, compare);
        Node<T>* right = mergeSort(nextToMiddle, compare);

        return merge(left, right, compare);
    }

    // Add transaction to the list
    void addTransaction(const Transaction& transaction) {
        transactions.add(transaction);
    }

    // Add review to the list
    void addReview(const Review& review) {
        reviews.add(review);
    }

    // Linear search for word frequency counting
    void linearSearch(const LinkedList<std::string>& words, LinkedList<WordFrequency>& frequencies) {
        Node<std::string>* wordNode = words.begin();
        while (wordNode) {
            bool found = false;
            Node<WordFrequency>* freqNode = frequencies.begin();
            
            while (freqNode) {
                if (freqNode->data.word == wordNode->data) {
                    freqNode->data.frequency++;
                    found = true;
                    break;
                }
                freqNode = freqNode->next;
            }
            
            if (!found) {
                frequencies.add(WordFrequency(wordNode->data, 1));
            }
            wordNode = wordNode->next;
        }
    }

    // Analyze negative reviews (1-star ratings)
    void analyzeNegativeReviews() {
        Node<Review>* current = reviews.begin();
        while (current) {
            if (current->data.rating == 1) {
                LinkedList<std::string> words = splitIntoWords(current->data.reviewText);
                linearSearch(words, wordFrequencies);
            }
            current = current->next;
        }

        // Sort word frequencies using merge sort
        Node<WordFrequency>* sortedHead = mergeSort(wordFrequencies.begin(), compareWordFrequency);
        // Update the head of wordFrequencies
        wordFrequencies = LinkedList<WordFrequency>();  // Clear the list
        Node<WordFrequency>* current_node = sortedHead;
        while (current_node) {
            wordFrequencies.add(current_node->data);
            current_node = current_node->next;
        }
    }

    // Get top N frequent words
    LinkedList<WordFrequency> getTopFrequentWords(int n) {
        LinkedList<WordFrequency> result;
        Node<WordFrequency>* current = wordFrequencies.begin();
        int count = 0;
        
        while (current && count < n) {
            result.add(current->data);
            current = current->next;
            count++;
        }
        return result;
    }

    // Getters for the lists
    const LinkedList<Transaction>& getTransactions() const { return transactions; }
    const LinkedList<Review>& getReviews() const { return reviews; }
};

#endif
