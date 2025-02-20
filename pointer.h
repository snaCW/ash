#ifndef ASH_POINTER_H_INCLUDED
#define ASH_POINTER_H_INCLUDED

#include <stdexcept>
#include <utility>

namespace ash
{
    template <class T>
    class pointer;
}

template <class T>
class ash::pointer
{
private:
    T *p = nullptr;
    
public:
    pointer() noexcept = default;

    pointer(const T &value)
    {
        p = new T(value);
    }

    explicit pointer(T *raw_ptr) noexcept : p(raw_ptr) {}

    pointer(const pointer &other) = delete;

    pointer(pointer&& other) noexcept = default;

    ~pointer() = default;

    pointer &operator=(const pointer &other) = delete;

    pointer &operator=(pointer&& other) noexcept
    {
        if (this != &other)
        {
            delete p;
            p = other.p;
            other.p = nullptr;
        }
        return *this;
    }

    bool null() const noexcept
    {
        return p == nullptr;
    }

    void clear()
    {
        delete p;
        p = nullptr;
    }

    T *release() noexcept
    {
        T *temp = p;
        p = nullptr;
        return temp;
    }

    T &operator*()
    {
        if (p == nullptr)
            throw std::runtime_error("Dereferencing Null Pointer.");
        return *p;
    }
    
    const T &operator*() const
    {
        if (p == nullptr)
            throw std::runtime_error("Dereferencing Null Pointer.");
        return *p;
    }

    bool operator==(const pointer &other) const noexcept
    {
        return p == other.p;
    }
    bool operator!=(const pointer &other) const noexcept
    {
        return p != other.p;
    }

    const T *get() const noexcept
    {
        return p;
    }
};

#endif