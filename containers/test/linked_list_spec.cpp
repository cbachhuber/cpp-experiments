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
}

TEST(LinkedListFront, ShouldReturnReference) {
    LinkedList<std::int32_t> unit{};

    unit.emplace_front(4);
    EXPECT_EQ(4, unit.front());
    EXPECT_EQ(1U, unit.size());

    unit.front() = 5;
    EXPECT_EQ(5, unit.front());
    EXPECT_EQ(1U, unit.size());
}

TEST(LinkedList, ShouldEmplaceAtFront) {
    LinkedList<std::int32_t> unit{};

    unit.emplace_front(4);
    EXPECT_EQ(4, unit.front());

    unit.emplace_front(5);
    EXPECT_EQ(2, unit.size());
    EXPECT_EQ(5, unit.front());
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
}

TEST(LinkedList, ShouldUseInitializerListInGivenOrder){
    LinkedList<std::int32_t> unit{{1,2}};
    EXPECT_EQ(1, unit.pop_front());
    EXPECT_EQ(2, unit.pop_front());
}

TEST(LinkedList, ShouldProvideConstAccessor){
    const LinkedList<std::int32_t> unit{{1,2}};
    EXPECT_EQ(1, unit.front());
}