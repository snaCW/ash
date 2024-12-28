#ifndef ASH_LIST_H_INCLUDED
#define ASH_LIST_H_INCLUDED

#include <stdexcept>

namespace ash
{
    template <class T>
    class list;
}

template <class T>
class ash::list
{
// Nested Classes
    private:
        class node;
    
    public:
        class iterator;
        class reverse_iterator;

// Typedef
    public:
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;

        typedef node*       node_pointer;
        typedef node&       node_reference;

// Private data members
    private:
        node *head = nullptr;
        size_t length;
    
    public:
// Constructors
        list()
        {
            length = 0;
        }
        list(const_reference value)
        {
            head = new node(value);
            head->set_next(head);
            length = 1;
        }
        list(const_pointer pointer_to_value)
        {
            head = new node(*pointer_to_value);
            head->set_next(head);
            length = 1;
        }
        list(const list &other)
        {
            if (other.length == 0)
                return;
            
            const iterator it = other.begin();
            const iterator end = other.end();
            do
            {
                push_back(new T(*it));
                ++it;
            } while (it != end);
            length = other.length;
        }
// Destructor
        ~list()
        {
            // To-do
        }

// Basic Information
        size_t size()
        {
            return length;
        }

// Element Access
        reference front()
        {
            return *(head->obj);
        }
        const_reference front() const
        {
            return *(head->obj);
        }
        
        reference back()
        {
            return *(head->prev->obj);
        }
        const_reference back() const
        {
            return *(head->prev->obj);
        }
        
// Modifiers
        void clear()
        {
            if (length == 0)
                return;

            node *temp = head;
            head->prev->next = nullptr;
            while (temp->next != nullptr)
            {
                node *next = temp->next;
                delete temp;
                temp = next;
                delete next;
            }
            delete temp;

            head = nullptr;
        }
        iterator insert(const iterator &pos, const_reference value)
        {
            return iterator::insert(pos, value);
        }
        reverse_iterator insert(const reverse_iterator &pos, const_reference value)
        {
            return reverse_iterator::insert(pos, value);
        }
        void pop_back()
        {
            node *last = head->prev;
            last->prev->set_next(head);

            length--;
            delete last;
        }
        void pop_front()
        {
            node *first = head;
            head->prev->set_next(head->next);

            length--;
            head = first->next;
            delete first;
        }
        void push_back(const_reference value)
        {
            if (length == 0)
            {
                head = new node(value);
                head->set_next(head);
                length++;
                return;
            }

            node *last = head->prev;
            node *new_node = new node(value);
            last->set_next(new_node);
            new_node->set_next(head);

            length++;
            //delete last;
        }
        void push_back(const_pointer pointer_to_value)
        {
            push_back(*pointer_to_value);
        }
        void push_front(const_reference value)
        {
            push_back(value);
            head = head->prev;
        }
        void push_front(const_pointer pointer_to_value)
        {
            push_back(pointer_to_value);
            head = head->prev;
        }
        iterator erase(const iterator &pos)
        {
            return iterator::erase(pos);
        }
        reverse_iterator erase(const reverse_iterator &pos)
        {
            return reverse_iterator::erase(pos);
        }

// Iterators
        iterator begin()
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have a beginning.");
            }
            return iterator(head);
        }
        const iterator begin() const
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have a beginning.");
            }
            return iterator(head);
        }
        reverse_iterator rbegin()
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have a reverse beginning.");
            }
            return reverse_iterator(head->prev);
        }
        const reverse_iterator rbegin() const
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have a reverse beginning.");
            }
            return reverse_iterator(head->prev);
        }

        iterator end()
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have an end.");
            }
            return iterator(head);
        }
        const iterator end() const
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have an end.");
            }
            return iterator(head);
        }
        reverse_iterator rend()
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have a reverse end.");
            }
            return reverse_iterator(head->prev);
        }
        const reverse_iterator rend() const
        {
            if (length == 0)
            {
                throw std::range_error("Empty list does not have a reverse end.");
            }
            return reverse_iterator(head->prev);
        }
};

template <class T>
class ash::list<T>::node
{
public:
    pointer pointer_to_value = nullptr;
    node *next = nullptr;
    node *prev = nullptr;

// Constructors
    node() {}
    node(const_reference value)
    {
        pointer_to_value = new T(value);
    }
    node(pointer pointer_to_value)
    {
        this->pointer_to_value = pointer_to_value;
    }
    node(const_reference value, node *next, node *prev)
    {
        pointer_to_value = new T(value);
        this->next = next;
        this->prev = prev;
    }
    node(const node &other)
    {
        pointer_to_value = other.pointer_to_value;
        next = other.next;
        prev = other.prev;
    }

// Destructor
    ~node()
    {
        //delete pointer_to_value;
        //pointer_to_value = nullptr;
        // delete next;
        // delete prev;
    }

// set_next methods
    void set_next(const_reference value)
    {
        next = new node(value, nullptr, this);
    }
    void set_next(node &next_node)
    {
        next_node.prev = this;
        next = &next_node;
    }
    void set_next(node *next_node)
    {
        next_node->prev = this;
        next = next_node;
    }

// set_prev methods
    void set_prev(const_reference value)
    {
        prev = new node(value, this, nullptr);
    }
    void set_prev(node &prev_node)
    {
        prev_node.next = this;
        prev = &prev_node;
    }
    void set_prev(node *prev_node)
    {
        prev_node->next = this;
        prev = prev_node;
    }
};

template <class T>
class ash::list<T>::iterator
{
friend class ash::list<T>;

private:
    node *target = nullptr;
    
public:
    iterator() {}
    iterator(node *target)
    {
        this->target = target;
    }
    iterator(const iterator &other)
    {
        target = other.target;
    }
    ~iterator()
    {
        // delete target;
    }

