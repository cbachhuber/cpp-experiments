#include <vector>

#include <utils.h>

std::vector<int> mergeSortedVectorsIntoNewSortedVectors(const std::vector<int>& src1, const std::vector<int>& src2) {
    std::vector<int> result{};
    std::size_t i{0U}, j{0U};
    while (i < src1.size() && j < src2.size()) {
        if (src1[i] <= src2[j]) {
            result.emplace_back(src1[i++]);
        } else {
            result.emplace_back(src2[j++]);
        }
    }
    if (i < src1.size()) {
        result.insert(result.end(), std::next(src1.begin(), i), src1.end());
    }
    if (j < src2.size()) {
        result.insert(result.end(), std::next(src2.begin(), j), src2.end());
    }
    return result;
}

int main() {
    std::vector<int> src1{1, 4, 6, 8, 23};
    std::vector<int> src2{2, 3, 5, 7, 10, 15};
    const auto merged_vector{mergeSortedVectorsIntoNewSortedVectors(src1, src2)};
    printContainerElements(merged_vector);
}
