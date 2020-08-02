#include <cassert>
#include <functional>
#include <utility>

int subtract(int a, int b) { return a - b; }

int main() {
    const auto inverted_subtract_bind = std::bind(subtract, std::placeholders::_2, std::placeholders::_1);
    const auto inverted_subtract_lambda = [](auto&& _1, auto&& _2) {
        return subtract(std::forward<decltype(_2)>(_2), std::forward<decltype(_1)>(_1));
    };

    assert(inverted_subtract_bind(1, 2) == inverted_subtract_lambda(1, 2));

    // `clang-tidy std_bind_vs_lambda.cpp --checks='-*,modernize-avoid-bind'` suggests
    //    ```cpp
    //    const auto inverted_subtract_bind = [] {
    //        return subtract(std::placeholders::_2, std::placeholders::_1);
    //    };
    //    ```
    // which is not correct. Enabling option 'PermissiveParameterList' in .clang-tidy does not help:
    //    ```cpp
    //    const auto inverted_subtract_bind = [](auto && ...) {
    //        return subtract(std::placeholders::_2, std::placeholders::_1);
    //    };
    //    ```
    // clang-tidy should suggest something similar to the implementation of `inverted_subtract_lambda` above.
}