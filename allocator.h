#ifndef ASH_ALLOCATOR_H_INCLUDED
#define ASH_ALLOCATOR_H_INCLUDED

namespace ash
{
    template <class T>
    class allocator;
}

template <class T>
class ash::allocator
{
public:
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;

private:

public:
    allocator()
    {

    }
    ~allocator()
    {

    }

    pointer allocate(size_t n)
    {
        return new T[n];
    }
    void deallocate(pointer p)
    {
        delete[] p;
    }
    void deallocate(pointer p, size_t n)
    {
        for (int i = 0; i < n; i++)
        {
            delete p + i;
        }
        delete[] p;
    }
};

#endif