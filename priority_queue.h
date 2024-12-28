#ifndef ASH_PRIORITY_QUEUE_H_INCLUDED
#define ASH_PRIORITY_QUEUE_H_INCLUDED

#include <map>
#include "queue.h"

namespace ash
{
    template <class T>
    class priority_queue;
}

template <class T>
class ash::priority_queue
{
private:
    std::map<int, queue<T>*> *container = nullptr;
    size_t capacityPerPriority;
    size_t totalElements;

public:
// Constructors
    priority_queue()
    {
        container = new std::map<int, *queue<T>>();
        capacityPerPriority = 1;
        totalElements = 0;
    }
    priority_queue(size_t capacityPerPriority)
    {
        container = new std::map<int, *queue<T>>();
        this->capacityPerPriority = capacityPerPriority;
        totalElements = 0;
    }
    priority_queue(const priority_queue &other)
    {
        capacityPerPriority = other.capacityPerPriority;
        totalElements = other.totalElements;

        container = new std::map<int, *queue<T>();
        for (auto it = other.begin(); it != other.end(); ++it)
        {
            container[it->first] = new queue<T>(it->second);
        }
    }
    
// Element Access
    T &front()
    {
        return container->rbegin()->front();
    }
    const T &front() const
    {
        return container->rbegin()->front();
    }
    T &back()
    {
        return container->begin()->back();
    }
    const T &back() const
    {
        return container->begin()->back();
    }

// Capacity
    bool empty() const
    {
        return container->size() == 0;
    }
    int size() const
    {
        return totalElements;
    }
    int capacity() const
    {
        return container->size() * capacityPerPriority;
    }

// Modifiers
    void clear()
    {
        container->clear();
        totalElements = 0;
    }
    void push(int priority, const T &value)
    {
        if (container->find(priority) != container->end())
        {
            container[priority]->push(value);
        }
        else
        {
            queue<T> *q = new queue<T>(capacityPerPriority);
            q->push(value);
            container[priority] = q;
        }

        totalElements++;
    }
    void pop()
    {
        container->rbegin()->pop();
        if (container->rbegin()->size() == 0)
        {
            container->erase(container->rbegin());
        }

        totalElements--;
    }

// Iterators
    std::map<int, *queue<T>>::reverse_iterator rbegin()
    {
        return container->rbegin();
    }
    std::map<int, *queue<T>>::reverse_iterator rend()
    {
        return container->rend();
    }
};

#endif