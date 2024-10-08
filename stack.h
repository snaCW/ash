#ifndef ASH_STACK_H_INCLUDED
#define ASH_STACK_H_INCLUDED

#include <stdexcept>

namespace ash
{
    template <class T>
    class stack;
}

template <class T>
class ash::stack
{
    private:
        T* array = new T[1];
        int allocation_size;
        int length;
    
    public:
    // Constructors
        stack()
        {
            allocation_size = 1;
            length = 0;
        }
        stack(int capacity)
        {
            delete[] array;
            array = new T[capacity];
            allocation_size = capacity;
            length = 0;
        }
        stack(const stack& st)
        {
            array = st.array;
            allocation_size = st.allocation_size;
            length = st.length;
        }
    
    // Element Access
        T& top()
        {
            if (length == 0)
            {
                throw std::out_of_range("Stack Underflow");
            }
            return array[length - 1];
        }
    
    // Capacity
        bool empty()
        {
            return length == 0;
        }
        int size()
        {
            return length;
        }
        int capacity()
        {
            return allocation_size;
        }
        
    // Modifiers
        void push(const T& value)
        {
            if (length == allocation_size)
            {
                throw std::out_of_range("Stack Overflow");
            }
            array[length++] = value;
        }
        T& pop()
        {
            if (length == 0)
            {
                throw std::out_of_range("Stack Underflow");
            }
            return array[--length];
        }
        void create(int capacity)
        {
            delete[] array;
            array = new T[capacity];
            allocation_size = capacity;
            length = 0;
        }
        void clear()
        {
            delete[] array;
            array = new T[1];
            allocation_size = 1;
            length = 0;
        }
};

#endif