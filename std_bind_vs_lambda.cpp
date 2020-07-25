#include <functional>

int subtract(int a, int b) { return a - b; }

int main() {
    const auto inverted_subtract_bind = std::bind(subtract, std::placeholders::_2, std::placeholders::_1);
    const auto inverted_subtract_lambda = [](auto _1, auto _2) { return subtract(_2, _1); };

    assert(inverted_subtract_bind(1, 2) == inverted_subtract_lambda(1, 2));

    // Issue: `clang-tidy std_bind_vs_lambda.cpp` suggests
    //    ```cpp
    //    const auto inverted_subtract_bind = [] {
    //        return subtract(std::placeholders::_2, std::placeholders::_1);
    //    };
    //    ```
    // which is not correct. It should suggest the implementation of `inverted_subtract_lambda` above
}