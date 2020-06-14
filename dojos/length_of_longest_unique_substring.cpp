#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>

int lengthOfLongestSubstring(std::string s) {
    std::unordered_set<char> seen_characters{};
    int maximum_seen_length{0};
    int current_start{0};

    for (int index{0}; index < s.size(); ++index) {
        if (seen_characters.find(s[index]) != seen_characters.end()) {
            while (s[current_start] != s[index]) {
                seen_characters.erase(s[current_start]);
                current_start++;
            }
            current_start++;
        } else {
            seen_characters.insert(s[index]);
        }
        maximum_seen_length = std::max(maximum_seen_length, index - current_start + 1);
    }
    return maximum_seen_length;
}

int main() { std::cout << lengthOfLongestSubstring("pwwkew") << std::endl; }
