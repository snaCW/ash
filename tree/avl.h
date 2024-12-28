#ifndef ASH_AVL_H_INCLUDED
#define ASH_AVL_H_INCLUDED

#include "tree.h"
#include <exception>
#include <string>
#include <vector>
#include <utility>

namespace ash
{
    template <class T>
    class avl;
}

template <class T>
class ash::avl
{
// Typedef
    public:
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;

    private:
        class node;
        node *root = nullptr;
    
    public:
        class iterator;
    
// Constructors
    public:
        avl() {}
        avl(const_reference value)
        {
            root = new node(value);
        }
        ~avl()
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
    public:
        bool same(const avl &other)
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
        bool is_compelete()
        {
            size_t max = __SIZE_MAX__;
            return __is_compelete__(root, 0, max);
        }
    protected:
        bool __is_compelete__(node *current, size_t depth, size_t &max)
        {
            if (current == nullptr)
            {
                if (depth > max)
                    return false;
                max = depth;
                return true;
            }

            bool result = true;
            for (node *child : current->children)
                result = result && __is_compelete__(child, depth + 1, max);
            return result;
        }

    public:
        void insert(const_reference value, std::string hint = "0") // 0 is right, 1 is left
        {
            if (!is_binary(hint))
                throw std::invalid_argument("Insertion hint is invalid.");
            
            std::pair<node*, int> parent = hint_node_parent(hint);
            node *target = parent->first->children[parent->second];
            connect(parent->first, target);
            balance_update_to_up(target);
        }
        void print()
        {
            std::vector<bool> rules;
            dfs_print(root, 0, true, false, rules);
        }
    
    private:
        std::pair<node*, int> hint_node_parent(const std::string &hint)
        {
            node *temp = root;
            int index = 0;
            while (true)
            {
                int next_child = hint[index] - '0';
                if (temp->children[next_child] == nullptr)
                    return std::make_pair(temp, next_child);
                
                temp = temp->children[next_child];
                index = (index + 1) % hint.size();
            }
            return temp;
        }
        bool is_binary(const std::string &str)
        {
            for (char c : str)
                if (c != '1' && c != '0')
                    return false;
            return true;
        }

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
            
            dfs_print(current->left(), depth + 1, true, current->right() == nullptr, rules);
            dfs_print(current->right(), depth + 1, false, true, rules);
        }
};

template <class T>
class ash::avl<T>::node : public ash::tree<T>::node
{
    friend class ash::avl<T>;

    private:
        int balance;
    
    public:
        node()
        {
            this->value = T();
            balance = 0;
            this->children = std::vector<node*>(2, nullptr);
        }
        node(const_reference value)
        {
            this->value = value;
            balance = 0;
            this->children = std::vector<node*>(2, nullptr);
        }
        node(const_reference value, const std::vector<node*> &children)
        {
            if (children.size() > 2)
                throw std::invalid_argument("Children count is more than 2.");
            
            this->children = children;
            to_binary(this->children);
            
            this->value = value;
            balance_update();
        }

        void balance_update()
        {
            balance = height(this->children[0]) - height(this->children[1]);
        }
        node *left()
        {
            return this->children[0];
        }
        node *right()
        {
            return this->children[1];
        }
        static void balance_update_to_up(node *current)
        {
            while (current->parent != nullptr)
            {
                current->balance_update();
                current = current->parent;
            }
        }
    
    private:
        void to_binary(std::vector<node*> &children)
        {
            for (size_t i = children.size(); i < 2; i++)
                children.push_back(nullptr);
        }
};

template <class T>
class ash::avl<T>::iterator : public ash::tree<T>::iterator
{

};

#endif