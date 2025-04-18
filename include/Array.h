#ifndef ARRAY_H
#define ARRAY_H

template <typename T>
class Array {
private:
    T* data;
    int capacity;
    int size;

    void resize(int newCapacity) {
        T* newData = new T[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Array(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
        data = new T[capacity];
    }

    // Copy constructor
    Array(const Array& other) : capacity(other.capacity), size(other.size) {
        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~Array() {
        delete[] data;
    }

    void push_back(const T& element) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        data[size++] = element;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    int getSize() const { return size; }

    // Copy assignment operator
    Array& operator=(const Array& other) {
        if (this != &other) {
            // Delete old data
            delete[] data;
            
            // Copy from other
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            
            // Copy elements
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
};

#endif
