#include <gtest/gtest.h>

#include <string>
#include <utility>

#include "binary_search_tree.hpp"
#include "array_sequence.hpp"

using traversal_order = binary_search_tree<int>::traversal_order;

template<class T>
array_sequence<T> get_values(
    const binary_search_tree<T>& tree,
    typename binary_search_tree<T>::traversal_order order
) {
    array_sequence<T> values;
    tree.traverse([&values](const T& value) {
        values.append(value);
    }, order);
    return values;
}

template<class T>
void expect_sequence(const array_sequence<T>& actual, const array_sequence<T>& expected) {
    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i)
        EXPECT_EQ(actual[i], expected[i]);
}

TEST(binary_search_tree, default_ctor) {
    binary_search_tree<int> tree;

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(binary_search_tree, value_ctor) {
    binary_search_tree<int> tree(10);

    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contains(10));
}

TEST(binary_search_tree, ilist_ctor) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};

    EXPECT_EQ(tree.size(), 5);
    EXPECT_TRUE(tree.contains(1));
    EXPECT_TRUE(tree.contains(6));
}

TEST(binary_search_tree, ilist_ctor_duplicates) {
    binary_search_tree<int> tree = {2, 1, 2, 3, 1};

    EXPECT_EQ(tree.size(), 3);
}

TEST(binary_search_tree, copy_ctor) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};
    binary_search_tree<int> copy(tree);

    EXPECT_TRUE(copy == tree);
    EXPECT_EQ(copy.size(), tree.size());
}

TEST(binary_search_tree, move_ctor) {
    binary_search_tree<int> tree = {4, 2, 6};
    binary_search_tree<int> moved(std::move(tree));

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.contains(2));
}

TEST(binary_search_tree, copy_assignment) {
    binary_search_tree<int> tree = {4, 2, 6};
    binary_search_tree<int> assigned = {100};
    assigned = tree;

    EXPECT_TRUE(assigned == tree);
    EXPECT_EQ(assigned.size(), 3);
}

TEST(binary_search_tree, move_assignment) {
    binary_search_tree<int> tree = {4, 2, 6};
    binary_search_tree<int> assigned = {100};
    assigned = std::move(tree);

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(assigned.size(), 3);
    EXPECT_TRUE(assigned.contains(6));
}

TEST(binary_search_tree, equality_same_structure) {
    binary_search_tree<int> first = {4, 2, 6, 1, 3};
    binary_search_tree<int> second = {4, 2, 6, 1, 3};

    EXPECT_TRUE(first == second);
}

TEST(binary_search_tree, equality_different_structure) {
    binary_search_tree<int> first = {4, 2, 6};
    binary_search_tree<int> second = {2, 4, 6};

    EXPECT_FALSE(first == second);
}

TEST(binary_search_tree, equality_empty_trees) {
    binary_search_tree<int> first;
    binary_search_tree<int> second;

    EXPECT_TRUE(first == second);
}

TEST(binary_search_tree, clear) {
    binary_search_tree<int> tree = {4, 2, 6};
    tree.clear();

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(binary_search_tree, insert) {
    binary_search_tree<int> tree;
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contains(1));
    EXPECT_TRUE(tree.contains(2));
    EXPECT_TRUE(tree.contains(3));
}

TEST(binary_search_tree, insert_duplicate) {
    binary_search_tree<int> tree = {2, 1, 3};
    tree.insert(2);

    EXPECT_EQ(tree.size(), 3);
}

TEST(binary_search_tree, contains) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};

    EXPECT_TRUE(tree.contains(3));
    EXPECT_FALSE(tree.contains(5));
}

TEST(binary_search_tree, remove_leaf) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};

    EXPECT_TRUE(tree.remove(1));
    EXPECT_FALSE(tree.contains(1));
    EXPECT_EQ(tree.size(), 4);
}

TEST(binary_search_tree, remove_node_with_one_child) {
    binary_search_tree<int> tree = {4, 2, 6, 5};

    EXPECT_TRUE(tree.remove(6));
    EXPECT_FALSE(tree.contains(6));
    EXPECT_TRUE(tree.contains(5));
    EXPECT_EQ(tree.size(), 3);
}

TEST(binary_search_tree, remove_node_with_two_children) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};

    EXPECT_TRUE(tree.remove(4));
    EXPECT_FALSE(tree.contains(4));
    EXPECT_EQ(tree.size(), 6);
    EXPECT_TRUE(tree.contains(1));
    EXPECT_TRUE(tree.contains(7));
}

TEST(binary_search_tree, remove_root_only_node) {
    binary_search_tree<int> tree(4);

    EXPECT_TRUE(tree.remove(4));
    EXPECT_TRUE(tree.empty());
}

