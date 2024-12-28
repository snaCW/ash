#ifndef ASH_QUEUE_H_INCLUDED
#define ASH_QUEUE_H_INCLUDED

#include <stdexcept>
#include <climits>
#include <functional>

namespace ash
{
    template <class T>
    class queue;
}

template <class T>
class ash::queue
{
private:
    T *container = nullptr;
    std::size_t head;
    std::size_t allocation_size;
    std::size_t length;

public:
    // Constructors
    queue()
    {
        head = 0;
        allocation_size = 0;
        length = 0;
    }
    queue(std::size_t capacity)
    {
        head = 0;
        allocation_size = capacity;
        length = 0;
        
        container = new T[allocation_size];
    }
    queue(const queue &other)
    {
        head = other.head;
        length = other.length;
        allocation_size = other.allocation_size;

        container = new T[allocation_size];
        for (int i = 0; i < length; i++)
        {
            container[i] = other.container[i];
        }
    }
    ~queue()
    {
        delete[] container;
    }

    // Element Access
    T &front()
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty queue.");
        }
        return container[head];
    }
    const T &front() const
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty queue.");
        }
        return container[head];
    }
    T &back()
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty queue.");
        }

        int index = (head + length - 1) % allocation_size;
        return container[index];
    }
    const T &back() const
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty queue.");
        }

        int index = (head + length - 1) % allocation_size;
        return container[index];
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
            throw std::out_of_range("Assign to the queue out of the expected allocation size.");
        }

        int index = (head + length) % allocation_size;
        container[index] = value;

        length++;
    }
    void pop()
    {
        if (length == 0)
        {
            throw std::out_of_range("Access to an element from an empty queue.");
        }
        length--;
        head = (head + 1) % allocation_size;
    }
    void swap(queue &other)
    {
        std::swap(container, other.container);
        std::swap(head, other.head);
        std::swap(allocation_size, other.allocation_size);
        std::swap(length, other.length);
    }


    void iterate_on_all(const std::function<void (T &element)> &process)
    {
        for (int i = 0; i < length; i++)
        {
            int index = (head + i) % allocation_size;
            process(container[index]);
        }
    }
};

template <class T>
void std::swap(ash::queue<T> &left, ash::queue<T> &right)
{
    left.swap(right);
}

#endif