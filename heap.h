#ifndef ASH_HEAP_H_INCLUDED
#define ASH_HEAP_H_INCLUDED

#include <vector>
#include <iostream>
#include <functional>

namespace ash
{
    template <class T>
    class heap;
}

template <class T>
class ash::heap
{
    class node;

    private:
        node *root = nullptr;
        std::function<bool (const T &left, const T &right)> comp;
    
    public:
        heap()
        {
            comp = [](const T &left, const T &right) { return left < right; };
        }
        heap(const std::function<bool (const T &left, const T &right)> &comp = [](const T &left, const T &right) { return left < right; })
        {
            root = new node();
            this->comp = comp;
        }
        heap(std::vector<T> &container, const std::function<bool (const T&, const T&)> &comp = [](const T &left, const T &right) { return left < right; })
        {
            this->comp = comp;

            for (T &element : container)
            {
                node *route = root;

                while (route->left_child == nullptr )
                {
                    if (comp(element, route->value()))
                    {
                        if (route->left_child == nullptr)
                            route->left_child = new node(element);

                        else if (route->right_child == nullptr)
                            route->right_child = new node(element);

                        route = route->left_child;
                    }
                    else
                    {
                        T value = route->value();
                        route->value() = element;

                        if (route->left_child == nullptr)
                            route->left_child = new node(value);

                        else if (route->right_child == nullptr)
                            route->right_child = new node(value);

                        route = route->left_child;
                    }
                }
                if (route == nullptr)
                    route = new node(element);
            }
        }

        void print()
        {
            printAll(root);
        }
        
    private:
        void printAll(node *current)
        {
            if (current == nullptr)
            {
                return;
            }

            std::cout << current->value() << " -> left: ";
            if (current->left_child != nullptr)
                std::cout << current->left_child->value();
            else
                std::cout << "null";
            
            std::cout << ", right: ";
            if (current->right_child != nullptr)
                std::cout << current->right_child->value();
            else
                std::cout << "null";

            std::cout << std::endl;

            printAll(current->left_child);
            printAll(current->right_child);
        }
};

template <class T>
class ash::heap<T>::node
{
    public:
        T *referece = nullptr;
        node *parent = nullptr;
        node *left_child = nullptr;
        node *right_child = nullptr;
    
        node()
        {
            referece = new T();
        }
        node(T &value)
        {
            referece = &value;
        }
        node(const node &other)
        {
            referece = new T(other.value());
            parent = other.parent;
            left_child = other.left_child;
            right_child = other.right_child;
        }

        T& value()
        {
            return *referece;
        }
};

#endif