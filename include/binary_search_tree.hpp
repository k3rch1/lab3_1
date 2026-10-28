#pragma once
#include <memory>
#include <initializer_list>
#include <cstddef>

template <class T>
class binary_search_tree {
public:
    enum class traversal_order {
        KLP,
        KPL,
        LPK,
        LKP,
        PKL,
        PLK
    };

private:
    struct node {
        T value_;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;

        explicit node(const T& value) : value_(value) {};
    };

    std::unique_ptr<node> root_;

    template <class F>
    void traverse_impl(const node* cur, F& func, const traversal_order& order) const;

    std::unique_ptr<node> copy_impl(const node* cur);

public:
    binary_search_tree() = default;
    explicit binary_search_tree(const T& value) : root_(std::make_unique<node>(value)) {};
    binary_search_tree(std::initializer_list<T> values);

    binary_search_tree(const binary_search_tree& other) : root_(copy_impl(other.root_.get())) {};
    binary_search_tree(binary_search_tree&& other) noexcept : root_(std::move(other.root_)) {};

    binary_search_tree& operator=(const binary_search_tree& other);
    binary_search_tree& operator=(binary_search_tree&& other);

    bool empty() const noexcept;
    void clear() noexcept;

    void insert(const T& value);
    bool contains(const T& value) const;
    bool remove(const T& value);

    size_t size() const;
    
    template <class F>
    void traverse(F func, traversal_order order) const;
};

template <class T>
binary_search_tree<T>::binary_search_tree(std::initializer_list<T> init) {
    for (const T& value : init) insert(value);
}

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

template <class T>
template <class F>
void binary_search_tree<T>::traverse_impl(const node* cur, F& func, const traversal_order& order) const {
    if (!cur) return;

    switch (order) {
        case traversal_order::KLP:
            func(cur->value_);
            traverse_impl(cur->left.get(), func, order);
            traverse_impl(cur->right.get(), func, order);
            break;

        case traversal_order::KPL:
            func(cur->value_);
            traverse_impl(cur->right.get(), func, order);
            traverse_impl(cur->left.get(), func, order);
            break;

        case traversal_order::LPK:
            traverse_impl(cur->left.get(), func, order);
            traverse_impl(cur->right.get(), func, order);
            func(cur->value_);
            break;

        case traversal_order::LKP:
            traverse_impl(cur->left.get(), func, order);
            func(cur->value_);
            traverse_impl(cur->right.get(), func, order);
            break;

        case traversal_order::PKL:
            traverse_impl(cur->right.get(), func, order);
            func(cur->value_);
            traverse_impl(cur->left.get(), func, order);
            break;

        case traversal_order::PLK:
            traverse_impl(cur->right.get(), func, order);
            traverse_impl(cur->left.get(), func, order);
            func(cur->value_);
            break;
    }
}

template <class T>
template <class F>
void binary_search_tree<T>::traverse(F func, traversal_order order) const {
    traverse_impl(root_.get(), func, order);
}

template <class T>
size_t binary_search_tree<T>::size() const {
    size_t count = 0;

    traverse([&count](const T&) {++count;}, traversal_order::KLP);

    return count;
}

template <class T>
std::unique_ptr<typename binary_search_tree<T>::node>
binary_search_tree<T>::copy_impl(const node* cur) {
    if (!cur) return nullptr;

    auto copy = std::make_unique<node>(cur->value_);

    copy->left = copy_impl(cur->left.get());
    copy->right = copy_impl(cur->right.get());

    return copy;
}

template <class T>
binary_search_tree<T>& binary_search_tree<T>::operator=(const binary_search_tree<T>& other) {
    if (this != &other) {
        root_ = copy_impl(other.root_.get());
    }

    return *this;
}

template <class T>
binary_search_tree<T>& binary_search_tree<T>::operator=(binary_search_tree<T>&& other) noexcept {
    if (this != &other) {
        root_ = std::move(other.root_);
    }

    return *this;
}