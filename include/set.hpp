#pragma once
#include "binary_search_tree.hpp"

template <class T>
class set {
private:
    binary_search_tree<T> tree_;

public:
    set() = default;
    set(std::initializer_list<T> init) : tree_(init) {}

    set(const set& other) = default;
    set(set&& other) noexcept = default;

    set& operator=(const set& other) = default;
    set& operator=(set&& other) noexcept = default;

    void clear() noexcept;

    void insert(const T& value);
    void insert(T&& value);

    bool contains(const T& value) const;
    bool remove(const T& value);

    bool empty() const noexcept;
    size_t size() const;

    set operator|(const set& other) const;
    set operator&(const set& other) const;
    set operator-(const set& other) const;

    set& operator|=(const set& other);
    set& operator|=(set&& other);

    set& operator&=(const set& other);
    set& operator&=(set&& other);

    set& operator-=(const set& other);
    set& operator-=(set&& other);

    bool operator<=(const set& other) const;
    bool operator==(const set& other) const;
};

template<class T>
void set<T>::clear() noexcept {
    tree_.clear();
}

template<class T>
void set<T>::insert(const T& value) {
    tree_.insert(value);
}

template<class T>
void set<T>::insert(T&& value) {
    tree_.insert(std::move(value));
}

template<class T>
bool set<T>::contains(const T& value) const {
    return tree_.contains(value);
}

template<class T>
bool set<T>::remove(const T& value) {
    return tree_.remove(value);
}

template<class T>
bool set<T>::empty() const noexcept {
    return tree_.empty();
}

template<class T>
size_t set<T>::size() const {
    return tree_.size();
}

template<class T>
set<T> set<T>::operator|(const set<T>& other) const {
    set res(*this);

    other.tree_.traverse(
        [&res](const T& value) {
            res.insert(value);
        },
        binary_search_tree<T>::traversal_order::KLP
    );

    return res;
}

template<class T>
set<T> set<T>::operator&(const set<T>& other) const {
    set<T> res;
    res.tree_ = tree_.filter(
        [&other](const T& value) {
            return other.contains(value);
        }
    );
    return res;
}

template<class T>
set<T> set<T>::operator-(const set<T>& other) const {
    set res;

    res.tree_ = tree_.filter(
        [&other](const T& value) {
            return !other.contains(value);
        }
    );
    return res;
}

template<class T>
set<T>& set<T>::operator|=(const set<T>& other) {
    other.tree_.traverse(
        [this](const T& value) {
            insert(value);
        },
        binary_search_tree<T>::traversal_order::KLP
    );

    return *this;
}

template<class T>
set<T>& set<T>::operator|=(set<T>&& other) {
    other.tree_.traverse(
        [this](const T& value) {
            insert(value);
        },
        binary_search_tree<T>::traversal_order::KLP
    );

    return *this;
}

template<class T>
set<T>& set<T>::operator&=(const set<T>& other) {
    *this = *this & other;
    return *this;
}

template<class T>
set<T>& set<T>::operator&=(set<T>&& other) {
    *this = *this & other;
    return *this;
}

template<class T>
set<T>& set<T>::operator-=(const set<T>& other) {
    *this = *this - other;
    return *this;
}

template<class T>
set<T>& set<T>::operator-=(set<T>&& other) {
    *this = *this - other;
    return *this;
}

template<class T>
bool set<T>::operator<=(const set<T>& other) const {
    if (size() > other.size()) return false;

    bool result = true;

    tree_.traverse(
        [&](const T& value) {
            if (!other.contains(value)) {
                result = false;
            }
        },
        binary_search_tree<T>::traversal_order::KLP
    );

    return result;
}

template<class T>
bool set<T>::operator==(const set<T>& other) const {
    return size() == other.size() && *this <= other;
}