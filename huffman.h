#ifndef ASH_HUFFMAN_H_INCLUDED
#define ASH_HUFFMAN_H_INCLUDED

#include <exception>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <string>

namespace ash
{
    template <class T>
    class huffman;
}

template <class T>
class ash::huffman
{
private:
    class node;

    node root;
    std::unordered_map<T, std::string> references;

public:
    huffman() {}
    huffman(const std::map<T, size_t> &table)
    {
        std::set<node> trees;
        for (auto pair : table)
        {
            trees.emplace(pair.first, pair.second);
        }
        create_tree(trees);
        trees.clear();

        std::string address;
        create_references(&root, address);
    }
#if T == char
    std::string encode(const std::string &input)
    {
        std::string result;
        for (char value : input)
            result += references[value];
        
        return result;
    }
#endif
#if T == char
    std::string decode(const std::string &input)
    {
        std::string result;

        const node *temp = &root;
        for (char hint : input)
        {
            int dir = hint - '0';
            if ((*temp)[dir] == nullptr)
            {
                result += temp->value;
                temp = &root;
            }
            temp = (*temp)[dir];
        }
        result += temp->value;

        return result;
    }
#elif
    std::vector<T> decode(const std::string &input)
    {
        std::vector<T> result;

        node *temp = &root;
        for (char hint : input)
        {
            int dir = hint - '0';
            if (temp[dir] == nullptr)
            {
                result.push_back(temp->value);
                temp = &root;
            }
            temp = temp[dir];
        }
        result.push_back(temp->value);

        return result;
    }
#endif
    std::string encode(const std::vector<T> &input)
    {
        std::string result;
        for (const T &value : input)
            result += references[value];
        
        return result;
    }

private:
    void create_tree(std::set<node> &trees)
    {
        while (trees.size() > 1)
        {
            auto it = trees.begin();
            auto nxt = std::next(it);
            node parent(*it, *nxt);

            trees.erase(trees.begin());
            trees.erase(trees.begin());

            trees.insert(parent);
        }
        root = node(*trees.begin());
    }
    void create_references(const node *current, std::string &address)
    {
        if (current->left == nullptr && current->right == nullptr)
        {
            references[current->value] = address;
            return;
        }

        for (int i = 0; i < 2; i++)
        {
            address += i + '0';
            create_references((*current)[i], address);
            address.pop_back();
        }
    }
};

template <class T>
class ash::huffman<T>::node
{
    friend class ash::huffman<T>;

public:
    size_t weight;
    T value;

    const node *left = nullptr;
    const node *right = nullptr;

    node()
    {
        weight = 0;
        value = T();
    }
    node(const node &other)
    {
        weight = other.weight;
        value = other.value;
        left = other.left;
        right = other.right;
    }
    node(size_t weight)
    {
        this->weight = weight;
        value = T();
    }
    node(const T &value, size_t weight)
    {
        this->value = value;
        this->weight = weight;
    }
    node(const node &left, const node &right)
    {
        this->left = new node(left);
        this->right = new node(right);
        weight = left.weight + right.weight;
        value = T();
    }
    const node *operator[](int index) const
    {
        switch (index)
        {
        case 0:
            return right;
        
        case 1:
            return left;

        default:
            throw std::invalid_argument("Unexpected child.");
        }
    }
    bool operator<(const node &right) const
    {
        if (weight == right.weight)
            return value > right.value;
        return weight < right.weight;
    }
};

#endif