#include "ShortList.h"
#include <algorithm> // For std::find
#include <iostream>

ShortList::ShortList(std::mt19937& rng) : size(0), rng(rng) {
    // Initialize containsArr and indexArr
    std::fill(containsArr, containsArr + MAX_SIZE, false);
    std::fill(indexArr, indexArr + MAX_SIZE, -1); // Invalid index
}

bool ShortList::insert(int num) {
    if (size < MAX_SIZE && !contains(num)) {
        arr[size] = num;
        containsArr[num] = true;
        indexArr[num] = size;
        ++size;
        return true;
    }
    return false; // List is full or integer already exists
}

bool ShortList::remove(int num) {
    if (contains(num)) {
        int idx = indexArr[num];
        arr[idx] = arr[--size]; // Replace the element with the last element and decrement size
        containsArr[arr[idx]] = true; // Update containsArr for the moved element
        containsArr[num] = false;
        indexArr[arr[idx]] = idx; // Update indexArr for the moved element
        indexArr[num] = -1; // Invalidate the index of the removed element
        return true;
    }
    return false; // Element not found
}

bool ShortList::contains(int num) const {
    return num >= 0 && num < MAX_SIZE && containsArr[num];
}

int ShortList::getRandomElement() const {
    if (size > 0) {
        return arr[rng() % size];
    }
    return -1; // No elements in the list
}

int ShortList::getSize() const {
    return size;
}

void ShortList::print() const {
    std::cout << "List: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
