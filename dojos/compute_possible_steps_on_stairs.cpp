#include <iostream>

int computePossibleStepsOnStairs(int remaining_steps) {
    if (remaining_steps == 3) {
        return 4;
    }
    if (remaining_steps == 2) {
        return 2;
    }
    if (remaining_steps == 1) {
        return 1;
    }
    if (remaining_steps == 0) {
        return 0;
    }
    return computePossibleStepsOnStairs(remaining_steps - 3) + computePossibleStepsOnStairs(remaining_steps - 2) +
           computePossibleStepsOnStairs(remaining_steps - 1);
}

int main() { std::cout << computePossibleStepsOnStairs(10) << std::endl; }
