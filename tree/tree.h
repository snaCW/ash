#ifndef ASH_TREE_H_INCLUDED
#define ASH_TREE_H_INCLUDED

#include <vector>
#include <iostream>
#include <deque>

namespace ash
{
    template <class T>
    class tree;
}

template <class T>
class ash::tree
{
// Nested classes
    public:
        class node;
        class iterator;

// Typedef
    public:
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;

// protected data members
    protected:
        node *root = nullptr;

// Constructors
    public:
        tree() {}
        tree(const_reference value)
        {
            root = new node(value);
        }
        ~tree()
        {
            delete root;
            // Does this statement delete all the children too?
        }

// Basic Information
        size_t size() const
        {
            return size(root);
        }
        static size_t size(const iterator &current)
        {
            return size(current->target);
        }
        size_t height() const
        {
            return height(root, 0);
        }
        static size_t height(const iterator &current, int depth)
        {
            return height(current->target, 0);
        }
        size_t leaves_count() const
        {
            return leaves_count(root);
        }
        static size_t leaves_count(const iterator &current)
        {
            return leaves_count(current->target);
        }
        T max() const
        {
            return max(root);
        }
        static T max(const iterator &current)
        {
            return max(current->target);
        }

        static size_t size(const node *current)
        {
            int result = 0;
            const std::vector<node*> &children = current->children;
            for (const node *child : children)
            {
                if (child != nullptr)
                    result += size(child);
            }
            return result;
        }
        static size_t height(const node *current, int depth)
        {
            int result = depth;
            const std::vector<node*> &children = current->children;
            for (const node *child : children)
            {
                if (child != nullptr)
                    result = max(result, height(child, depth + 1));
            }
            return result;
        }
        static size_t leaves_count(const node *current)
        {
            int result = 0;
            bool is_leaf = true;
            const std::vector<node*> &children = current->target->children;
            for (const node *child : children)
            {
                if (child != nullptr)
                    is_leaf = false;
                result += leaves_count(child);
            }
            return result + (int)is_leaf;
        }
        static T max(const node *current)
        {
            T result = current->value;
            const std::vector<node*> &children = current->target->children;
            for (const node *child : children)
            {
                if (child != nullptr)
                    if (child->value > result)
                        result = child->value;
            }
            return result;
        }

// Check
        bool same(const tree &other) const
        {
            std::deque<node*> dq1, dq2;
            dq1.push_back(root);
            dq2.push_back(other.root);
            while (!dq1.empty() && !dq2.empty())
            {
                if (dq1.front() != dq2.front())
                    return false;
                for (node *child : dq1.front()->children)
                    dq1.push_back(child);
                for (node *child : dq2.front()->children)
                    dq2.push_back(child);
                dq1.pop_front();
                dq2.pop_front();
            }
            return true;
        }
        bool is_compelete() const
        {
            size_t max = __SIZE_MAX__;
            return __is_compelete__(root, 0, max);
        }
    protected:
        static bool __is_compelete__(const node *current, size_t depth, size_t &max)
        {
            if (current == nullptr)
            {
                if (depth > max)
                    return false;
                max = depth;
                return true;
            }

            bool result = true;
            for (const node *child : current->children)
                result = result && __is_compelete__(child, depth + 1, max);
            return result;
        }
};

template <class T>
class ash::tree<T>::node
{
    friend class ash::tree<T>;
    friend class ash::tree<T>::iterator;

    protected:
        T value;
        node *parent = nullptr;
        std::vector<node*> children;

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
            this->value = value;
        }
        node(T value, node *parent, const std::vector<node*> &children)
        {
            this->children = children;
            this->value = value;
            this->parent = parent;
            
            if (!parent->contains(this))
                throw std::invalid_argument("Node is not the child of the specified parent.");
        }
        node *operator[](size_t index)
        {
            if (index > children.size())
                throw std::invalid_argument("Access to undefined child (out of range)");
            return this->children[index];
        }
        const node *operator[](size_t index) const
        {
            if (index > children.size())
                throw std::invalid_argument("Access to undefined child (out of range)");
            return this->children[index];
        }
        bool contains(const node *child) const
        {
            for (const node *ch : this->children)
                if (ch == child)
                    return true;
            return false;
        }
        static void connect(node *parent, node *child, size_t index)
        {
            parent[index] = child;
            child.parent = parent;
        }
};

template <class T>
class ash::tree<T>::iterator
{
    friend class ash::tree<T>;
    friend class ash::tree<T>::node;

    protected:
        node *target = nullptr;
    
    public:
        iterator() {}
        iterator(node *target)
        {
            this->target = target;
        }
        void add_child(const_reference value)
        {
            node *child(value);
            child->parent = target;
            target->children.push_back(child);
        }
        iterator operator[](size_t index) const
        {
            return iterator(target->children[index]);
        }
        iterator parent() const
        {
            return iterator(target->parent);
        }
        const_reference operator*() const
        {
            return target->value;
        }
        reference operator*()
        {
            return target->value;
        }
        // Pure abstract methods that are only logical for other types of iteration.
        virtual iterator operator++() = 0;
        virtual iterator operator++(int) = 0;
        virtual iterator operator--() = 0;
        virtual iterator operator--(int) = 0;
};

#endif