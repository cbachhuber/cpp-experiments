#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// TODO perform time msmt in macro

static constexpr std::size_t kNumberOfElements{static_cast<std::size_t>(1e6)};

// TODO constrain to bidirectional iterators
template <typename T>
void accumulateInArrayUsingTransform(T& array) {
    std::transform(array.begin(), std::prev(array.end()), std::next(array.begin()), std::next(array.begin()),
                   [](int first, int second) { return first + second; });
}

template <typename T>
void accumulateInArrayUsingForLoop(T& array) {
    const auto sz = array.size();
    for (auto index{1U}; index < sz; ++index) {
        array[index] = array[index] + array[index - 1];
    }
}

template <typename T>
void testAccumulate(T& array, const std::string& container_name) {
    std::cout << "Performing tests on " << container_name << "\n";
    auto start = std::chrono::steady_clock::now();
    accumulateInArrayUsingTransform(array);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Transform took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "µs"
              << std::endl;

    start = std::chrono::steady_clock::now();
    accumulateInArrayUsingForLoop(array);
    end = std::chrono::steady_clock::now();
    std::cout << "For loop took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "µs\n\n";
}

template <typename T>
void testAccumulatePtr(T& array_ptr, const std::string& container_name) {
    std::cout << "Performing tests on " << container_name << "\n";
    auto start = std::chrono::steady_clock::now();
    std::transform(array_ptr->begin(), std::prev(array_ptr->end()), std::next(array_ptr->begin()),
                   std::next(array_ptr->begin()), [](int first, int second) { return first + second; });
    auto end = std::chrono::steady_clock::now();
    std::cout << "Transform took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "µs"
              << std::endl;

    start = std::chrono::steady_clock::now();
    const auto sz = array_ptr->size();  // no performance change in release mode, probably optimized away
    for (auto index{1U}; index < sz; ++index) {
        (*array_ptr)[index] = (*array_ptr)[index] + (*array_ptr)[index - 1];
    }
    end = std::chrono::steady_clock::now();
    std::cout << "For loop took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "µs\n\n";
}

int main() {
    // TODO perform multiple measurements
    {
        std::vector<int> to_accumulate(kNumberOfElements);
        std::fill(to_accumulate.begin(), to_accumulate.end(), 1);
        testAccumulate(to_accumulate, "std::vector");
    }
    {
        std::array<int, kNumberOfElements> to_accumulate{};
        std::fill(to_accumulate.begin(), to_accumulate.end(), 1);
        testAccumulate(to_accumulate, "std::array");
    }
    {
        auto to_accumulate{std::make_unique<std::array<int, kNumberOfElements>>()};
        std::fill(to_accumulate->begin(), to_accumulate->end(), 1);
        testAccumulatePtr(to_accumulate, "std::unique_ptr<std::array>");
    }
}
