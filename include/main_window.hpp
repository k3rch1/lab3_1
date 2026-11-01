#pragma once

#include <QHBoxLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QWidget>

#include "array_sequence.hpp"
#include "set.hpp"
#include "binary_search_tree.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    array_sequence<binary_search_tree<int>*> trees;
    array_sequence<set<int>*> set;

    QListWidget* tree_list;
    QListWidget* set_list;

    QWidget* workspace;
    QHBoxLayout* elements_layout;

    QWidget* tree_controls;
    QWidget* tree_element_controls;

    QWidget* set_controls;
    QWidget* set_element_controls;

    QLineEdit* input;
    int selected_element = 0;
    bool has_selected_element = false;

    enum class selected_type {
        none,
        tree,
        set
    };

    selected_type current_type = selected_type::none;

    void create_tree();
    void create_set();

    void redraw_trees();
    void redraw_sets();
    void redraw_workspace();
};