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

        explicit node(const T& value) : value_(value) {};
    };

    std::unique_ptr<node> root_;

public:
    binary_search_tree() = default;
    explicit binary_search_tree(const T& value) : root_(std::make_unique<node>(value)) {};
    binary_search_tree(std::initializer_list<T> values);

    binary_search_tree(const binary_search_tree& other);
    //binary_search_tree(binary_search_tree&& other) noexcept : root_(std::move(other.root_)) {};

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

/*template <class T>
binary_search_tree<T>::binary_search_tree(const binary_search_tree& other) {
    // нужны обходы
}*/

template <class T>
bool binary_search_tree<T>::empty() const noexcept {
    return root_ == nullptr;
}

template <class T>
void binary_search_tree<T>::clear() noexcept {
    root_.reset();
}

template <class T>
void binary_search_tree<T>::insert(const T& value) {
    if (!root_) {
        root_ = std::make_unique<node>(value);
        return;
    }

    node* cur = root_.get();
    while (true) {
        if (cur->value_ < value) {
            if (cur->right) cur = cur->right.get();
            else {
                cur->right = std::make_unique<node>(value);
                break;
            }
            
        }
        else if (value < cur->value_) {
            if (cur->left) cur = cur->left.get();
            else {
                cur->left = std::make_unique<node>(value);
                break;
            }
        } else break;
    }
}

template <class T>
bool binary_search_tree<T>::contains(const T& value) const {
    if (!root_) {
        return false;
    }

    node* cur = root_.get();
    while (true) {
        if (cur->value_ < value) {
            if (cur->right) cur = cur->right.get();
            else {
                return false;
            }
            
        }
        else if (value < cur->value_) {
            if (cur->left) cur = cur->left.get();
            else {
                return false;
            }
        } else return true;
    }
}

template <class T>
bool binary_search_tree<T>::remove(const T& value) {
    std::unique_ptr<node>* cur = &root_;

    while (*cur) {
        if (value < (*cur)->value_) cur = &(*cur)->left;
        else if ((*cur)->value_ < value) cur = &(*cur)->right;
        else {
            if (!(*cur)->left) *cur = std::move((*cur)->right);
            else if (!(*cur)->right) *cur = std::move((*cur)->left);
            else {
                node* min_node = (*cur)->right.get();
                while (min_node->left) min_node = min_node->left.get();
                (*cur)->value_ = min_node->value_;

                std::unique_ptr<node>* min_ptr = &(*cur)->right;
                while ((*min_ptr)->left) min_ptr = &(*min_ptr)->left;
                *min_ptr = std::move((*min_ptr)->right);
            }

            return true;
        }
    }

    return false;
}