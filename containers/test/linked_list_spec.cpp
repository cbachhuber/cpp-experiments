#include <gtest/gtest.h>

#include <linked_list.h>

TEST(LinkedListElement, ShouldAssignConstructorArgumentsCorrectly) {
    const std::int32_t expected{5};
    const LinkedListElement<std::int32_t> unit{expected};
    EXPECT_EQ(expected, unit.value);
    EXPECT_EQ(nullptr, unit.next);
}

TEST(LinkedList, ShouldConstructWithNoElements) {
    const LinkedList<std::int32_t> unit{};
    EXPECT_EQ(0U, unit.size());
    EXPECT_TRUE(unit.empty());
    EXPECT_FALSE(unit.front());
}

TEST(LinkedList, ShouldEmplaceAtFront) {
    LinkedList<std::int32_t> unit{};

    unit.emplace_front(4);
    ASSERT_TRUE(unit.front());
    EXPECT_EQ(4, unit.front().value());

    unit.emplace_front(5);
    ASSERT_TRUE(unit.front());
    EXPECT_EQ(2, unit.size());
    EXPECT_EQ(5, unit.front().value());
}

TEST(LinkedList, ShouldPopFirstItem){
    LinkedList<std::int32_t> unit{};
    unit.emplace_front(4);
    unit.emplace_front(5);

    EXPECT_EQ(5, unit.pop_front());
    EXPECT_EQ(1U, unit.size());

    EXPECT_EQ(4, unit.pop_front());
    EXPECT_EQ(0U, unit.size());
    EXPECT_TRUE(unit.empty());
    EXPECT_FALSE(unit.front());
}