    void swap(iterator &other)
    {
        node *prev_of_this = target->prev;
        node *next_of_this = target->next;
        node *prev_of_other = other.target->prev;
        node *next_of_other = other.target->next;

        target->set_prev(prev_of_other);
        target->set_next(next_of_other);
        other.target->set_prev(prev_of_this);
        other.target->set_next(next_of_this);

        node *target_of_other = other.target;
        other.target = target;
        target = target_of_other;

        delete prev_of_this;
        delete next_of_this;
        delete prev_of_other;
        delete next_of_other;
        
        delete target_of_other;
    }
    
    int operator-(const iterator &other) const
    {
        int distance = 0;
        iterator it = other;
        while (it.target != target)
        {
            if (distance != 0 && it.target == other.target)
            {
                ~it;
                return -1;
            }
            
            ++distance;
            ++it;
        }

        ~it;
        return distance;
    }
    iterator operator-(int count) const
    {
        if (count < 0)
        {
            return *this + -count;
        }

        iterator it = *this;
        for (int i = 0; i < count; i++)
            --it;
        
        return it;
    }
    iterator operator+(int count) const
    {
        if (count < 0)
        {
            return *this - -count;
        }

        iterator it = *this;
        for (int i = 0; i < count; i++)
            ++it;
        
        return it;
    }
    iterator &operator++()
    {
        target = target->next;
        return *this;
    }
    iterator operator++(int)
    {
        iterator before = *this;
        target = target->next;
        return before;
    }
    iterator &operator--()
    {
        target = target->prev;
        return *this;
    }
    iterator operator--(int)
    {
        iterator after = *this;
        target = target->prev;
        return after;
    }
    void operator=(const iterator &other)
    {
        target = other.target;
    }
    reference operator*()
    {
        return *(target->pointer_to_value);
    }
    const_reference operator*() const
    {
        return *(target->pointer_to_value);
    }
    bool operator==(const iterator &other) const
    {
        return target == other.target;
    }
    bool operator!=(const iterator &other) const
    {
        return target != other.target;
    }

    reverse_iterator reverse()
    {
        return reverse_iterator(target);
    }
    /*
    const reverse_iterator reverse() const
    {
        return reverse_iterator(target);
    }
    */

private:
    static iterator insert(const iterator &pos, const_reference value)
    {
        iterator temp = pos;

        node *prev = pos.target->prev;
        node *new_node = new node(value);
        prev->set_next(new_node);
        new_node->set_next(temp.target);

        // delete prev;
        return iterator(new_node);
    }
    static iterator erase(const iterator &pos)
    {
        node *prev_node = pos.target->prev;
        prev_node->set_next(pos.target->next);
        return pos + 1;
    }
};

/*
template <class T>
class ash::list<T>::reverse_iterator
{
    friend class ash::list<T>;

    private:
        node *target = nullptr;
    
    public:
        reverse_iterator() {}
        reverse_iterator(node *target)
        {
            this->target = target;
        }
        ~reverse_iterator()
        {
            delete target;
        }

        void swap(reverse_iterator &other)
        {
            node *prev_of_this = target->prev;
            node *next_of_this = target->next;
            node *prev_of_other = other->target->prev;
            node *next_of_other = other->target->next;

            target->set_prev(prev_of_other);
            target->set_next(next_of_other);
            other->target->set_prev(prev_of_this);
            other->target->set_next(next_of_this);

            node *target_of_other = other->target;
            other->target = target;
            target = target_of_other;

            delete prev_of_this;
            delete next_of_this;
            delete prev_of_other;
            delete next_of_other;
            delete target_of_other;
        }
        
        long long operator-(const reverse_iterator &other) const
        {
            long long distance = 0;
            reverse_iterator it = other;
            while (it->target != target)
            {
                if (distance != 0 && it->target == other->target)
                {
                    delete it->target;
                    return -1;
                }
                
                distance++;
                ++it;
            }

            delete it->target;
            return distance;
        }
        reverse_iterator operator-(long long count) const
        {
            reverse_iterator it = *this;
            for (long long i = 0; i < count; i++)
                --it;
            
            return it;
        }
        reverse_iterator operator+(long long count) const
        {
            reverse_iterator it = *this;
            for (long long i = 0; i < count; i++)
                ++it;
            
            return it;
        }
        reverse_iterator &operator++() const
        {
            target = target->prev;
            return *this;
        }
        reverse_iterator operator++(int) const
        {
            reverse_iterator before = *this;
            target = target->prev;
            return before;
        }
        reverse_iterator &operator--() const
        {
            target = target->next;
            return *this;
        }
        reverse_iterator operator--(int) const
        {
            reverse_iterator after = *this;
            target = target->next;
            return after;
        }
        reference operator*()
        {
            return *(target->obj);
        }
        const_reference operator*() const
        {
            return *(target->obj);
        }
        bool operator!=(const reverse_iterator &other) const
        {
            return target != other.target;
        }

        iterator forward()
        {
            return iterator(target);
        }
        const iterator forward() const
        {
            return iterator(target);
        }

    private:
        static reverse_iterator insert(const reverse_iterator &pos, const_reference value)
        {
            reverse_iterator temp = pos;

            node *next = pos->next;
            node *new_node = new node(value);
            next->set_prev(new_node);
            new_node->set_prev(temp);

            delete temp;
            delete next;
            return reverse_iterator(new_node);
        }
        static reverse_iterator erase(const reverse_iterator &pos)
        {
            node *prev_node = pos->target->next;
            prev_node->set_next(pos->target->next);
            return pos + 1;
        }
};
*/

#endif