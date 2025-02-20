#ifndef ASH_NULLABLE_H_INCLUDED
#define ASH_NULLABLE_H_INCLUDED

#include <stdexcept>

namespace ash
{
    template <class T>
    class nullable;
}

template <class T>
class ash::nullable
{
private:
    T *p = nullptr;

public:
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef T*          pointer;
    typedef const T*    const_pointer;

public:
    nullable() = default;
    explicit nullable(const_reference value) : p(new T(value)) {}
    explicit nullable(pointer ptr) : p(ptr) {}
    nullable(const_pointer ptr)
    {
        p = ptr ? new T(*ptr) : nullptr;
    }
    nullable(T&& obj) : p(new T(std::move(obj))) {}
    nullable(const nullable& other)
    {
        if (other.p)
            p = new T(*other.p);
    }
    nullable(nullable&& other) noexcept : p(other.p)
    {
        other.p = nullptr;
    }
    nullable& operator=(const nullable& other)
    {
        if (this == &other)
            return *this;
        
        delete p;
        p = other.p ? new T(*other.p) : nullptr;
        return *this;
    }
    nullable& operator=(nullable&& other)
    {
        if (this != &other)
        { 
            delete p;
            p = other.p;
            other.p = nullptr;
        }

        return *this;
    }
    void clear()
    {
        delete p;
        p = nullptr;
    }
    bool null() const
    {
        return p == nullptr;
    }
    T &operator*()
    {
        if (!p)
            throw std::runtime_error("Dereferencing Null Pointer.");
        return *p;
    }
    const T &operator*() const
    {
        if (!p)
            throw std::runtime_error("Dereferencing Null Pointer.");
        return *p;
    }
    bool operator==(const_reference value) const
    {
        return p && (*p == value);
    }
    bool operator==(pointer ptr) const
    {
        if (!p || !ptr)
            return p == ptr;

        return *p == *ptr;
    }
    T value_or(const_reference default_value) const
    {
        return p ? *p : default_value;
    }
    ~nullable()
    {
        delete p;
        p = nullptr;
    }
};

#endif