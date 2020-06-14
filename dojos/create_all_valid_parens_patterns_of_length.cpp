#include <string>
#include <vector>

#include <utils.h>

void appendParens(std::vector<std::string>& existing_combinations, int parens_to_open, int parens_to_close = 0) {
    if (parens_to_open == 0 && parens_to_close > 0) {
        for (size_t i{0U}; i < parens_to_close; ++i) {
            for (auto& s : existing_combinations) {
                s.append(")");
            }
        }
        return;
    }

    if (existing_combinations.empty()) {
        existing_combinations.emplace_back("(");
        appendParens(existing_combinations, parens_to_open - 1, parens_to_close + 1);
    } else {
        if (parens_to_open > 0 && parens_to_close > 0) {
            auto new_opening_bracket{existing_combinations};
            for (auto& s : new_opening_bracket) {
                s.append("(");
            }
            appendParens(new_opening_bracket, parens_to_open - 1, parens_to_close + 1);

            auto new_closing_bracket{existing_combinations};
            for (auto& s : new_closing_bracket) {
                s.append(")");
            }
            appendParens(new_closing_bracket, parens_to_open, parens_to_close - 1);

            existing_combinations.clear();
            existing_combinations.insert(existing_combinations.end(), new_opening_bracket.begin(),
                                         new_opening_bracket.end());
            existing_combinations.insert(existing_combinations.end(), new_closing_bracket.begin(),
                                         new_closing_bracket.end());
            return;
        }
        if (parens_to_open > 0 && parens_to_close == 0) {
            for (auto& s : existing_combinations) {
                s.append("(");
            }
            appendParens(existing_combinations, parens_to_open - 1, parens_to_close + 1);
            return;
        }
    }
}

int main() {
    std::vector<std::string> brackets{};
    appendParens(brackets, 3);
    printContainerElements(brackets);
}
