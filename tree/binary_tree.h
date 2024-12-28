#ifndef ASH_BINARY_TREE_H_INCLUDED
#define ASH_BINARY_TREE_H_INCLUDED

#include "tree.h"
#include <exception>
#include <vector>

namespace ash
{
    template <class T>
    class binary_tree;
}

template <class T>
class ash::binary_tree : public ash::tree
{
    public:
        class node;

    binary_tree() {}
};

template <class T>
class ash::binary_tree<T>::node : public ash::tree<T>::node
{
    public:
        node()
        {
            this->value = T();
        }
        node(T value)
        {
            this->value = value;
        }
        node(T value, const std::vector<node*> &children)
        {
            this->children = children;
            to_binary(this->children);

            this->value = value;
        }
        node(T value, node *parent, const std::vector<node*> &children)
        {
            this->children = children;
            to_binary(this->children);

            this->value = value;
            this->parent = parent;
            
            if (!parent->contains(this))
                throw std::invalid_argument("Node is not the child of the specified parent.");
        }
        node *left()
        {
            return this->children[0];
        }
        const node *left() const
        {
            return this->children[0];
        }
        node *right()
        {
            return this->children[1];
        }
        const node *right() const
        {
            return this->children[1];
        }
        static void to_binary(std::vector<node*> &children)
        {
            if (children.size() > 2)
                throw std::invalid_argument("Children count is more than 2.");
            for (size_t i = children.size(); i < 2; i++)
                children.push_back(nullptr);
        }
};

#endif