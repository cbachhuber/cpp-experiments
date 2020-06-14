#include <deque>
#include <iostream>
#include <string>

std::string longestPalindrome(std::string s) {
    std::string result{};
    for (int index = 0; index < s.size(); ++index) {
        std::deque<char> stack{};
        int last_success = index;
        for (int j = index; j >= 0; --j) {
            stack.push_front(s[j]);
            if ((stack.size() > 1) && (stack.front() == stack.back())) {
                stack.pop_front();
                stack.pop_back();
            }
            if (stack.empty() || (stack.size() < 2 && last_success == index)) {
                last_success = j;
            }
        }

        if (stack.size() < 2) {
            std::string candidate{s.begin() + last_success + 1, s.begin() + index + 1};

            if (result.size() < candidate.size()) {
                result = candidate;
            }
        }
    }
    return result;
}

int main() { std::cout << longestPalindrome("babad") << std::endl; }
