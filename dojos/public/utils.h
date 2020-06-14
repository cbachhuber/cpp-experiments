#ifndef DOJO_UTILS_H
#define DOJO_UTILS_H

#include <iostream>

template <typename T>
void printContainerElements(const T& container) {
    for (auto& element : container) {
        std::cout << element << ", ";
    }
    std::cout << std::endl;
}

#endif  // DOJO_UTILS_H
