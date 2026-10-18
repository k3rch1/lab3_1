#pragma once
#include <memory>
#include <initializer_list>
#include <cstddef>

template <class T>
class binary_search_tree {
private:
    struct node {
        T value_;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;

        explicit node(const T& value) : value_(value) {}
    };

    std::unique_ptr<node> root_;

public:
    binary_search_tree() = default;
    explicit binary_search_tree(const T& value) : root_(std::make_unique<node>(value)) {};
    binary_search_tree(std::initializer_list<T> values);

    binary_search_tree(const binary_search_tree& other) : root_(std::move(other.root_)) {};
    binary_search_tree(binary_search_tree&& other) noexcept;

    bool empty() const noexcept;
    void clear() noexcept;

    void insert(const T& value);
    bool contains(const T& value) const;
    bool remove(const T& value);

    size_t size();
};

template <class T>
binary_search_tree<T>::binary_search_tree(std::initializer_list<T> init) {
    for (const T& value : init) insert(value);
}