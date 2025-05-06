# E-commerce Analysis Algorithms

This is E-commerce analysis algorithms developed as part of the CT077-3-2-DSTR (Data Structures) group assignment for the Asia Pacific University (APU) in 2025. This repository contains a set of custom data structures and algorithms designed for analyzing e-commerce data, specifically transaction records and product reviews. The implementation uses both array-based and linked list-based data structures to demonstrate different approaches to data analysis.

## Project Overview

This project implements custom data structures (Array and LinkedList) from scratch in C++ and uses them to analyze e-commerce data. The analysis includes:

- Processing transaction records
- Analyzing product reviews
- Identifying frequent words in negative reviews
- Sorting data using custom implementations of sorting algorithms

## Data Structures

The project implements two main data structures:

### Array Implementation

A dynamic array implementation with the following features:
- Automatic resizing
- Push operations
- Index-based access
- Copy constructor and assignment operator

### Linked List Implementation

A singly linked list implementation with the following features:
- Node-based structure
- Push operations
- Traversal functionality
- Copy constructor and assignment operator

## Algorithms

The project includes several algorithm implementations:

### Sorting Algorithms

- **Quick Sort**: Implemented for both array and linked list data structures
- **Comparison Functions**: Custom comparison functions for sorting by different criteria

### Search Algorithms

- **Linear Search**: Used for word frequency analysis in reviews

### Data Processing

- **Data Cleaning**: Functions to clean and validate CSV data
- **Text Analysis**: Functions to analyze review text, including word frequency counting

## Data Files

The repository includes sample data files:

- `transactions.csv`: Raw transaction data
- `transactions_cleaned.csv`: Cleaned transaction data
- `reviews.csv`: Raw product review data
- `reviews_cleaned.csv`: Cleaned product review data

## Usage

### Data Cleaning

To clean the raw data files:

```bash
# Compile the data cleaning program
g++ -o clean_data cleanData.cpp

# Run the program
./clean_data
```

This will process the raw CSV files and generate cleaned versions.

### Data Analysis

To analyze the data using array-based implementation:

```bash
# Compile the array implementation
g++ -o array_analysis arrayImplementation.cpp

# Run the program
./array_analysis
```

To analyze the data using linked list-based implementation:

```bash
# Compile the linked list implementation
g++ -o linked_list_analysis linkedListImplementation.cpp

# Run the program
./linked_list_analysis
```

## Project Structure

```
├── include/                  # Header files
│   ├── Array.h               # Dynamic array implementation
│   ├── ArrayDataAnalyzer.h   # Array-based data analysis
│   ├── DataStructures.h      # Common data structures
│   ├── LinkedList.h          # Linked list implementation
│   └── LinkedListDataAnalyzer.h  # Linked list-based data analysis
├── arrayImplementation.cpp   # Main program using array implementation
├── linkedListImplementation.cpp  # Main program using linked list implementation
├── cleanData.cpp             # Data cleaning utility
├── transactions.csv          # Raw transaction data
├── transactions_cleaned.csv  # Cleaned transaction data
├── reviews.csv               # Raw review data
└── reviews_cleaned.csv       # Cleaned review data
```

## Implementation Details

### Data Structures

The project implements custom data structures rather than using the C++ Standard Library containers to demonstrate fundamental data structure concepts:

- **Template-based**: Both Array and LinkedList are template-based to handle different data types
- **Memory Management**: Proper memory management with destructors and copy constructors
- **Dynamic Resizing**: The Array implementation includes dynamic resizing when capacity is reached

### Analysis Features

- **Transaction Analysis**: Sort and analyze transaction data by date, category, etc.
- **Review Sentiment Analysis**: Identify common words in negative reviews
- **Word Frequency Analysis**: Count and rank most frequent words in reviews

## Requirements

- C++ compiler with C++11 support
- Standard input/output libraries

## License

This project is available for educational and research purposes.
