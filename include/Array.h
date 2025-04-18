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

    // Quick sort implementation
    void quickSort(int (*compare)(const T&, const T&)) {
        quickSortHelper(0, size - 1, compare);
    }

private:
    void quickSortHelper(int low, int high, int (*compare)(const T&, const T&)) {
        if (low < high) {
            int pi = partition(low, high, compare);
            quickSortHelper(low, pi - 1, compare);
            quickSortHelper(pi + 1, high, compare);
        }
    }

    int partition(int low, int high, int (*compare)(const T&, const T&)) {
        T pivot = data[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (compare(data[j], pivot) <= 0) {
                i++;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }
};

#endif