TEST(binary_search_tree, remove_missing_value) {
    binary_search_tree<int> tree = {4, 2, 6};

    EXPECT_FALSE(tree.remove(10));
    EXPECT_EQ(tree.size(), 3);
}

TEST(binary_search_tree, traverse_klp) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    array_sequence<int> expected = {4, 2, 1, 3, 6, 5, 7};

    expect_sequence(get_values(tree, traversal_order::KLP), expected);
}

TEST(binary_search_tree, traverse_kpl) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    array_sequence<int> expected = {4, 6, 7, 5, 2, 3, 1};

    expect_sequence(get_values(tree, traversal_order::KPL), expected);
}

TEST(binary_search_tree, traverse_lpk) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    array_sequence<int> expected = {1, 3, 2, 5, 7, 6, 4};

    expect_sequence(get_values(tree, traversal_order::LPK), expected);
}

TEST(binary_search_tree, traverse_lkp) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    array_sequence<int> expected = {1, 2, 3, 4, 5, 6, 7};

    expect_sequence(get_values(tree, traversal_order::LKP), expected);
}

TEST(binary_search_tree, traverse_pkl) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    array_sequence<int> expected = {7, 6, 5, 4, 3, 2, 1};

    expect_sequence(get_values(tree, traversal_order::PKL), expected);
}

TEST(binary_search_tree, traverse_plk) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    array_sequence<int> expected = {7, 5, 6, 3, 1, 2, 4};

    expect_sequence(get_values(tree, traversal_order::PLK), expected);
}

TEST(binary_search_tree, traverse_empty) {
    binary_search_tree<int> tree;
    size_t count = 0;

    tree.traverse([&count](const int&) { ++count; }, traversal_order::KLP);

    EXPECT_EQ(count, 0);
}

TEST(binary_search_tree, map) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};
    auto mapped = tree.map([](int value) {
        return value * 2;
    });
    binary_search_tree<int> expected = {8, 4, 12, 2, 6};

    EXPECT_TRUE(mapped == expected);
}

TEST(binary_search_tree, map_type_change) {
    binary_search_tree<int> tree = {1, 2, 3};
    auto mapped = tree.map([](int value) {
        return std::to_string(value);
    });
    binary_search_tree<std::string> expected = {"1", "2", "3"};

    EXPECT_TRUE(mapped == expected);
}

TEST(binary_search_tree, filter) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    auto filtered = tree.filter([](int value) {
        return value % 2 == 0;
    });
    binary_search_tree<int> expected = {4, 2, 6};

    EXPECT_TRUE(filtered == expected);
}

TEST(binary_search_tree, filter_empty_result) {
    binary_search_tree<int> tree = {1, 3, 5};
    auto filtered = tree.filter([](int value) {
        return value % 2 == 0;
    });

    EXPECT_TRUE(filtered.empty());
}

TEST(binary_search_tree, reduce) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};
    auto result = tree.reduce([](int acc, int value) {
        return acc + value;
    }, 0, traversal_order::KLP);

    EXPECT_EQ(result, 16);
}

TEST(binary_search_tree, reduce_order) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3};
    auto result = tree.reduce([](std::string acc, int value) {
        return acc + std::to_string(value);
    }, std::string{}, traversal_order::LKP);

    EXPECT_EQ(result, "12346");
}

TEST(binary_search_tree, extract_subtree) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    auto extracted = tree.extract_subtree(2);
    binary_search_tree<int> expected = {2, 1, 3};

    EXPECT_TRUE(extracted == expected);
}

TEST(binary_search_tree, extract_subtree_missing_value) {
    binary_search_tree<int> tree = {4, 2, 6};
    auto extracted = tree.extract_subtree(10);

    EXPECT_TRUE(extracted.empty());
}

TEST(binary_search_tree, contains_subtree) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    binary_search_tree<int> subtree = {2, 1, 3};

    EXPECT_TRUE(tree.contains_subtree(subtree));
}

TEST(binary_search_tree, does_not_contain_subtree) {
    binary_search_tree<int> tree = {4, 2, 6, 1, 3, 5, 7};
    binary_search_tree<int> subtree = {2, 3, 1, 5};

    EXPECT_FALSE(tree.contains_subtree(subtree));
}

TEST(binary_search_tree, contains_empty_subtree) {
    binary_search_tree<int> tree = {4, 2, 6};
    binary_search_tree<int> empty_tree;

    EXPECT_TRUE(tree.contains_subtree(empty_tree));
}

TEST(binary_search_tree, balance) {
    binary_search_tree<int> tree = {1, 2, 3, 4, 5, 6, 7};
    tree.balance();
    binary_search_tree<int> expected = {4, 2, 6, 1, 3, 5, 7};

    EXPECT_TRUE(tree == expected);
}

TEST(binary_search_tree, balance_empty) {
    binary_search_tree<int> tree;
    tree.balance();

    EXPECT_TRUE(tree.empty());
}
