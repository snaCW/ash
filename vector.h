#ifndef ASH_VECTOR_H_INCLUDED
#define ASH_VECTOR_H_INCLUDED

#include <stdexcept>
#include <limits>

namespace ash
{
    template <class T>
    class vector;
}

template <class T>
class ash::vector
{
public:
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;

    typedef T&          reference;
    typedef const T&    const_reference;
    typedef T*          pointer;
    typedef const T*    const_pointer;

private:
    pointer container = nullptr;
    std::size_t _capacity = 0;
    std::size_t _size = 0;

public:
    vector() : container(nullptr), _capacity(0), _size(0) {}
    
    vector(std::size_t count)
    {
        reserve(count);
        _size = count;
    }

    vector(std::size_t count, const_reference initial_value)
    {
        reserve(count);
        _size = count;

        for (std::size_t i = 0; i < _size; ++i)
            new (&container[i]) T(initial_value);
    }

    vector(const vector &other)
    {
        _size = other._size;
        _capacity = other._size;

        container = (_capacity > 0) ? new T[_capacity] : nullptr;

        for (std::size_t i = 0; i < _size; ++i)
            new (&container[i]) T(other.container[i]);
    }

    vector(vector &&other) noexcept : container(other.container), _capacity(other._capacity), _size(other._size)
    {
        other.container = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    ~vector()
    {
        destroy_elements();
        delete[] container;
        container = nullptr;
    }

    vector &operator=(const vector &other)
    {
        if (this == &other)
            return *this;

        destroy_elements();
        if (other._size > _capacity)
        {
            delete[] container;
            container = new T[other._size];
            _capacity = other._size;
        }
        
        for (std::size_t i = 0; i < other._size; ++i)
            new (&container[i]) T(other.container[i]);

        _size = other._size;
        
        return *this;
    }
    
    vector &operator=(vector &&other) noexcept
    {
        if (this == &other)
            return *this;
        
        swap(other);
        return *this;
    }

/* Element Access */

    reference at(std::size_t index)
    {
        if (index >= _size)
            throw std::out_of_range("Out of range.");

        return container[index];
    }

    const_reference at(std::size_t index) const
    {
        if (index >= _size)
            throw std::out_of_range("Out of range.");

        return container[index];
    }

    reference operator[](std::size_t index)
    {
        return container[index];
    }

    const_reference operator[](std::size_t index) const
    {
        return container[index];
    }

    reference front()
    {
        if (_size == 0)
            throw std::out_of_range("Vector is empty.");
        return container[0];
    }

    const_reference front() const
    {
        if (_size == 0)
            throw std::out_of_range("Vector is empty.");
        return container[0];
    }

    reference back()
    {
        if (_size == 0)
            throw std::out_of_range("Vector is empty.");
        return container[_size - 1];
    }

    const_reference back() const
    {
        if (_size == 0)
            throw std::out_of_range("Vector is empty.");
        return container[_size - 1];
    }

    pointer data()
    {
        return container;
    }

    const_pointer data() const
    {
        return container;
    }

/* Iterators */

/* Capacity */

    bool empty() const noexcept
    {
        return _size == 0;
    }
    
    std::size_t size() const noexcept
    {
        return _size;
    }

    std::size_t max_size() const noexcept
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= _capacity)
            return;
        
        pointer new_container = static_cast<pointer>(::operator new(new_capacity * sizeof(T)));

        for (std::size_t i = 0; i < _size; ++i)
            new (&new_container[i]) T(std::move(container[i]));
        
        destroy_elements();
        ::operator delete(container);

        container = new_container;
        _capacity = new_capacity;
    }

    std::size_t capacity() const noexcept
    {
        return _capacity;
    }

    void shrink_to_fit()
    {
        if (_size == _capacity)
            return;

        pointer new_container = static_cast<pointer>(::operator new(_size * sizeof(T)));

        for (std::size_t i = 0; i < _size; ++i)
            new (&new_container[i]) T(std::move(container[i]));

        destroy_elements();
        ::operator delete(container);

        container = new_container;
        _capacity = _size;
    }

/* Modifiers */

    void clear() noexcept
    {
        destroy_elements();
        _size = 0;
    }

    void swap(vector &other) noexcept
    {
        std::swap(container, other.container);
        std::swap(_capacity, other._capacity);
        std::swap(_size, other._size);
    }

private:
    void destroy_elements()
    {
        for (std::size_t i = 0; i < _size; ++i)
            container[i].~T();
    }
};

#endif /* vector.h */