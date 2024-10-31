#ifndef ASH_TASK_QUEUE_H_INCLUDED
#define ASH_TASK_QUEUE_H_INCLUDED

#include "task.h"
#include "task_queue.h"
#include <stdexcept>
#include <climits>

namespace ash
{
    template <class T>
    class task_queue;
}

template <class T>
class ash::task_queue
{
private:
    task<T> *container = nullptr;
    std::size_t head;
    std::size_t allocation_size;
    std::size_t length;

public:
    // Constructors
    task_queue()
    {
        head = 0;
        allocation_size = 0;
        length = 0;
    }
    task_queue(std::size_t capacity)
    {
        head = 0;
        allocation_size = capacity;
        length = 0;
        
        container = new task<T>[allocation_size];
    }
    task_queue(const task_queue &other)
    {
        head = other.head;
        length = other.length;
        allocation_size = other.allocation_size;

        container = new task<T>[allocation_size];
        for (int i = 0; i < length; i++)
        {
            container[i] = other.container[i];
        }
    }
    ~task_queue()
    {
        for (int i = 0; i < allocation_size; i++)
        {
            delete ~container[i];
        }
        delete[] container;
    }

    // Element Access
    T &front()
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty task_queue.");
        }

        int maxPriority = 0;
        int indexOfMaxPriority = 0;
        for (int i = 0; i < length; i++)
        {
            int index = (i + head) % allocation_size;
            if (container[index].get_priority() > maxPriority)
            {
                maxPriority = container[index].get_priority();
                indexOfMaxPriority = index;
            }
        }

        return container[indexOfMaxPriority];
    }
    const T &front() const
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty task_queue.");
        }

        int maxPriority = 0;
        int indexOfMaxPriority = 0;
        for (int i = 0; i < length; i++)
        {
            int index = (i + head) % allocation_size;
            if (container[index].get_priority() > maxPriority)
            {
                maxPriority = container[index].get_priority();
                indexOfMaxPriority = index;
            }
        }

        return container[indexOfMaxPriority];
    }
    T &back()
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty task_queue.");
        }

        int minPriority = 0;
        int indexOfMinPriority = 0;
        for (int i = 0; i < length; i++)
        {
            int index = (i + head) % allocation_size;
            if (container[index].get_priority() <= minPriority)
            {
                maxPriority = container[index].get_priority();
                indexOfMinPriority = index;
            }
        }

        return container[indexOfMinPriority];
    }
    const T &back() const
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty task_queue.");
        }

        int minPriority = 0;
        int indexOfMinPriority = 0;
        for (int i = 0; i < length; i++)
        {
            int index = (i + head) % allocation_size;
            if (container[index].get_priority() <= minPriority)
            {
                maxPriority = container[index].get_priority();
                indexOfMinPriority = index;
            }
        }

        return container[indexOfMinPriority];
    }

    // Capacity
    bool empty() const
    {
        return length == 0;
    }
    int size() const
    {
        return length;
    }
    int capacity() const
    {
        return allocation_size;
    }

    // Modifiers
    void clear()
    {
        length = 0;
        head = 0;
    }
    void push(const T &value)
    {
        if (length == allocation_size)
        {
            throw std::out_of_range("Assign to the task_queue out of the expected allocation size.");
        }

        int index = (head + length) % allocation_size;
        container[index] = value;

        length++;
    }
    void pop()
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty task_queue.");
        }
        length--;
        head = (head + 1) % allocation_size;
    }
    void swap(task_queue &other)
    {
        std::swap(container, other.container);
        std::swap(head, other.head);
        std::swap(allocation_size, other.allocation_size);
        std::swap(length, other.length);
    }
};

template <class T>
void std::swap(ash::task_queue<T> &left, ash::task_queue<T> &right)
{
    left.swap(right);
}


#endif