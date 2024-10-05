#ifndef ASH_VECTOR_H_INCLUDED
#define ASH_VECTOR_H_INCLUDED

#include <stdexcept>

namespace ash
{
    template <class T>
    class vector;
}

template <class T>
class ash::vector
{
    class iterator;
    class reverse_iterator;
    
    private:
        T* array = new T[1];
        int length;
        int allocation_size;

    public:
    // Constructors
        vector()
        {
            length = 0;
            allocation_size = 1;
        }
        vector(int allocation_size)
        {
            if (allocation_size == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = allocation_size;
            this->allocation_size = allocation_size;
            array = new T[allocation_size];
            
            T defaultValue = T();
            for (int i = 0; i < length; i++)
            {
                array[i] = defaultValue;
            }
        }
        vector(int allocation_size, const T& initial_value)
        {
            if (allocation_size == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = allocation_size;
            this->allocation_size = allocation_size;
            array = new T[allocation_size];

            for (int i = 0; i < length; i++)
            {
                array[i] = initial_value;
            }
        }
        vector(const vector& v)
        {
            array = v.array;
            length = v.length;
            allocation_size = v.allocation_size;
        }

    // Assign
        void assign(int count, const T& value)
        {
            if (count == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = count;
            allocation_size = count;
            array = new T[allocation_size];

            for (int i = 0; i < length; i++)
            {
                array[i] = value;
            }
        }
        void assign(const iterator& begin, const iterator& end)
        {
            if (end - begin == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = end - begin;
            allocation_size = length;
            array = new T[allocation_size];

            int index = 0;
            for (iterator it = begin; it != end; it++)
            {
                array[index++] = *it;
            }
        }
        void assign(int startIndex, int endIndex, const vector<T>& other)
        {
            if (endIndex - startIndex == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = endIndex - startIndex;
            allocation_size = length;
            array = new T[allocation_size];

            int index = 0;
            while (startIndex < endIndex)
            {
                array[index++] = other[startIndex++];
            }
        }
        void assign(int count, T array[])
        {
            if (end - begin == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = count;
            allocation_size = count;
            this->array = new T[allocation_size];
            
            for (int i = 0; i < length; i++)
            {
                this->array[i] = array[i];
            }
        }
        void assign(int startIndex, int endIndex, T array[])
        {
            if (endIndex - startIndex == 0)
            {
                throw std::invalid_argument("Cannot initialize with size zero.");
            }

            length = endIndex - startIndex;
            allocation_size = length;
            array = new T[allocation_size];

            int index = 0;
            while (startIndex < endIndex)
            {
                array[index++] = array[startIndex++];
            }
        }
    
    // Element Access
        T& at(int index)
        {
            if (index >= length)
                throw std::out_of_range("Access to an out of range index.");
            return array[index];
        }
        const T& at(int index) const
        {
            if (index >= length)
                throw std::out_of_range("Access to an out of range index.");
            return array[index];
        }
        T& operator[](int index)
        {
            if (index >= length)
                throw std::out_of_range("Access to an out of range index.");
            return array[index];
        }
        const T& operator[](int index) const
        {
            if (index >= length)
                throw std::out_of_range("Access to an out of range index.");
            return array[index];
        }
        T& front()
        {
            if (length == 0)
            {
                throw std::out_of_range("Access to an out of range index.");
            }
            return array[0];
        }
        const T& front() const
        {
            if (length == 0)
            {
                throw std::out_of_range("Access to an out of range index.");
            }
            return array[0];
        }
        T& back()
        {
            if (length == 0)
            {
                throw std::out_of_range("Access to an out of range index.");
            }
            return array[length - 1];
        }
        const T& back() const
        {
            if (length == 0)
            {
                throw std::out_of_range("Access to an out of range index.");
            }
            return array[length - 1];
        }
        T* data()
        {
            return array;
        }
        const T* data() const
        {
            return array;
        }
    
    // Iterators
        iterator begin()
        {
            return iterator(array, this);
        }
        reverse_iterator rbegin()
        {
            return reverse_iterator(array + length - 1, this);
        }
        iterator end()
        {
            return iterator(array + length, this);
        }
        reverse_iterator rend()
        {
            return reverse_iterator(array - 1, this);
        }

        iterator cbegin() const
        {
            return iterator(array, this);
        }
        reverse_iterator crbegin() const
        {
            return reverse_iterator(array + length - 1, this);
        }
        iterator cend() const
        {
            return iterator(array + length, this);
        }
        reverse_iterator crend() const
        {
            return reverse_iterator(array - 1, this);
        }

    // Capacity
        bool empty() const
        {
            return (length == 0);
        }
        int size() const
        {
            return length;
        }
        int max_size() const
        {
            return INT_MAX;
        }
        void reserve(int allocation_size)
        {
            if (allocation_size <= this->allocation_size)
            {
                return;
            }

            this->allocation_size = allocation_size;
            T* array = new T[allocation_size];

            for (int i = 0; i < length; i++)
            {
                array[i] = this->array[i];
            }
            this->array = array;
        }
        int capacity() const
        {
            return allocation_size;
        }
        void shrink_to_fit()
        {
            if (length == allocation_size)
            {
                return;
            }
            
            allocation_size = length;
            T* array = new T[allocation_size];

            for (int i = 0; i < length; i++)
            {
                array[i] = this->array[i];
            }
            this->array = array;
        }

    // Modifiers
        void clear()
        {
            delete array;
            array = new T[1];
            length = 0;
            allocation_size = 1;
        }
        void hard_clear()
        {
            T defaultValue = T();
            for (int i = 0; i < allocation_size; i++)
            {
                array[i] = defaultValue;
            }
            clear();
        }
        void push_back(const T& value)
        {
            if (length + 1 > allocation_size)
            {
                resize();
            }
            array[length] = value;
            length++;
        }
        void pop_back()
        {
            length--;
        }
        void hard_pop_back()
        {
            array[length - 1] = T();
            pop_back();
        }

    private:
        void resize()
        {
            T* array = new T[allocation_size * 2];
            for (int i = 0; i < allocation_size; i++)
            {
                array[i] = this->array[i];
            }
            allocation_size *= 2;
            this->array = array;
        }
};

template <class T>
class ash::vector<T>::iterator
{
    protected:
        T* pointer = nullptr;
        const vector* _vector = nullptr;

    public:
        iterator() {}
        iterator(T* pointer, vector* _vector)
        {
            this->pointer = pointer;
            this->_vector = _vector;
        }
        iterator(T* pointer, const vector* _vector)
        {
            this->pointer = pointer;
            this->_vector = _vector;
        }
        iterator& operator++()
        {
            ++pointer;
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            ++pointer;
            return it;
        }
        iterator& operator--()
        {
            --pointer;
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            --pointer;
            return it;
        }
        bool operator==(const iterator& it) const
        {
            if (pointer == it.pointer)
                return true;
            return false;
        }
        bool operator!=(const iterator& it) const
        {
            if (pointer != it.pointer)
                return true;
            return false;
        }
        T& operator*()
        {
            return *pointer;
        }
        const T& operator*() const
        {
            return *pointer;
        }
        int operator-(const iterator& it) const
        {
            return pointer - it.pointer;
        }
        iterator operator-(int i) const
        {
            return iterator(pointer - i, _vector);
        }
        iterator operator+(int i) const
        {
            return iterator(pointer + i, _vector);
        }
        int distance(const iterator& it) const
        {
            return pointer - it.pointer;
        }
};

template <class T>
class ash::vector<T>::reverse_iterator : public ash::vector<T>::iterator
{
    public:
        reverse_iterator() : iterator() {}
        reverse_iterator(T* pointer) : iterator(pointer) {}
        reverse_iterator& operator++()
        {
            --(this->pointer);
            return *this;
        }
        reverse_iterator operator++(int)
        {
            reverse_iterator it = *this;
            --(this->pointer);
            return it;
        }
        reverse_iterator& operator--()
        {
            ++(this->pointer);
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator it = *this;
            ++(this->pointer);
            return it;
        }
        bool operator==(const reverse_iterator& it)
        {
            if (this->pointer == it.pointer)
                return true;
            return false;
        }
        bool operator!=(const reverse_iterator& it)
        {
            if (this->pointer != it.pointer)
                return true;
            return false;
        }
        T& operator*()
        {
            return *this->pointer;
        }
        const T& operator*() const
        {
            return *this->pointer;
        }
        int operator-(const reverse_iterator& it) const
        {
            return it.pointer - this->pointer;
        }
        reverse_iterator operator-(int i) const
        {
            return reverse_iterator(this->pointer + i, this->_vector);
        }
        reverse_iterator operator+(int i) const
        {
            return reverse_iterator(this->pointer - i, this->_vector);
        }
        int distance(const reverse_iterator& it)
        {
            return it.pointer - this->pointer;
        }
};

#endif