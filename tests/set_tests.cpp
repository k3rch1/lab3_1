#include <gtest/gtest.h>

#include <initializer_list>
#include <string>
#include <utility>

#include "set.hpp"

template<class T>
void expect_set(const set<T>& actual, std::initializer_list<T> expected) {
    ASSERT_EQ(actual.size(), expected.size());

    for (const T& value : expected)
        EXPECT_TRUE(actual.contains(value));
}

TEST(set, default_ctor) {
    set<int> values;

    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.size(), 0);
}

TEST(set, ilist_ctor) {
    set<int> values = {4, 2, 6, 1, 3};

    EXPECT_FALSE(values.empty());
    expect_set(values, {1, 2, 3, 4, 6});
}

TEST(set, ilist_ctor_duplicates) {
    set<int> values = {2, 1, 2, 3, 1};

    EXPECT_EQ(values.size(), 3);
    expect_set(values, {1, 2, 3});
}

TEST(set, copy_ctor) {
    set<int> values = {4, 2, 6, 1, 3};
    set<int> copy(values);

    EXPECT_TRUE(copy == values);
    values.insert(10);
    EXPECT_FALSE(copy.contains(10));
}

TEST(set, move_ctor) {
    set<int> values = {4, 2, 6};
    set<int> moved(std::move(values));

    EXPECT_TRUE(values.empty());
    expect_set(moved, {2, 4, 6});
}

TEST(set, copy_assignment) {
    set<int> values = {4, 2, 6};
    set<int> assigned = {100};
    assigned = values;

    EXPECT_TRUE(assigned == values);
    expect_set(assigned, {2, 4, 6});
}

TEST(set, move_assignment) {
    set<int> values = {4, 2, 6};
    set<int> assigned = {100};
    assigned = std::move(values);

    EXPECT_TRUE(values.empty());
    expect_set(assigned, {2, 4, 6});
}

TEST(set, clear) {
    set<int> values = {1, 2, 3};
    values.clear();

    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.size(), 0);
}

TEST(set, insert_lvalue) {
    set<int> values;
    int value = 10;
    values.insert(value);

    EXPECT_EQ(values.size(), 1);
    EXPECT_TRUE(values.contains(10));
}

TEST(set, insert_rvalue) {
    set<std::string> values;
    values.insert(std::string("hello"));

    EXPECT_EQ(values.size(), 1);
    EXPECT_TRUE(values.contains("hello"));
}

TEST(set, insert_duplicate) {
    set<int> values = {1, 2, 3};
    values.insert(2);

    EXPECT_EQ(values.size(), 3);
    expect_set(values, {1, 2, 3});
}

TEST(set, contains) {
    set<int> values = {4, 2, 6, 1, 3};

    EXPECT_TRUE(values.contains(3));
    EXPECT_FALSE(values.contains(5));
}

TEST(set, remove_existing) {
    set<int> values = {4, 2, 6, 1, 3};

    EXPECT_TRUE(values.remove(2));
    EXPECT_FALSE(values.contains(2));
    expect_set(values, {1, 3, 4, 6});
}

TEST(set, remove_missing) {
    set<int> values = {1, 2, 3};

    EXPECT_FALSE(values.remove(10));
    expect_set(values, {1, 2, 3});
}

TEST(set, union) {
    set<int> first = {1, 2, 3};
    set<int> second = {3, 4, 5};

    set<int> result = first | second;

    expect_set(result, {1, 2, 3, 4, 5});
}

TEST(set, union_with_empty) {
    set<int> first = {1, 2, 3};
    set<int> empty;

    expect_set(first | empty, {1, 2, 3});
    expect_set(empty | first, {1, 2, 3});
}

TEST(set, intersection) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {3, 4, 5, 6};

    set<int> result = first & second;

    expect_set(result, {3, 4});
}

TEST(set, intersection_disjoint) {
    set<int> first = {1, 2};
    set<int> second = {3, 4};

    set<int> result = first & second;

    EXPECT_TRUE(result.empty());
}

TEST(set, difference) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {3, 4, 5};

    set<int> result = first - second;

    expect_set(result, {1, 2});
}

TEST(set, difference_disjoint) {
    set<int> first = {1, 2};
    set<int> second = {3, 4};

    set<int> result = first - second;

    expect_set(result, {1, 2});
}

TEST(set, difference_all_elements) {
    set<int> first = {1, 2, 3};
    set<int> second = {1, 2, 3, 4};

    EXPECT_TRUE((first - second).empty());
}

TEST(set, union_assignment_lvalue) {
    set<int> first = {1, 2, 3};
    set<int> second = {3, 4, 5};

    set<int>& result = (first |= second);

    EXPECT_EQ(&result, &first);
    expect_set(first, {1, 2, 3, 4, 5});
}

TEST(set, union_assignment_rvalue) {
    set<int> first = {1, 2, 3};
    set<int> second = {3, 4, 5};

    first |= std::move(second);

    expect_set(first, {1, 2, 3, 4, 5});
}

TEST(set, intersection_assignment_lvalue) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {3, 4, 5};

    first &= second;

    expect_set(first, {3, 4});
}

TEST(set, intersection_assignment_rvalue) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {3, 4, 5};

    first &= std::move(second);

    expect_set(first, {3, 4});
}

TEST(set, difference_assignment_lvalue) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {3, 4, 5};

    first -= second;

    expect_set(first, {1, 2});
}

TEST(set, difference_assignment_rvalue) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {3, 4, 5};

    first -= std::move(second);

    expect_set(first, {1, 2});
}

TEST(set, subset_true) {
    set<int> first = {1, 2};
    set<int> second = {1, 2, 3};

    EXPECT_TRUE(first <= second);
}

TEST(set, subset_equal_sets) {
    set<int> first = {1, 2, 3};
    set<int> second = {3, 2, 1};

    EXPECT_TRUE(first <= second);
    EXPECT_TRUE(second <= first);
}

TEST(set, subset_false_missing_element) {
    set<int> first = {1, 2, 4};
    set<int> second = {1, 2, 3};

    EXPECT_FALSE(first <= second);
}

TEST(set, subset_false_by_size) {
    set<int> first = {1, 2, 3, 4};
    set<int> second = {1, 2, 3};

    EXPECT_FALSE(first <= second);
}

TEST(set, empty_is_subset) {
    set<int> empty;
    set<int> values = {1, 2, 3};

    EXPECT_TRUE(empty <= values);
}

TEST(set, equality_same_elements_different_order) {
    set<int> first = {4, 2, 6, 1, 3};
    set<int> second = {2, 1, 4, 3, 6};

    EXPECT_TRUE(first == second);
}

TEST(set, equality_different_elements) {
    set<int> first = {1, 2, 3};
    set<int> second = {1, 2, 4};

    EXPECT_FALSE(first == second);
}

TEST(set, equality_empty_sets) {
    set<int> first;
    set<int> second;

    EXPECT_TRUE(first == second);
}
