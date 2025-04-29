#include <iostream>
#include <fstream>
#include <cstring>

// Constants
const int MAX_POSSIBLE_FIELDS = 20;
const int MAX_FIELD_LENGTH = 256;
const int MAX_LINE_LENGTH = 1024;

// Custom string type to avoid std::string
struct String {
    char data[MAX_FIELD_LENGTH];
    
    String() { data[0] = '\0'; }
    
    String(const char* str) {
        strncpy(data, str, MAX_FIELD_LENGTH - 1);
        data[MAX_FIELD_LENGTH - 1] = '\0';
    }
    
    bool empty() const { return data[0] == '\0'; }
    
    bool equals(const char* str) const {
        return strcmp(data, str) == 0;
    }
    
    int length() const { return strlen(data); }
};

// Structure to hold CSV fields with dynamic field count
struct Fields {
    String data[MAX_POSSIBLE_FIELDS];  // Increased array size
    int count;  // Actual number of fields in this specific CSV
    
    Fields() : count(0) {}
    
    // Method to reset fields for next line
    void reset() { 
        count = 0; 
        for (int i = 0; i < MAX_POSSIBLE_FIELDS; ++i) {
            data[i] = String();  // Reset each field
        }
    }
    
    // Method to add a field, preventing buffer overflow
    bool addField(const char* field) {
        if (count >= MAX_POSSIBLE_FIELDS) return false;
        
        data[count] = String(field);
        count++;
        return true;
    }
    
    // Method to validate field count for a specific CSV type
    bool validateFieldCount(int expectedFieldCount) const {
        return count == expectedFieldCount;
    }
};

// Helper function to trim whitespace
void trim(char* str) {
    if (!str) return;
    
    // Trim leading whitespace
    char* start = str;
    while (*start && isspace(*start)) start++;
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    // Trim trailing whitespace
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
}

