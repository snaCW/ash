#ifndef ASH_BST_H_INCLUDED
#define ASH_BST_H_INCLUDED

#include <iostream>
#include <vector>
#include <stdexcept>
#include "../stack.h"
#include <climits>

namespace ash
{
    template <class T>
    class bst;
}

template <class T>
class ash::bst
{
// Nested Classes
private:
    class node;

public:
    class iterator;

// Typedef
public:
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;

// Private data members
private:
    node *root;

// Constructors
public:
    bst()
    {
        root = nullptr;
    }
    bst(const_reference value)
    {
        root = new node(value);
    }
    bst(const bst &other)
    {

    }

// Element Access
    const_reference head()
    {
        return root->value;
    }
    void print()
    {
        std::vector<bool> rules;
        dfs_print(root, 0, true, false, rules);
    }
    iterator find(const_reference value)
    {
        
    }

// Modifiers
    void clear()
    {
        if (root != nullptr)
            ~root;
    }
    void insert(const_reference value)
    {
        node *parent = nullptr;
        node *current = root;

        while (current != nullptr)
        {
            parent = current;
            if (value > current->value)
                current = current->right_child;
            
            else if (value < current->value)
                current = current->left_child;
            
            else
                return;
        }
        
        if (parent == nullptr)
        {
            root = new node(value);
            return;
        }
        
        if (value > parent->value)
            parent->right_child = new node(value);
        else
            parent->left_child = new node(value);
    }
    void erase(const iterator &it)
    {
        erase(*it);
    }
    void erase(const_reference value)
    {
        node *parent = nullptr;
        node *current = root;

        while (current != nullptr)
        {
            if (value == current->value)
                break;

            parent = current;
            if (value > current->value)
                current = current->right_child;
            
            else if (value < current->value)
                current = current->left_child;
            
            else
                return;
        }
        
        if (parent == nullptr) // Root is the value to erase
        {
            delete root;
            root = nullptr;
            return;
        }
        if (current == nullptr) // Value not found
        {
            return;
        }
        if (current->left_child == nullptr && current->right_child == nullptr)
        {
            if (current->value > parent->value)
            {
                delete parent->right_child;
                parent->right_child = nullptr;
            }
            else
            {
                delete parent->left_child;
                parent->left_child = nullptr;
            }
            return;
        }
        
        // Finding the lowest value in the right sub-tree
        if (current->right_child != nullptr)
        {
            parent = current;
            node *target = parent->right_child;
            if (target->left_child != nullptr)
            {
                parent = parent->right_child;
                target = target->left_child;
            }
            else
            {
                current->value = target->value;
                delete parent->right_child;
                parent->right_child = nullptr;
                return;
            }

            while (target->left_child != nullptr)
            {
                parent = parent->left_child;
                target = target->left_child;
            }

            int value_to_replace = target->value;
            delete parent->left_child;
            parent->left_child = nullptr;
            current->value = value_to_replace;
        }
        else // Find the biggest value in the left sub-tree
        {
            parent = current;
            node *target = parent->left_child;
            if (target->right_child != nullptr)
            {
                parent = parent->left_child;
                target = target->right_child;
            }
            else
            {
                current->value = target->value;
                delete parent->left_child;
                parent->left_child = nullptr;
                return;
            }

            while (target->right_child != nullptr)
            {
                parent = parent->right_child;
                target = target->right_child;
            }

            int value_to_replace = target->value;
            delete parent->right_child;
            parent->right_child = nullptr;
            current->value = value_to_replace;
        }
    }

// Iterators
    

// Private methods
private:
    void dfs_print(node *current, int depth, bool left_child, bool is_last_child, std::vector<bool> &rules)
    {
        if (current == nullptr)
            return;

        for (int i = 0; i < depth - 1; i++)
            if (rules[i])
                std::cout << "│  ";
            else
                std::cout << "   ";
        
        if (depth != 0)
        {
            if (!is_last_child)
                std::cout << "├";
            else
                std::cout << "└";

            if (left_child)
                std::cout << "L─";
            else
                std::cout << "R─";
        }
        std::cout << current->value << std::endl;

        if (rules.size() <= depth)
            rules.resize(depth * 2 + 1);
        rules[depth] = true;
        if (is_last_child)
            rules[depth - 1] = false;
        
        dfs_print(current->left_child, depth + 1, true, current->right_child == nullptr, rules);
        dfs_print(current->right_child, depth + 1, false, true, rules);
    }
};

template <class T>
class ash::bst<T>::node
{
public:
// Data members
    T value;
    node *left_child;
    node *right_child;

// Constructors
    node()
    {
        value = T();
        left_child = nullptr;
        right_child = nullptr;
    }
    node(const_reference value)
    {
        this->value = value;
        left_child = nullptr;
        right_child = nullptr;
    }
    node(const_reference value, node *left_child, node *right_child)
    {
        this->value = value;
        this->left_child = left_child;
        this->right_child = right_child;
    }
    ~node()
    {
        delete left_child;
        delete right_child;
    }
};

template <class T>
class ash::bst<T>::iterator
{
private:
    const node *target;

public:
// Constructors
    iterator()
    {
        target = nullptr;
    }
    iterator(const node *target)
    {
        this->target = target;
    }

// Element Access
    const_reference operator*()
    {
        return target->value;
    }
    iterator left()
    {
        return iterator(target->left_child);
    }
    iterator right()
    {
        return iterator(target->right_child);
    }
    iterator operator[](int child_index)
    {
        if (child_index == 0)
            return iterator(target->left_child);
        else if (child_index == 1)
            return iterator(target->right_child);
        else
            throw std::invalid_argument("Unexpeced index");
    }
    int child_count()
    {
        return (int)(target->left_child != nullptr) + (int)(target->right_child != nullptr);
    }
    iterator first_child()
    {
        if (target->left_child == nullptr)
            return iterator(target->right_child);
        return iterator(target->left_child);
    }
    iterator last_child()
    {
        if (target->right_child == nullptr)
            return iterator(target->left_child);
        return iterator(target->right_child);
    }
    int max_height()
    {
        int result = 0;
        dfs_max_height(target, 0, result);
        return result;
    }
    int min_height()
    {
        int result = 0;
        dfs_min_height(target, INT_MAX, result);
        return result;
    }

private:
    void dfs_max_height(node *current, int depth, int &result)
    {
        if (current == nullptr)
        {
            result = std::max(depth, result);
            return;
        }

        dfs_max_height(current->left_child, depth + 1, result);
        dfs_max_height(current->right_child, depth + 1, result);
    }
    void dfs_min_height(node *current, int depth, int &result)
    {
        if (current == nullptr)
        {
            result = std::min(depth, result);
            return;
        }

        dfs_min_height(current->left_child, depth + 1, result);
        dfs_min_height(current->right_child, depth + 1, result);
    }
};

#endif