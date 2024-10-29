#ifndef ASH_STRING_H_INCLUDED
#define ASH_STRING_H_INCLUDED

#include "vector.h"

namespace ash
{
    class string;
}

class string : public ash::vector<char>
{
    public:
    // Constructors
        string() : vector() {}
        string(int allocation_size) : vector(allocation_size) {}
        string(const string& str) : vector(str) {}

    // Element Access
        const char* c_str() const
        {
            return data();
        }

    // Modifiers
        string& operator+=(char c)
        {
            push_back(c);
            return *this;
        }
        string& operator+=(const string& other)
        {
            for (int i = 0; i < other.length; i++)
            {
                push_back(other[i]);
            }
            return *this;
        }
    
    // Search
        int find(const string& substring)
        {
            for (int i = 0; i < length - substring.length; i++)
            {
                if (array[i] == substring[i])
                {
                    int index = 1;
                    while (index < substring.length)
                    {
                        if (i + index >= length || array[i + index] != substring[index])
                        {
                            index = substring.length;
                            continue;
                        }
                        index++;
                    }
                    if (index != substring.length)
                    {
                        return index;
                    }
                }
            }

            return -1;
        }
};

#endif