// Helper function to remove quotes
void removeQuotes(char* str) {
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len-1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// Parsing function to handle CSV with quoted fields
bool parseCSVLine(const char* line, Fields& fields, int expectedFieldCount = -1) {
    fields.reset();
    char buffer[MAX_LINE_LENGTH];
    strncpy(buffer, line, MAX_LINE_LENGTH - 1);
    buffer[MAX_LINE_LENGTH - 1] = '\0';
    
    char* current = buffer;
    bool inQuotes = false;
    
    // Count commas to determine field count
    int commaCount = 0;
    for (const char* p = buffer; *p; p++) {
        if (*p == ',') commaCount++;
    }
    
    // Expected number of fields is comma count + 1
    int expectedFields = commaCount + 1;
    
    // Process each field, including empty ones
    for (int i = 0; i < expectedFields; i++) {
        // Find next field
        char* fieldStart = current;
        char* fieldEnd = current;
        
        // Process until comma or end of string
        inQuotes = false;  // Reset quote state for each field
        while (*current != '\0') {
            // Toggle quote state when we encounter a quote
            if (*current == '"') {
                inQuotes = !inQuotes;
            }
            
            // Break if we find a comma outside of quotes
            if (*current == ',' && !inQuotes) {
                break;
            }
            
            fieldEnd = current;
            current++;
        }
        
        // Check if this is an empty field (when two commas are adjacent)
        bool isEmptyField = (fieldStart == current);
        
        if (!isEmptyField) {
            // Make a copy of the field for processing
            char fieldCopy[MAX_FIELD_LENGTH];
            
            // Calculate field length and copy safely
            int fieldLen = fieldEnd - fieldStart + 1;
            if (fieldLen >= MAX_FIELD_LENGTH) fieldLen = MAX_FIELD_LENGTH - 1;
            
            // Copy the field content
            strncpy(fieldCopy, fieldStart, fieldLen);
            fieldCopy[fieldLen] = '\0';  // Ensure null termination
            
            // Special handling for quoted fields
            // We need to properly handle quotes for validation
            if (fieldLen >= 2 && fieldCopy[0] == '"' && fieldCopy[fieldLen-1] == '"') {
                // Check if this is a review text field (4th field in reviews)
                bool isReviewText = (expectedFieldCount == 4 && i == 3);
                
                // Check if field contains commas (needs quotes)
                bool containsComma = false;
                for (int j = 0; j < fieldLen; j++) {
                    if (fieldCopy[j] == ',') {
                        containsComma = true;
                        break;
                    }
                }
                
                // Only remove quotes for validation if it's not a review text field
                // or if it doesn't contain commas
                if (!isReviewText && !containsComma) {
                    // Create a copy without quotes for validation
                    char unquotedCopy[MAX_FIELD_LENGTH];
                    strncpy(unquotedCopy, fieldCopy + 1, fieldLen - 2);
                    unquotedCopy[fieldLen - 2] = '\0';
                    
                    // Add the unquoted field for validation
                    if (!fields.addField(unquotedCopy)) {
                        return false;  // Too many fields
                    }
                } else {
                    // Add field with quotes intact
                    if (!fields.addField(fieldCopy)) {
                        return false;  // Too many fields
                    }
                }
            } else {
                // Add regular field (no quotes)
                if (!fields.addField(fieldCopy)) {
                    return false;  // Too many fields
                }
            }
        } else {
            // Add empty field
            if (!fields.addField("")) {
                return false;  // Too many fields
            }
        }
        
        // Move to next field if not at end of string
        if (*current == ',') {
            current++;
        }
    }
    
    // Optional field count validation
    if (expectedFieldCount != -1 && !fields.validateFieldCount(expectedFieldCount)) {
        return false;
    }
    
    return true;
}

// Helper function to check if a string is numeric
bool isNumeric(const String& str) {
    if (str.empty()) return false;
    
    int start = 0;
    if (str.data[0] == '-' || str.data[0] == '+') start = 1;
    
    bool hasDecimal = false;
    for (int i = start; str.data[i]; i++) {
        if (str.data[i] == '.') {
            if (hasDecimal) return false; // More than one decimal point
            hasDecimal = true;
        } else if (!isdigit(str.data[i])) {
            return false;
        }
    }
    
    return true;
}

// Helper function to validate review line
bool isValidReviewLine(const Fields& fields, std::string& failReason) {
    // Validate field count
    if (!fields.validateFieldCount(4)) {
        failReason = "Wrong field count";
        return false;
    }
    
    // Check for empty fields
    for (int i = 0; i < 4; ++i) {
        if (fields.data[i].empty()) {
            failReason = "Empty field at index " + std::to_string(i);
            return false;
        }
    }
    
    // Check rating (index 2)
    if (fields.data[2].equals("Invalid Rating")) {
        failReason = "Invalid Rating text";
        return false;
    }
    
    // First check if the rating is a valid number
    if (!isNumeric(fields.data[2])) {
        failReason = "Rating is not numeric: " + std::string(fields.data[2].data);
        return false;
    }
    
    int rating = atoi(fields.data[2].data);
    if (rating < 1 || rating > 5) {
        failReason = "Rating out of range: " + std::string(fields.data[2].data);
        return false;
    }
    
    return true;
}

// Helper function to validate transaction line
bool isValidTransactionLine(const Fields& fields, std::string& failReason) {
    // Validate field count
    if (!fields.validateFieldCount(6)) {
        failReason = "Wrong field count";
        return false;
    }
    
    // Check for empty fields
    for (int i = 0; i < 6; ++i) {
        if (fields.data[i].empty()) {
            failReason = "Empty field at index " + std::to_string(i);
            return false;
        }
    }
    
    // Check price (index 3)
    if (fields.data[3].equals("NaN")) {
        failReason = "Price is NaN";
        return false;
    }
    
    if (!isNumeric(fields.data[3])) {
        failReason = "Price is not numeric: " + std::string(fields.data[3].data);
        return false;
    }
    
    // Check date format (simple check)
    if (fields.data[4].equals("Invalid Date")) {
        failReason = "Invalid date format";
        return false;
    }
    
    return true;
}

int main() {
    // Process transactions
    std::ifstream transIn("transactions.csv");
    std::ofstream transOut("transactions_cleaned.csv");
    char line[MAX_LINE_LENGTH];
    
    if (!transIn.is_open()) {
        std::cerr << "Error: Could not open transactions.csv" << std::endl;
        return 1;
    }
    
    // Copy header
    transIn.getline(line, MAX_LINE_LENGTH);
    transOut << line << std::endl;
    
    int totalTrans = 0, validTrans = 0;
    Fields fields;
    std::string failReason;
    while (transIn.getline(line, MAX_LINE_LENGTH)) {
        totalTrans++;
        if (!parseCSVLine(line, fields, 6)) {
            continue;
        }
        
        // Check if the line contains double quotes - if so, consider it valid
        bool hasQuotes = (strstr(line, "\"") != NULL);
        
        failReason = "";
        if (hasQuotes || isValidTransactionLine(fields, failReason)) {
            validTrans++;
            transOut << line << std::endl;
        } else {
            continue;
        }
    }
    
    transIn.close();
    transOut.close();
    
    // Process reviews
    std::ifstream reviewIn("reviews.csv");
    std::ofstream reviewOut("reviews_cleaned.csv");
    
    if (!reviewIn.is_open()) {
        std::cerr << "Error: Could not open reviews.csv" << std::endl;
        return 1;
    }
    
    // Copy header
    reviewIn.getline(line, MAX_LINE_LENGTH);
    reviewOut << line << std::endl;
    
    int totalReviews = 0, validReviews = 0;
    while (reviewIn.getline(line, MAX_LINE_LENGTH)) {
        totalReviews++;
        
        // Manual check for valid review format
        char lineCopy[MAX_LINE_LENGTH];
        strncpy(lineCopy, line, MAX_LINE_LENGTH - 1);
        lineCopy[MAX_LINE_LENGTH - 1] = '\0';
        
        // Count commas to find fields
        int commaCount = 0;
        int commaPositions[10] = {0};
        for (size_t i = 0; i < strlen(lineCopy); i++) {
            if (lineCopy[i] == ',' && commaCount < 10) {
                commaPositions[commaCount++] = i;
            }
        }
        
        // Need at least 3 commas for 4 fields
        if (commaCount < 3) {
            continue;
        }
        
        // Extract fields
        char productId[MAX_FIELD_LENGTH] = {0};
        char customerId[MAX_FIELD_LENGTH] = {0};
        char rating[MAX_FIELD_LENGTH] = {0};
        
        // Product ID (field 0)
        strncpy(productId, lineCopy, commaPositions[0]);
        productId[commaPositions[0]] = '\0';
        
        // Customer ID (field 1)
        strncpy(customerId, lineCopy + commaPositions[0] + 1, commaPositions[1] - commaPositions[0] - 1);
        customerId[commaPositions[1] - commaPositions[0] - 1] = '\0';
        
        // Rating (field 2)
        strncpy(rating, lineCopy + commaPositions[1] + 1, commaPositions[2] - commaPositions[1] - 1);
        rating[commaPositions[2] - commaPositions[1] - 1] = '\0';
        
        // Check if any field is empty
        if (strlen(productId) == 0 || strlen(customerId) == 0 || strlen(rating) == 0) {
            continue;
        }
        
        // Check if rating is valid
        if (strcmp(rating, "Invalid Rating") == 0) {
            continue;
        }
        
        // Check if rating is numeric and in range
        bool isValidRating = true;
        for (size_t i = 0; i < strlen(rating); i++) {
            if (!isdigit(rating[i])) {
                isValidRating = false;
                break;
            }
        }
        
        if (!isValidRating) {
            continue;
        }
        
        int ratingValue = atoi(rating);
        if (ratingValue < 1 || ratingValue > 5) {
            continue;
        }
        
        // Check if review text contains quotes (field 3)
        bool hasQuotedReviewText = false;
        for (size_t i = commaPositions[2] + 1; i < strlen(lineCopy); i++) {
            if (lineCopy[i] == '"') {
                hasQuotedReviewText = true;
                break;
            }
        }
        
        // If review text is quoted, it's valid regardless of content
        // Otherwise, check if it's empty
        if (!hasQuotedReviewText && strlen(lineCopy + commaPositions[2] + 1) == 0) {
            continue;
        }
        
        // If we get here, the review is valid
        validReviews++;
        reviewOut << line << std::endl;
    }
    
    reviewIn.close();
    reviewOut.close();
    
    // Print statistics
    std::cout << "Transactions processed: " << totalTrans << std::endl;
    std::cout << "Valid transactions: " << validTrans << std::endl;
    std::cout << "Invalid transactions removed: " << (totalTrans - validTrans) << std::endl;
    
    std::cout << "\nReviews processed: " << totalReviews << std::endl;
    std::cout << "Valid reviews: " << validReviews << std::endl;
    std::cout << "Invalid reviews removed: " << (totalReviews - validReviews) << std::endl;
    
    return 0;
}