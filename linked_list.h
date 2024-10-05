#ifndef ASH_LINKLIST_H_INCLUDED
#define ASH_LINKLIST_H_INCLUDED

#include <stdexcept>

namespace ash
{
    template <class T>
    class linked_list;
}

template <class T>
class ash::linked_list
{
    class node;
    class iterator;
    class reverse_iterator;

    private:
        node* start = nullptr;
        int length = 0;
    
    public:
        linked_list()
        {
            start = new node();
            start->set_next(start);
        }
        
        void push_back(const T& element)
        {
            node* newNode = new node(element);

            node* last = &start->rprev();
            last->set_next(newNode);
            newNode->set_next(start);

            length++;
        }
        iterator begin()
        {
            if (start == &start->rnext())
            {
                throw std::range_error("Empty linked list does not have a beginning.");
            }
            return iterator(&start->rnext());
        }
        reverse_iterator rbegin()
        {
            if (start == &start->rprev())
            {
                throw std::range_error("Empty linked list does not have a reverse beginning.");
            }
            return reverse_iterator(&start->rprev());
        }
        iterator end()
        {
            return iterator(start);
        }
        reverse_iterator rend()
        {
            return reverse_iterator(start);
        }
};

template <class T>
class ash::linked_list<T>::node
{
    private:
        T obj;
        node* next = nullptr;
        node* prev = nullptr;
    
    public:
        node() {}
        node(const T& obj)
        {
            this->obj = obj;
        }
        node(const T& obj, node& next, node& prev)
        {
            this->obj = obj;
            this->next = next;
            this->prev = prev;
        }
        node(const node& otherNode)
        {
            obj = otherNode.obj;
            next = otherNode.next;
            prev = otherNode.prev;
        }
        
        void set_next(const T& obj)
        {
            node* nextNode = new node(obj, nullptr, this);
            next = nextNode;
        }
        void set_next(node& nextNode)
        {
            nextNode.prev = this;
            next = &nextNode;
        }
        void set_next(node* nextNode)
        {
            nextNode->prev = this;
            next = nextNode;
        }
        void set_prev(const T& obj)
        {
            node* prevNode = new node(obj, this, nullptr);
            prev = prevNode;
        }
        void set_prev(node& prevNode)
        {
            prevNode.next = this;
            prev = &prevNode;
        }
        void set_prev(node* prevNode)
        {
            prevNode->next = this;
            prev = prevNode;
        }
        
        void delete_next()
        {
            delete next;
            next = nullptr;
        }
        void delete_prev()
        {
            delete prev;
            prev = nullptr;
        }
        void delete_members()
        {
            delete_next();
            delete_prev();
        }
        
        void clear_obj()
        {
            obj = T();
        }
        void clear_next()
        {
            next = nullptr;
        }
        void clear_prev()
        {
            prev = nullptr;
        }
        void clear()
        {
            clear_obj();
            clear_next();
            clear_prev();
        }
        
        T& robj()
        {
            return obj;
        }
        node& rnext()
        {
            return *next;
        }
        node& rprev()
        {
            return *prev;
        }
        
        const T& cobj() const
        {
            return *obj;
        }
        const node& cnext() const
        {
            return *next;
        }
        const node& cprev() const
        {
            return *prev;
        }
        
        bool has_obj() const
        {
            return obj != nullptr;
        }
        bool has_next() const
        {
            return next != nullptr;
        }
        bool has_prev() const
        {
            return prev != nullptr;
        }
};

template <class T>
class ash::linked_list<T>::iterator
{
    protected:
        node* target = nullptr;
    
    public:
        iterator() {}
        iterator(node* target)
        {
            this->target = target;
        }
        
        iterator& operator++()
        {
            target = &target->rnext();
            return *this;
        }
        iterator operator++(int)
        {
            node before = *this;
            target = &target->rnext();
            return before;
        }
        iterator& operator--()
        {
            target = &target->rprev();
            return *this;
        }
        iterator operator--(int)
        {
            node after = *this;
            target = &target->rprev();
            return after;
        }
        T& operator*() const
        {
            return target->robj();
        }
        bool operator!=(const iterator& it) const
        {
            return target != it.target;
        }
};

template <class T>
class ash::linked_list<T>::reverse_iterator : public ash::linked_list<T>::iterator
{
    /*private:
        node* target = nullptr;*/
    
    public:
        reverse_iterator() : iterator() {}
        reverse_iterator(node* target) : iterator(target) {}
        
        reverse_iterator& operator++()
        {
            this->target = &target->rprev();
            return *this;
        }
        reverse_iterator operator++(int)
        {
            node before = *this;
            this->target = &target->rprev();
            return before;
        }
        reverse_iterator& operator--()
        {
            this->target = &target->rnext();
            return *this;
        }
        reverse_iterator operator--(int)
        {
            node after = *this;
            this->target = &target->rnext();
            return after;
        }
        T& operator*() const
        {
            return this->target->robj();
        }
        bool operator!=(const reverse_iterator& it) const
        {
            return this->target != it.target;
        }
};

#endif