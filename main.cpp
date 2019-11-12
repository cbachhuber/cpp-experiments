#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

static constexpr std::size_t kNumberOfElements{static_cast<std::size_t>(1e6)};

int main() {
    // std::vector<int> to_accumulate(kNumberOfElements);
    std::array<int, kNumberOfElements> to_accumulate{};
    std::fill(to_accumulate.begin(), to_accumulate.end(), 1);

    auto start = std::chrono::steady_clock::now();
    std::transform(to_accumulate.begin(), std::prev(to_accumulate.end()), std::next(to_accumulate.begin()),
                   std::next(to_accumulate.begin()), [](int first, int second) { return first + second; });
    auto end = std::chrono::steady_clock::now();
    std::cout << "Transform took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms"
              << std::endl;

    start = std::chrono::steady_clock::now();
    for (auto index{1U}; index < to_accumulate.size(); ++index) {
        to_accumulate[index] = to_accumulate[index] + to_accumulate[index - 1];
    }
    end = std::chrono::steady_clock::now();
    std::cout << "For loop took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms"
              << std::endl;

    auto to_accumulate_unique_ptr{std::make_unique<std::array<int, kNumberOfElements>>()};
    std::fill(to_accumulate_unique_ptr->begin(), to_accumulate_unique_ptr->end(), 1);

    start = std::chrono::steady_clock::now();
    std::transform(to_accumulate_unique_ptr->begin(), std::prev(to_accumulate_unique_ptr->end()),
                   std::next(to_accumulate_unique_ptr->begin()), std::next(to_accumulate_unique_ptr->begin()),
                   [](int first, int second) { return first + second; });
    end = std::chrono::steady_clock::now();
    std::cout << "Transform took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms"
              << std::endl;

    start = std::chrono::steady_clock::now();
    for (auto index{1U}; index < to_accumulate_unique_ptr->size(); ++index) {
        (*to_accumulate_unique_ptr)[index] =
            (*to_accumulate_unique_ptr)[index] + (*to_accumulate_unique_ptr)[index - 1];
    }
    end = std::chrono::steady_clock::now();
    std::cout << "For loop took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms"
              << std::endl;
}
