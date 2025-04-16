#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

// Structure to represent a Review
struct Review {
    string productID; 
    string customerID;
    int rating;
    string reviewText; 
};

// Structure to represent a Transaction
struct Transaction {
    string customerID;
    string product;
    string category;
    double price;
    string date; 
    string paymentMethod;
};

// Custom DynamicArray class for managing a resizable array
class DynamicArray {
private:
    Transaction* data; // Pointer to the dynamic array
    int capacity; // Current capacity of the array
    int size; // Current number of elements in the array
//test
    // Method to resize the array when it runs out of space
    void resize() {
        if (capacity <= 0) {
            throw logic_error("Invalid capacity in resize operation");
        }
        
        int newCapacity = capacity * 2;
        try {
            Transaction* newData = new Transaction[newCapacity];
            
            // Copy old data to the new array
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }

            // Free the old array and update pointers
            delete[] data;
            data = newData;
            capacity = newCapacity;
        } catch (const bad_alloc& e) {
            throw runtime_error("Failed to allocate memory for resized array");
        }
    }

public:
    // Constructor
    DynamicArray(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
        if (initialCapacity <= 0) {
            throw invalid_argument("Initial capacity must be greater than 0");
        }
        try {
            data = new Transaction[capacity];
        } catch (const bad_alloc& e) {
            throw runtime_error("Failed to allocate memory for array");
        }
    }

    // Destructor
    ~DynamicArray() {
        if (data != nullptr) {
            delete[] data;
        }
    }

    // Method to add a new transaction
    void add(const Transaction& transaction) {
        // Validate transaction data
        if (transaction.customerID.empty() || transaction.product.empty() || 
            transaction.category.empty() || transaction.date.empty() || 
            transaction.paymentMethod.empty()) {
            throw invalid_argument("Transaction data cannot contain empty fields");
        }
        
        if (transaction.price <= 0) {
            throw invalid_argument("Transaction price must be greater than 0");
        }

        if (size == capacity) {
            resize();  // Resize if the array is full
        }
        try {
            data[size++] = transaction;  // Add the new transaction
        } catch (const bad_alloc& e) {
            throw runtime_error("Failed to copy transaction data");
        }
    }

    // Method to get a transaction at a specific index
    Transaction get(int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range. Valid range is 0 to " + to_string(size-1));
        }
        return data[index];
    }

    // Method to get the current size of the array
    int getSize() const {
        return size;
    }

    // Method to print all transactions
    void print() const {
        for (int i = 0; i < size; i++) {
            cout << "Customer ID: " << data[i].customerID
                 << ", Product: " << data[i].product
                 << ", Category: " << data[i].category
                 << ", Price: $" << data[i].price
                 << ", Date: " << data[i].date
                 << ", Payment Method: " << data[i].paymentMethod << endl;
        }
    }
};

// Sample main function to demonstrate usage
int main() {
    // Fixed-size array for reviews
    const int reviewCapacity = 3;
    Review reviews[reviewCapacity] = {
        {"PROD274", "CUST7329", 4, "Customer service was unhelpful."},
        {"PROD101", "CUST1122", 5, "Excellent product! Highly recommend."},
        {"PROD333", "CUST4523", 3, "Average product, could be better."}
    };

    // Dynamic array for transactions
    DynamicArray transactions;

    // Adding transactions to the dynamic array
    transactions.add({"CUST4434", "Mouse", "Automotive", 483.15, "9/5/2023", "Bank Transfer"});
    transactions.add({"CUST7329", "Keyboard", "Electronics", 1000.00, "8/15/2023", "Credit Card"});
    transactions.add({"CUST1122", "Monitor", "Electronics", 150.75, "10/10/2023", "PayPal"});

    // Output reviews
    cout << "Reviews Data:" << endl;
    for (int i = 0; i < reviewCapacity; i++) {
        cout << "Product ID: " << reviews[i].productID
             << ", Customer ID: " << reviews[i].customerID
             << ", Rating: " << reviews[i].rating
             << ", Review Text: " << reviews[i].reviewText << endl;
    }

    // Output transactions
    cout << "\nTransactions Data:" << endl;
    transactions.print();

    return 0;
}